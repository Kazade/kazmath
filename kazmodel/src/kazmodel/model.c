#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <kazmath/quaternion.h>
#include <kazmath/mat4.h>

#include "model.h"
#include "md3_model.h"

static int counter = 0;
static kmModel** modelArray = NULL;
static int arraySize = 0;
static int numModels = 0;
static kmModel* currentModel = NULL;

static KMenum error = KM_NO_ERROR;

void destructModel(kmModel* model) {
    //Do destruction stuff
    free(model);
}

void addToArray(kmModel* newModel) {
    if (numModels == arraySize) {
        if (arraySize == 0) {
            arraySize = 3; //Start with 3
        } else {
            arraySize *= 2; //Double the array size
        }

        modelArray = (kmModel**) realloc(modelArray, (arraySize * sizeof(kmModel*)));

        if (modelArray == NULL) {
            fprintf(stderr, "FATAL ERROR: Couldn't resize model array\n");
            exit(2);
        }
    }

    modelArray[numModels] = newModel;
    numModels++;
}

void deleteFromArray(unsigned modelID) {
    int i = 0;
    for (i = 0; i < numModels; ++i) {
        //If there is a model here, and the ID matches then free the model
        if (modelArray[i] && modelArray[i]->_modelID == modelID) {
            destructModel(modelArray[i]);
            modelArray[i] = NULL;
        }
    }
}

void freeMesh(kmModelMesh* m) {
    if (m->_indices) {
        free(m->_indices);
    }

    m->_indices = NULL;
    m->_numIndices = 0;

    if (m->_vertices) {
        free(m->_vertices);
    }

    m->_vertices = NULL;
    m->_numVertices = 0;
}

void freePart(kmModelPart* p) {
    int i = 0;
    if (p->_animations) {
        free(p->_animations);
    }
    //p->_animations = NULL;
    p->_numAnimations = 0;

    if (p->_meshes) {
        for (i = 0; i < p->_numMeshes; ++i) {
            freeMesh(&p->_meshes[i]);
        }
        free(p->_meshes);
    }
    p->_meshes = NULL;
    p->_numMeshes = 0;

    if (p->_tags) {
        free(p->_tags);
    }
    p->_tags = NULL;
    p->_numTags = 0;
    p->_numFrames = 0;
    p->_currentAnimation = 0;
    p->_currentFrame = 0;
}

kmModel* getBoundModel(void) {
    return currentModel;
}

/**
    Generate a number of models and set the handles
*/
void kmGenModels(KMuint n, KMuint* models) {
    if (!models) {
        return;
    }

    if (n < 0) {
        error = KM_INVALID_VALUE;
        return;
    }

    if (n == 0) {
        currentModel = NULL;
        return;
    }

    int i = 0;
    fprintf(stdout, "Generating IDs for %d models\n", n);

    for (i = 0; i < n; ++i) {
        *(models + i) = ++counter;

        fprintf(stdout, "Allocating memory for model with ID %d\n", counter);
        kmModel* newModel = (kmModel*) malloc(sizeof(kmModel));
        newModel->_modelID = counter;
        newModel->_parts = NULL;
        newModel->_numParts = 0;
        //Create a new model and add it to the array
        addToArray(newModel);
    }

    fprintf(stdout, "Model array size: %d\n", numModels);
}

void kmDeleteModels(KMuint n, KMuint* models) {
    if (n < 0) {
        error = KM_INVALID_VALUE;
        return;
    }

    int i = 0;
    for(i = 0; i < n; ++i) {
        deleteFromArray(models[i]);
        models[i] = -1;
    }

    kmBindModel(0);
}

KMboolean kmLoadMD3ModelFromFile(const KMbyte* filename) {
    if (!currentModel) {
        error = KM_INVALID_OPERATION;
        return KM_FALSE;
    }

    if (loadMD3Model(currentModel, filename)) {
        kmLinkModel();
        return KM_TRUE;
    } else {
        error = KM_INVALID_VALUE;
        return KM_FALSE;
    }
/*
    if (strstr(filename, ".md3") != 0 || strstr(filename, ".MD3") != 0) {
        fprintf(stdout, "Determined model type as: MD3\n");
        //It's a quake 3 model we are loading
        return loadMD3Model(currentModel, filename);
    }*/

    error = KM_INVALID_VALUE;
    fprintf(stderr, "Could not determine model type\n");
    return KM_FALSE;
}

