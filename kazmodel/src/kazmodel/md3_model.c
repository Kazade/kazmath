#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#include "md3_model.h"

KMboolean loadAnimationFile(const char* filename);

static int readSurface(MD3Surface* s, FILE* fileIn) {
    long surfaceOffset = ftell(fileIn);

    int j = 0;

    fread(&s->_ident, sizeof(int), 1, fileIn);
    fread(s->_name, sizeof(char), 64, fileIn);
    fread(&s->_flags, sizeof(int), 1, fileIn);
    fread(&s->_numFrames, sizeof(int), 1, fileIn);
    fread(&s->_numShaders, sizeof(int), 1, fileIn);
    fread(&s->_numVerts, sizeof(int), 1, fileIn);
    fread(&s->_numTriangles, sizeof(int), 1, fileIn);
    fread(&s->_triangleOffset, sizeof(int), 1, fileIn);
    fread(&s->_shaderOffset, sizeof(int), 1, fileIn);
    fread(&s->_stOffset, sizeof(int), 1, fileIn);
    fread(&s->_vertOffset, sizeof(int), 1, fileIn);
    fread(&s->_endOffset, sizeof(int), 1, fileIn);

    fseek(fileIn, s->_shaderOffset + surfaceOffset, SEEK_SET);
    for (j = 0; j < s->_numShaders; ++j) {
        fread(s->_shaders[j]._name, sizeof(char), 64, fileIn);
        fread(&s->_shaders[j]._index, sizeof(int), 1, fileIn);
    }

    fseek(fileIn, s->_triangleOffset + surfaceOffset, SEEK_SET);
    fread(s->_triangles, sizeof(MD3Triangle), s->_numTriangles, fileIn);
    /*for (j = 0; j < s->_numTriangles; ++j) {
        fread(&s->_triangles[j]._indexes, sizeof(int), 3, fileIn);
    }*/

    fseek(fileIn, s->_stOffset + surfaceOffset, SEEK_SET);
    fread(s->_texCoords, sizeof(MD3TexCoord), s->_numVerts, fileIn);
    /*for (j = 0; j < s->_numVerts; ++j) {
        fread(&s->_texCoords[j].st, sizeof(float), 2, fileIn);
    }*/

    fprintf(stdout, "\nLoading frame vertex data\n");
    fseek(fileIn, s->_vertOffset + surfaceOffset, SEEK_SET);
    s->_vertices = (MD3Vertex*) malloc (sizeof(MD3Vertex) * s->_numVerts * s->_numFrames);
    fread(s->_vertices, sizeof(MD3Vertex), s->_numVerts * s->_numFrames, fileIn);
    fprintf(stdout, "Done.\n");

    fseek(fileIn, s->_endOffset + surfaceOffset, SEEK_SET);

    return KM_TRUE;
}

