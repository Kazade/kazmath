#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <kazmath/vec3.h>
#include <kazmath/vec2.h>
#include <kazmath/mat3.h>

#ifndef KM_FALSE
#define KM_FALSE 0
#endif

#ifndef KM_TRUE
#define KM_TRUE 1
#endif

#define KM_NO_ERROR 0x0
#define KM_INVALID_ENUM 0x0500
#define KM_INVALID_VALUE 0x0501
#define KM_INVALID_OPERATION 0x502

#define KM_NUM_MODEL_PARTS 0x0100
#define KM_NUM_MODEL_PART_FRAMES 0x0101
#define KM_NUM_MODEL_PART_ANIMATIONS 0x0102
#define KM_NUM_MODEL_PART_MESHES 0x0103
#define KM_NUM_MODEL_PART_TAGS 0x0104
#define KM_NUM_MODEL_PART_MESH_VERTICES 0x0105
#define KM_NUM_MODEL_PART_MESH_INDICES 0x0106
#define KM_MODEL_PART_ANIMATION_START_FRAME 0x0107
#define KM_MODEL_PART_ANIMATION_END_FRAME 0x0108

typedef unsigned int KMenum;
typedef unsigned int KMuint;
typedef signed int KMint;
typedef unsigned char KMboolean;
typedef signed char KMbyte;
typedef unsigned char KMubyte;
typedef float KMfloat;

#define KM_MAX_MESHES 32
#define KM_MAX_LINKED_MODELS 5
#define KM_MAX_PART_ANIMATIONS 32

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kmTag {
    kmVec3 _origin;
    kmMat3 _axis;
    KMbyte _name[64];
} kmTag;


typedef struct kmModelMesh {
    KMuint _numVertices;
    KMuint _numIndices;
    kmVec3* _vertices; ///!< The vertices that make up this keyframe
    kmVec3* _vertexNormals; ///<! Each vertex in the keyframe has a normal
    KMuint* _indices; ///< Triangle indices
    kmVec2* _textureCoordinates; ///< One set of texture coordinates for the mesh
} kmModelMesh;

/**
 * Stores the details of an animation so we
 * can switch animation using just a name
 */
typedef struct kmAnimation {
    KMuint _startFrame; //Start frame of this animation
    KMuint _endFrame; //End frame of this animation
    KMuint _loopingFrames; //The last n frames are looped once the end of the animation is reached
    KMfloat _fps;
    KMbyte _name[255]; //The name of this animation
} kmAnimation;

/**
 * A "part" of the model. MD2s only have 1 of these.. whereas
 * MD3s normally have 3 (head, torso, legs)
 */
typedef struct kmModelPart {
    KMuint _numAnimations; ///< The animation count
    KMuint _numMeshes; ///< Number of meshes
    KMuint _numFrames; ///< Number of frames
    KMuint _numTags; ///< Number of tags

    KMuint _animationInsertionPoint; ///< Internal. Used to find where to insert an animation into the array

    KMuint _currentFrame;
    KMuint _nextFrame;
    KMfloat _frameInterpolation;

    kmAnimation _animations[KM_MAX_PART_ANIMATIONS]; ///< Array of animations
    kmAnimation* _currentAnimation; ///< The current animation

    kmModelMesh* _meshes;
    kmTag* _tags;
    struct kmModelPart** _linkedModels;
} kmModelPart;

/** kmModel encompasses all the data associated with a model */
typedef struct kmModel {
    KMuint _modelID; ///< Unique ID
    kmModelPart* _parts; ///< Array of "parts"
    KMuint _numParts; ///< The number of parts (usually one)
} kmModel;

kmModel* getBoundModel(void);

void kmGenModels(KMuint n, KMuint* models);
void kmBindModel(KMuint n);

///Create a model made up of 1 or more parts (MD2 = 1, MD3 = 3 (or 4) etc.
void kmInitModel(KMint numParts);

///Create a part, specify the number of meshes it will have, the number of key frames, number of animations, and number of tags (if any)
void kmPart(KMuint partNumber, KMuint numMeshes, KMuint numFrames, KMuint numAnimations, KMuint numTags, kmTag* tags);

///Create a mesh
void kmPartMesh(KMuint partNumber, KMuint meshNumber, KMuint numVertices, KMuint numTriangles, KMuint* indices);

void kmPartAnimation(KMuint partNumber, const char* name, KMint start, KMint end, KMint looping, KMint fps);

/// Load a single keyframe into the specified mesh
void kmPartMeshFrame(KMuint partNumber, KMuint meshNumber, KMuint frameNumber,
                     kmVec3* vertices, kmVec2* textureCoordinates, kmVec3* vertexNormals);

KMboolean kmLoadMD3ModelFromFile(const KMbyte* filename);
void kmUpdateModel(KMfloat dT);
void kmRenderModel();
void kmLinkModel();
void kmDeleteModels(KMuint n, KMuint* models);
KMenum kmGetError(void);

void kmSetPartFrame(KMint part, KMint i);
void kmSetPartAnimation(unsigned part, const char* name);

KMint kmGetModelAttri(KMenum what);
KMint kmGetModelPartAttri(KMint part, KMenum what);
KMint kmGetModelPartAnimationAttri(KMint part, KMbyte* animation, KMenum what);
KMint kmGetModelPartMeshAttri(KMint part, KMint frame, KMenum what);

#ifdef __cplusplus
}
#endif
#endif // MODEL_H_INCLUDED