/** Bind the model for further operations */
void kmBindModel(KMuint n) {

    if (n == 0) {
        currentModel = NULL;
        return;
    }

    int i = 0;
    for (i = 0; i < numModels; ++i) {
        if (modelArray[i] && modelArray[i]->_modelID == n) {
            fprintf(stdout, "Bound model with ID %d\n", n);
            currentModel = modelArray[i];
            return;
        }
    }

    fprintf(stderr, "Couldn't find model %d\n", n);
    error = KM_INVALID_VALUE;
    return;
}

void kmUpdateModel(float dT) {
    kmModel* model = getBoundModel();
    int i = 0;
    for (i = 0; i < model->_numParts; ++i) {
        kmModelPart* p = &model->_parts[i];

        //Do some bounds checking
        if (p->_currentFrame >= p->_numFrames) {
            p->_currentFrame = p->_currentAnimation->_startFrame;
        }

        if (p->_nextFrame >= p->_numFrames) {
            p->_nextFrame = p->_currentAnimation->_startFrame;
        }

        //If there is no animation set, then don't do any update
        if (!p->_currentAnimation) {
            continue;
        }

        //Update the frame interpolation
        p->_frameInterpolation += dT * p->_currentAnimation->_fps;

        //If it's greater than one, move along the frames
        if (p->_frameInterpolation >= 1.0f) {
            p->_frameInterpolation = 0.0f;
            p->_currentFrame = p->_nextFrame;
            p->_nextFrame++;

            //If we are at the end of the animation
            if (p->_nextFrame >= p->_currentAnimation->_endFrame) {
                //Set the next frame to the start of the animation
                p->_nextFrame = p->_currentAnimation->_startFrame;
                //Unless we have a looping frame, in which case we set it to the end - looping
                if (p->_currentAnimation->_loopingFrames) {
                    p->_nextFrame = (p->_currentAnimation->_endFrame - p->_currentAnimation->_loopingFrames);
                }
            }
        }
    }
}

static void renderModelPart(kmModelPart* part, kmModelPart* parent) {
    int j = 0;

    assert(part->_currentFrame < part->_numFrames);
    assert(part->_nextFrame < part->_numFrames);

    for (j = 0; j < part->_numMeshes; ++j) {
        //kmModelKeyframe* f = &part->_meshes[j]._keyFrames[part->_currentFrame];
        kmVec3* verts = part->_meshes[j]._vertices + (part->_currentFrame * part->_meshes[j]._numVertices);
        glVertexPointer(3, GL_FLOAT, 0, verts);
        glDrawElements(GL_TRIANGLES, part->_meshes[j]._numIndices, GL_UNSIGNED_INT, part->_meshes[j]._indices);
    }

    //If there are no linked models we are done
    if (!part->_linkedModels) {
        return;
    }
    //return;

    //Else
    for (j = 0; j < part->_numTags; ++j) {
        if (!part->_linkedModels[j]) {
            continue; //Not all placeholders will be linked to a model
        }

      /*  if (part->_linkedModels[j] == parent) {
            continue;
        }*/

        kmVec3 position, nextPosition;
        kmVec3Assign(&position, &part->_tags[part->_currentFrame * part->_numTags + j]._origin);
        kmVec3Assign(&nextPosition, &part->_tags[part->_nextFrame * part->_numTags + j]._origin);

        position.x = position.x + part->_frameInterpolation * (nextPosition.x - position.x);
        position.y = position.y + part->_frameInterpolation * (nextPosition.y - position.y);
        position.z = position.z + part->_frameInterpolation * (nextPosition.z - position.z);

        kmQuaternion rotation, nextRotation, interp;
        kmQuaternionRotationMatrix(&rotation, &part->_tags[part->_currentFrame * part->_numTags + j]._axis);
        kmQuaternionRotationMatrix(&nextRotation, &part->_tags[part->_nextFrame * part->_numTags + j]._axis);
        kmQuaternionSlerp(&interp, &rotation, &nextRotation, part->_frameInterpolation);

        kmMat4 final;
        kmMat4Identity(&final);
        kmMat4RotationQuaternion(&final, &rotation);

        final.mat[12] = position.x;
        final.mat[13] = position.y;
        final.mat[14] = position.z;
        final.mat[15] = 1.0f;

        glPushMatrix();
            glMultMatrixf(final.mat);
            renderModelPart(part->_linkedModels[j], part);
        glPopMatrix();
    }
}