static int loadMD3ModelPart(MD3Model* part, const char* filename) {
    FILE* fileIn = fopen(filename, "rb");
    int i = 0;

    if (!fileIn) {
        fprintf(stderr, "Could not load model part %s\n", filename);
        return 0;
    }

    fread(part->_header._ident, sizeof(char), 4, fileIn);

    //Only compare 4 chars (no terminating char)
    if (strncmp(part->_header._ident, "IDP3", 4) != 0) {
        fprintf(stderr, "ERROR: The file is not a valid MD3 model\n");
        return 0;
    }

    fread(&part->_header._version, sizeof(int), 1, fileIn);
    fread(part->_header._name, sizeof(char), 64, fileIn);
    fread(&part->_header._flags, sizeof(int), 1, fileIn);
    fread(&part->_header._numFrames, sizeof(int), 1, fileIn);
    fread(&part->_header._numTags, sizeof(int), 1, fileIn);
    fread(&part->_header._numSurfaces, sizeof(int), 1, fileIn);
    fread(&part->_header._numSkins, sizeof(int), 1, fileIn);
    fread(&part->_header._frameOffset, sizeof(int), 1, fileIn);
    fread(&part->_header._tagOffset, sizeof(int), 1, fileIn);
    fread(&part->_header._surfaceOffset, sizeof(int), 1, fileIn);
    fread(&part->_header._eofOffset, sizeof(int), 1, fileIn);

    fseek(fileIn, part->_header._frameOffset, SEEK_SET); //Seek to the start of the frames
    //part->_frames = (MD3Frame*) malloc(sizeof(MD3Frame) * part->_header._numFrames);
    fprintf(stdout, "Loading frames ");
    for (i = 0; i < part->_header._numFrames; ++i) {
        fprintf(stdout, ".");
        //Read each frame
        fread(&part->_frames[i]._min, sizeof(float), 3, fileIn);
        fread(&part->_frames[i]._max, sizeof(float), 3, fileIn);
        fread(&part->_frames[i]._origin, sizeof(float), 3, fileIn);
        fread(&part->_frames[i]._radius, sizeof(float), 1, fileIn);
        fread(part->_frames[i]._name, sizeof(char), 16, fileIn);
    }
    fprintf(stdout, " Done.\n");

    fseek(fileIn, part->_header._tagOffset, SEEK_SET); //Seek to the start of the tags
    //part->_tags = (MD3Tag*) malloc (sizeof(MD3Tag) * part->_header._numTags);
    fprintf(stdout, "Loading tags ");
    fread(part->_tags, sizeof(MD3Tag), part->_header._numTags * part->_header._numFrames, fileIn);
    fprintf(stdout, " Done.\n");

    fseek(fileIn, part->_header._surfaceOffset, SEEK_SET); //Seek to the start of the surfaces
    fprintf(stdout, "Loading surfaces ");
    for (i = 0; i < part->_header._numSurfaces; ++i) {
        fprintf(stdout, ".");
        //Read each surface
        MD3Surface* s = &part->_surfaces[i];

        if (!readSurface(s, fileIn)) {
            return KM_FALSE;
        }
    }
    fprintf(stdout, " Done.\n");
    fclose(fileIn);

    return KM_TRUE;
}