void kmSetPartFrame(KMint part, KMint i) {
    kmModel* bound = getBoundModel();
    if (!bound) {
        error = KM_INVALID_OPERATION;
        return;
    }

    if (part < 0 || part > bound->_numParts) {
        error = KM_INVALID_VALUE;
        return;
    }

    if (i < 0 || i > bound->_parts[part]._numFrames) {
        error = KM_INVALID_VALUE;
        return;
    }

    bound->_parts[part]._currentFrame = i;
}

void kmRenderModel() {
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1, 1, 1);
    glDisable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);

    kmModel* model = getBoundModel();

    renderModelPart(&model->_parts[0], NULL);

    glDisableClientState(GL_VERTEX_ARRAY);
}

kmAnimation* findAnimation(kmModelPart* part, const char* name) {
    int i = 0;
    for(i = 0; i < part->_numAnimations; ++i) {
        if (!strcmp(part->_animations[i]._name, name)) {
            return &part->_animations[i];
        }
    }
    return NULL;
}

void kmSetPartAnimation(unsigned part, const char* name) {
    kmModel* model = getBoundModel();

    if (part >= model->_numParts) {
        fprintf(stderr, "Invalid part number specified %d\n", part);
        return;
    }

    kmModelPart* p = &model->_parts[part];
    kmAnimation* ani = findAnimation(p, name);
    if (ani == NULL) {
        fprintf(stderr, "Invalid animation %s\n", name);
        return;
    }

    p->_frameInterpolation = 0.0f;
    p->_currentFrame = ani->_startFrame;
    p->_nextFrame = p->_currentFrame + 1;

    if (p->_nextFrame >= ani->_endFrame) {
        p->_nextFrame = ani->_startFrame;
    }
    p->_currentAnimation = ani;
}

KMenum kmGetError(void) {
    KMenum e = error;
    error = KM_NO_ERROR;
    return e;
}

///Create a model made up of 1 or more parts (MD2 = 1, MD3 = 3 (or 4) etc.
void kmInitModel(KMint numParts) {
    kmModel* m = getBoundModel();
    int i = 0;

    if (m == NULL) {
        fprintf(stderr, "No model is bound\n");
        error = KM_INVALID_OPERATION;
        return;
    }

    if (numParts <= 0) {
        error = KM_INVALID_VALUE;
        return;
    }

    //If the model previously had some data, free it
    if (m->_parts) {
        int i = 0;
        for(i = 0; i < m->_numParts; ++i) {
            freePart(&m->_parts[i]);
        }
        free(m->_parts);
        m->_parts = NULL;
    }

    m->_numParts = numParts;
    m->_parts = (kmModelPart*) malloc (sizeof(kmModelPart) * numParts);

    for (i = 0; i < numParts; ++i) {
        m->_parts[i]._numAnimations = 0;
        m->_parts[i]._numMeshes = 0;
        m->_parts[i]._numFrames = 0;
        m->_parts[i]._numTags = 0;

//        m->_parts[i]._animations = NULL;
        m->_parts[i]._meshes = NULL;
        m->_parts[i]._tags = NULL;
    }
}

///Create a part, specify the number of meshes it will have, the number of key frames, number of animations, and number of tags (if any)
void kmPart(KMuint partNumber, KMuint numMeshes, KMuint numFrames, KMuint numAnimations, KMuint numTags, kmTag* tags) {
    int i = 0;
    kmModel* m = getBoundModel();

    if (!m) {
        error = KM_INVALID_OPERATION;
        return;
    }

    if (numMeshes < 1 || numFrames < 1 || numAnimations < 0 || numTags < 0 || partNumber >= m->_numParts || partNumber < 0) {
        error = KM_INVALID_VALUE;
        return;
    }

    kmModelPart* p = &m->_parts[partNumber];

    if (p->_numMeshes != 0) {
        freePart(p);
    }

    p->_numFrames = numFrames;
    p->_currentFrame = p->_nextFrame = 0;

    //TODO: check memory is allocated OK

    //Allocate space for the meshes
    p->_meshes = (kmModelMesh*) malloc (sizeof(kmModelMesh) * numMeshes);
    p->_numMeshes = numMeshes;

    for (i = 0; i < p->_numMeshes; ++i) {
        p->_meshes[i]._indices = NULL;
        p->_meshes[i]._vertices = NULL;
        p->_meshes[i]._textureCoordinates = NULL;
        p->_meshes[i]._vertexNormals = NULL;

        p->_meshes[i]._numIndices = 0;
        p->_meshes[i]._numVertices = 0;
    }

    p->_animationInsertionPoint = 0; //Next specified animation (kmPartAnimation) belongs at pos 0
    p->_frameInterpolation = 0.0f;

    p->_numAnimations = numAnimations;
    p->_currentAnimation = NULL;
    /*if (numAnimations) {
        //Allocate space for the animations
        p->_animations = (kmAnimation*) malloc (sizeof(kmAnimation) * numAnimations);
    }*/

    p->_numTags = numTags;
    if (numTags) {
        //Allocate space for the tags
        p->_tags = (kmTag*) malloc (sizeof(kmTag) * numTags * numFrames);
        memcpy(p->_tags, tags, sizeof(kmTag) * numTags * numFrames);
    }

}

void kmLinkModel() {
    int i = 0, l = 0, j = 0, m = 0;
    kmModel* bound = getBoundModel();
    if (!bound) {
        error = KM_INVALID_OPERATION;
        return;
    }

    //Go through all the submodels
    for (i = 0; i < bound->_numParts; ++i) {

        //For each submodel create an array of links, and set them to NULL
        bound->_parts[i]._linkedModels = (kmModelPart**) malloc(sizeof(kmModelPart*) * bound->_parts[i]._numTags);
        for (j = 0; j < bound->_parts[i]._numTags; ++j) {
            bound->_parts[i]._linkedModels[j] = NULL;
        }

        //Go through all submodels with a higher index than this one (prevents double links)
        for (j = i+1; j < bound->_numParts; ++j) {
            //Go through the tags in the current model
            for (l = 0; l < bound->_parts[i]._numTags; ++l) {
                int m = 0;

                //Now link the parts
                for (m = 0; m < bound->_parts[j]._numTags; ++m) {
                    kmTag* tag1 = &bound->_parts[i]._tags[l];
                    kmTag* tag2 = &bound->_parts[j]._tags[m];

                    if (strcmp(tag1->_name, tag2->_name) == 0) {
                        bound->_parts[i]._linkedModels[l] = &bound->_parts[j];
                    }
                }
            }
        }
    }
}

void kmPartMesh(KMuint partNumber, KMuint meshNumber, KMuint numVertices, KMuint numTriangles, KMuint* indices) {
    kmModel* m = getBoundModel();

    if (!m) {
        error = KM_INVALID_OPERATION;
        return;
    }

    if (numVertices <= 0 || numTriangles <= 0 || indices == NULL || partNumber < 0 || partNumber >= m->_numParts ||
        meshNumber < 0 || meshNumber > m->_parts[partNumber]._numMeshes) {
        error = KM_INVALID_VALUE;
        return;
    }

    kmModelPart* p = &m->_parts[partNumber];
    kmModelMesh* mesh = &m->_parts[partNumber]._meshes[meshNumber];

    if (mesh->_numVertices) {
        freeMesh(mesh);
    }

    mesh->_numVertices = numVertices;
    mesh->_vertices = (kmVec3*) malloc (sizeof(kmVec3) * numVertices * p->_numFrames);

    mesh->_numIndices = numTriangles * 3;
    mesh->_indices = (KMuint*) malloc (sizeof(KMuint) * mesh->_numIndices);
    memcpy(mesh->_indices, indices, sizeof(KMuint) * mesh->_numIndices);
}