/** INTERNAL: Converts a quake3 model to our internal format */
int convertTokmModel(MD3Model* head, MD3Model* upper, MD3Model* lower) {
    int i = 0, j = 0, k = 0, l = 0;

    kmInitModel(3); //Create a model with 3 parts

    MD3Model* models [] = { lower, upper, head };

    for (i = 0; i < 3; ++i) {
        MD3Model* currMod = models[i];

        //Create a place to store the tags for each frame
        kmTag* temp = (kmTag*) malloc (sizeof(kmTag) * currMod->_header._numTags * currMod->_header._numFrames);

        for (k = 0; k < currMod->_header._numTags * currMod->_header._numFrames; ++k) {
            //Copy the origin
            temp[k]._origin.x = currMod->_tags[k]._origin[0];
            temp[k]._origin.y = currMod->_tags[k]._origin[2];
            temp[k]._origin.z = currMod->_tags[k]._origin[1];
            //Copy the axis
            //kmMat3 tmp;
            //kmMat3Transpose(&tmp, &currentMD3->_tags[k]._axis);

            temp[k]._axis.mat[0] = currMod->_tags[k]._axis[0][0];
            temp[k]._axis.mat[1] = currMod->_tags[k]._axis[0][1];
            temp[k]._axis.mat[2] = currMod->_tags[k]._axis[0][2];

            temp[k]._axis.mat[3] = currMod->_tags[k]._axis[1][0];
            temp[k]._axis.mat[4] = currMod->_tags[k]._axis[1][1];
            temp[k]._axis.mat[5] = currMod->_tags[k]._axis[1][2];

            temp[k]._axis.mat[6] = currMod->_tags[k]._axis[2][0];
            temp[k]._axis.mat[7] = currMod->_tags[k]._axis[2][1];
            temp[k]._axis.mat[8] = currMod->_tags[k]._axis[2][2];

            memcpy(temp[k]._name, currMod->_tags[k]._name, sizeof(char) * 64);
            //kmMat3Assign(&part->_tags[k]._axis, &currentMD3->_tags[k]._axis);
        }

        //Initialize the part
        kmPart(i, models[i]->_header._numSurfaces, currMod->_header._numFrames, 0, models[i]->_header._numTags, temp);
        free(temp);
        temp = NULL;

        for (j = 0; j < currMod->_header._numSurfaces; ++j) {
            MD3Surface* currSurface = &currMod->_surfaces[j];
            int numVertices = currSurface->_numVerts;
            int numTriangles = currSurface->_numTriangles;

            //Create room for the indices (temp)
            KMuint* indices = (KMuint*) malloc (sizeof(KMuint) * numTriangles * 3);

            int idx = 0;
            for (k = 0; k < numTriangles; ++k) {
                indices[idx++] = currSurface->_triangles[k]._indexes[0];
                indices[idx++] = currSurface->_triangles[k]._indexes[1];
                indices[idx++] = currSurface->_triangles[k]._indexes[2];
            }

            //Create the mesh
            kmPartMesh(i, j, numVertices, numTriangles, indices);

            free(indices);

            for (k = 0; k < currMod->_header._numFrames; ++k) {

                MD3Vertex* verts = currSurface->_vertices + (currSurface->_numVerts * k);
                kmVec3* tmpVerts = (kmVec3*) malloc (sizeof(kmVec3) * currSurface->_numVerts);

                for (l = 0; l < currSurface->_numVerts; ++l) {
                    tmpVerts[l].x = ((float)verts[l].v[0]) / 64.0f;
                    tmpVerts[l].y = ((float)verts[l].v[2]) / 64.0f;
                    tmpVerts[l].z = ((float)verts[l].v[1]) / 64.0f;
                }

                //Update the frame with data
                kmPartMeshFrame(i, j, k, tmpVerts, NULL, NULL);
                free(tmpVerts);
            }

            free(currSurface->_vertices); //We no longer need the MD3 vertices
        }
    }



    /*int i = 0, j = 0, k = 0, l = 0;

    //Shortcut pointers
    MD3Model* sc [] = { lower, upper, head };
    kmModel* out = getBoundModel();

    //The MD3 model is made up of 3 parts
    out->_parts = (kmModelPart*) malloc (sizeof(kmModelPart) * 3);
    out->_numParts = 3;

    out->_parts[0]._currentFrame = 90;
    out->_parts[1]._currentFrame = 90;
    out->_parts[2]._currentFrame = 0;
    //Loop through each part and convert it
    for (i = 0; i < 3; ++i) {
        MD3Model* currentMD3 = sc[i];

        out->_parts[i]._numMeshes = currentMD3->_header._numSurfaces;

        //out->_parts[i]._currentFrame = 0;
        out->_parts[i]._nextFrame = out->_parts[i]._currentFrame;
        out->_parts[i]._frameInterpolation = 0.0f;
        out->_parts[i]._numFrames = currentMD3->_header._numFrames; //sc[i]->_surfaces[i]._numFrames;
        out->_parts[i]._numTags = currentMD3->_header._numTags;
        out->_parts[i]._linkedModels = NULL;
        out->_parts[i]._currentAnimation = NULL; //No animation

        //Go through all the meshes (surfaces)
        for (j = 0; j < out->_parts[i]._numMeshes; ++j) {
            kmModelMesh* mesh = &out->_parts[i]._meshes[j];

            //Set the vertex count
            mesh->_vertexCount = currentMD3->_surfaces[j]._numVerts;

            //Get the number of frames
            unsigned numKeyframes = currentMD3->_header._numFrames;
            unsigned numTriangles = currentMD3->_surfaces[j]._numTriangles;

            mesh->_indexCount = numTriangles * 3;
            mesh->_indices = (unsigned*) malloc (sizeof(unsigned) * mesh->_indexCount);
            l = 0;
            for (k = 0; k < numTriangles; ++k) {
                mesh->_indices[l++] = currentMD3->_surfaces[j]._triangles[k]._indexes[0];
                mesh->_indices[l++] = currentMD3->_surfaces[j]._triangles[k]._indexes[1];
                mesh->_indices[l++] = currentMD3->_surfaces[j]._triangles[k]._indexes[2];
            }

            //Allocate space for the frames
            mesh->_keyFrames = (kmModelKeyframe*) malloc (sizeof(kmModelKeyframe) * numKeyframes);
            for (k = 0; k < numKeyframes; ++k) {
                kmModelKeyframe* frame = &mesh->_keyFrames[k];
                //Create space for the vertices
                frame->_vertices = (kmVec3*) malloc (sizeof(kmVec3) * mesh->_vertexCount);

                //Copy the vertices
                for (l = (k * mesh->_vertexCount); l < (k * mesh->_vertexCount) + mesh->_vertexCount; ++l) {
                    MD3Vertex* currentVert = &currentMD3->_surfaces[j]._vertices[l];
                    int idx = l - (k * mesh->_vertexCount);
                    frame->_vertices[idx].x = ((float)currentVert->v[0]) / 64.0f;
                    frame->_vertices[idx].y = ((float)currentVert->v[1]) / 64.0f;
                    frame->_vertices[idx].z = ((float)currentVert->v[2]) / 64.0f;
                    //TODO: Copy normals
                }
            }
        }

        //Allocate space for the tags
        kmModelPart* part = &out->_parts[i];
        part->_tags = (kmTag*) malloc (sizeof(kmTag) * currentMD3->_header._numTags * currentMD3->_header._numFrames);
        part->_numTags = currentMD3->_header._numTags;

        for (k = 0; k < part->_numTags * part->_numFrames; ++k) {
            //Copy the origin
            part->_tags[k]._origin.x = currentMD3->_tags[k]._origin[0];
            part->_tags[k]._origin.y = currentMD3->_tags[k]._origin[1];
            part->_tags[k]._origin.z = currentMD3->_tags[k]._origin[2];
            //Copy the axis
            //kmMat3 tmp;
            //kmMat3Transpose(&tmp, &currentMD3->_tags[k]._axis);

            part->_tags[k]._axis.mat[0] = currentMD3->_tags[k]._axis[0][0];
            part->_tags[k]._axis.mat[1] = currentMD3->_tags[k]._axis[0][1];
            part->_tags[k]._axis.mat[2] = currentMD3->_tags[k]._axis[0][2];

            part->_tags[k]._axis.mat[3] = currentMD3->_tags[k]._axis[1][0];
            part->_tags[k]._axis.mat[4] = currentMD3->_tags[k]._axis[1][1];
            part->_tags[k]._axis.mat[5] = currentMD3->_tags[k]._axis[1][2];

            part->_tags[k]._axis.mat[6] = currentMD3->_tags[k]._axis[2][0];
            part->_tags[k]._axis.mat[7] = currentMD3->_tags[k]._axis[2][1];
            part->_tags[k]._axis.mat[8] = currentMD3->_tags[k]._axis[2][2];
            //kmMat3Assign(&part->_tags[k]._axis, &currentMD3->_tags[k]._axis);
        }

        //Allocate space for the linked model
        part->_linkedModels = (kmModelPart**) malloc (sizeof(kmModelPart*) * part->_numTags);

        for (k = 0; k < part->_numTags; ++k) {
            part->_linkedModels[k] = NULL; //Set this link to NULL
        }

        //Go through each model (lower, upper, head)
        for (k = 0; k < 3; k++) {
            MD3Model* compMD3 = sc[k];
            if (k == i) {
                continue; //We don't want to link a model to itself
            }

            //Go through the tags in this model
            for (l = 0; l < currentMD3->_header._numTags; ++l) {
                int m = 0;

                //Now link the models
                for (m = 0; m < compMD3->_header._numTags; ++m) {
                    MD3Tag* tag1 = &currentMD3->_tags[l];
                    MD3Tag* tag2 = &compMD3->_tags[m];

                    if (strcmp(tag1->_name, tag2->_name) == 0) {
                        part->_linkedModels[l] = &out->_parts[k];
                    }
                }
            }
        }

    }*/

    return KM_TRUE;
}

int loadMD3Model(kmModel* pModel, const char* filename) {
    MD3Model* head = NULL;
    MD3Model* upper = NULL;
    MD3Model* lower = NULL;

    const int MAX_PATH_LENGTH = 256;
    char head_path[MAX_PATH_LENGTH];
    char upper_path[MAX_PATH_LENGTH];
    char lower_path[MAX_PATH_LENGTH];
    char animation_path[MAX_PATH_LENGTH];

    char head_filename [] = "head.md3\0";
    char upper_filename [] = "upper.md3\0";
    char lower_filename [] = "lower.md3\0";
    char animation_filename [] = "animation.cfg\0";

    int stringLength = strlen(filename);
    int i = stringLength;
    while (i--) {
        if (filename[i] == PATH_SEPARATOR) {
            //i now equals the last separator
            break;
        }
    }

    i++; //Move the char after the last slash
    assert(i + 1 < MAX_PATH_LENGTH && "File path was too long");

    strncpy(head_path, filename, i);
    strncpy(upper_path, filename, i);
    strncpy(lower_path, filename, i);
    strncpy(animation_path, filename, i);

    head_path[i] = '\0';
    upper_path[i] = '\0';
    lower_path[i] = '\0';
    animation_path[i] = '\0';

    strcat(head_path, head_filename);
    strcat(upper_path, upper_filename);
    strcat(lower_path, lower_filename);
    strcat(animation_path, animation_filename);

    head = (MD3Model*) malloc (sizeof(MD3Model));
    if (!loadMD3ModelPart(head, head_path)) {
        free(head);
        head = NULL;
        return KM_FALSE;
    }

    fprintf(stdout, "Successfully loaded head.md3\n");

    upper = (MD3Model*) malloc (sizeof(MD3Model));
    if (!loadMD3ModelPart(upper, upper_path)) {
        free(upper);
        upper = NULL;
        return KM_FALSE;
    }

    fprintf(stdout, "Successfully loaded upper.md3\n");

    lower = (MD3Model*) malloc (sizeof(MD3Model));
    if (!loadMD3ModelPart(lower, lower_path)) {
        free(lower);
        lower = NULL;
        return KM_FALSE;
    }

    fprintf(stdout, "Successfully loaded lower.md3\n");

    if (!convertTokmModel(head, upper, lower)) {
        fprintf(stderr, "Could not convert the model to the internal format\n");
        return KM_FALSE;
    }

    loadAnimationFile(animation_path);

    //Free everything
    free(head);
    free(upper);
    free(lower);

    head = upper = lower = NULL;
    return KM_TRUE;
}