/// Load a single keyframe into the specified mesh
void kmPartMeshFrame(KMuint partNumber, KMuint meshNumber, KMuint frameNumber, kmVec3* vertices, kmVec2* textureCoordinates, kmVec3* vertexNormals) {
    kmVec3* insertionPtr = NULL;
    kmModel* m = getBoundModel();

    if (!m) {
        error = KM_INVALID_OPERATION;
        return;
    }

    if (partNumber < 0 || partNumber >= m->_numParts || meshNumber < 0 || meshNumber >= m->_parts[partNumber]._numMeshes ||
        frameNumber < 0 || frameNumber >= m->_parts[partNumber]._numFrames || vertices == NULL) {
        error = KM_INVALID_VALUE;
        return;
    }

    kmModelPart* p = &m->_parts[partNumber];
    kmModelMesh* mesh = &p->_meshes[meshNumber];

    //Find the right place to insert the vertex data
    insertionPtr = &mesh->_vertices[frameNumber * mesh->_numVertices];

    //Copy the vertices
    memcpy(insertionPtr, vertices, sizeof(kmVec3) * mesh->_numVertices);
}

KMint kmGetModelAttri(KMenum what) {
    kmModel* bound = getBoundModel();

    if (!bound) {
        error = KM_INVALID_OPERATION;
        return -1;
    }

    if (what == KM_NUM_MODEL_PARTS) {
        return bound->_numParts;
    }

    error = KM_INVALID_VALUE;
    return -1;
}

KMint kmGetModelPartAttri(KMint part, KMenum what) {
    kmModel* bound = getBoundModel();
    if (!bound) {
        error = KM_INVALID_OPERATION;
        return -1;
    }

    if (part < 0 || part >= bound->_numParts) {
        error = KM_INVALID_VALUE;
        return -1;
    }

    if (what == KM_NUM_MODEL_PART_FRAMES) {
        return bound->_parts[part]._numFrames;
    }

    if (what == KM_NUM_MODEL_PART_MESHES) {
        return bound->_parts[part]._numMeshes;
    }

    if (what == KM_NUM_MODEL_PART_TAGS) {
        return bound->_parts[part]._numTags;
    }

    if (what == KM_NUM_MODEL_PART_ANIMATIONS) {
        return bound->_parts[part]._numAnimations;
    }

    error = KM_INVALID_VALUE;
    return -1;
}

KMint kmGetModelPartMeshAttri(KMint part, KMint mesh, KMenum what) {
    kmModel* bound = getBoundModel();
    if (!bound) {
        error = KM_INVALID_OPERATION;
        return -1;
    }

    if (part < 0 || part >= bound->_numParts) {
        error = KM_INVALID_VALUE;
        return -1;
    }

    if (mesh < 0 || mesh >= bound->_parts[part]._numMeshes) {
        error = KM_INVALID_VALUE;
        return -1;
    }


}

KMint kmGetModelPartAnimationAttri(KMint part, KMbyte* animation, KMenum what) {
    kmModel* bound = getBoundModel();
    int i = 0;
    if (!bound) {
        error = KM_INVALID_OPERATION;
        return -1;
    }

    if (part < 0 || part >= bound->_numParts) {
        error = KM_INVALID_VALUE;
        return -1;
    }

    for (i = 0; i < bound->_parts[part]._numAnimations; ++i) {
        if (!strcmp(bound->_parts[part]._animations[i]._name, animation)) {
            switch(what) {
                case KM_MODEL_PART_ANIMATION_START_FRAME:
                {
                    return bound->_parts[part]._animations[i]._startFrame;
                }
                break;
                case KM_MODEL_PART_ANIMATION_END_FRAME:
                {
                    return bound->_parts[part]._animations[i]._endFrame;
                }
                break;
                default:
                    error = KM_INVALID_VALUE;
                break;
            }
        }
    }

    return -1;
}

void kmPartAnimation(KMuint part, const char* name, KMint start, KMint end, KMint looping, KMint fps) {
    kmModel* bound = getBoundModel();
    kmModelPart* p = NULL;

    if (!bound) {
        error = KM_INVALID_OPERATION;
        return;
    }

    if (part < 0 || part >= bound->_numParts) {
        error = KM_INVALID_VALUE;
        return;
    }

    p = &bound->_parts[part];

    p->_animations[p->_numAnimations]._startFrame = start;
    p->_animations[p->_numAnimations]._endFrame = end;
    p->_animations[p->_numAnimations]._fps = fps;
    p->_animations[p->_numAnimations]._loopingFrames = looping;
    memcpy(p->_animations[p->_numAnimations]._name, name, 64);
    p->_numAnimations++;

}