int firstNonWhitespace(char* line) {
    int i = 0;
    char c = -1;
    while(c != '\0' && c != '\n') {
        c = line[i++];
        if (c != ' ' || c != '\t') {
            break;
        }
    }
    return i-1;
}

int readLine(FILE* file, char* line, int bufferSize) {
    int i = 0;
    char c = '\0';
    do {
        c = fgetc(file);
        if (c == '\n') {
            break;
        }
        line[i] = c;
        ++i;
    } while(c != '\n' && i < 255 && c != EOF);

    line[i] = '\0';
	return i;
}

KMboolean loadAnimationFile(const char* filename) {
    //TODO: Read the config file, add each animation to the appropriate part
    FILE* fileIn = fopen(filename, "r");
    int legsStart = -1, torsoStart = -1;

    if (!fileIn) {
        fprintf(stderr, "Couldn't load the animation configuration from %s\n", filename);
        return KM_FALSE;
    }

    int i = 0;
    char line[255];

    while(!feof(fileIn)) {
        int start, count, looping, fps, part, end;
        char anim[64];

        //fgets(line, 255, fileIn); //Read oneline of text
        readLine(fileIn, line, 255);
        i = firstNonWhitespace(line);
        if (line[i] == '/' && line[i+1] == '/') {
            continue; //It's a comment
        }

        if (i == strlen(line)) {
            continue; //Line is empty
        }

        if (strstr(line, "sex") != 0) {
            continue; //We don't use the sex of the model
        }

        if (strstr(line, "headoffset") != 0) {
            //?
            continue;
        }

        //If we get here, then this is an animation line
        //This assumes there is a space between the // and the name of the animation
        //TODO: make this more robust
        if (sscanf(line, "%d %d %d %d %*s %s\n", &start, &count, &looping, &fps, anim) != 5) {
            continue;
        }
        end = start + count;
        fprintf(stdout, "Loaded animation: %s\n", anim);

        if (strstr(anim, "LEGS")) {
            //If this is a leg animation, we need to subtract the difference between the start
            //of the torso animation and the start of the leg animation
            if (legsStart == -1) {
                legsStart = start;
            }
            int diff = (legsStart - torsoStart);
            part = 0;
            start -= diff;
            end -= diff;
        } else if (strstr(anim, "TORSO")) {
            //If it's a torso animation, we set to 1
            if (torsoStart == -1) {
                torsoStart = start;
            }
            part = 1;
        } else {
            //Otherwise the animation is for the torso AND the legs (BOTH)
            part = -1;
        }

        if (part != -1) {
            kmPartAnimation(part, anim, start, end, looping, fps);
        } else {
            kmPartAnimation(0, anim, start, end, looping, fps);
            kmPartAnimation(1, anim, start, end, looping, fps);
        }
    }

    return KM_TRUE;
}
