#ifndef MD3_MODEL_H_INCLUDED
#define MD3_MODEL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <kazmath/vec3.h>
#include <kazmath/mat3.h>

#include "model.h"

#define MD3_MAX_SHADERS 256
#define MD3_MAX_VERTS 4096
#define MD3_MAX_TRIANGLES 8192
#define MD3_MAX_SURFACES 32
#define MD3_MAX_TAGS 16
#define MD3_MAX_FRAMES 1024

typedef struct tagMD3Shader {
    char _name[64];
    int _index;
} MD3Shader;

typedef struct tagMD3Triangle {
    int _indexes[3];
} MD3Triangle;

typedef struct tagMD3TexCoord {
    kmVec2 st;
} MD3TexCoord;

typedef struct tagMD3Vertex {
    signed short v[3]; //Compressed vertex position
    unsigned char _normal; //Compressed normal
} MD3Vertex;

typedef struct tagMD3Frame {
    kmVec3 _min;
    kmVec3 _max;
    kmVec3 _origin;

    float _radius;
    char _name[16];
} MD3Frame;

typedef struct tagMD3Tag {
    char _name[64];
    float _origin[3];
    float _axis[3][3];
} MD3Tag;

typedef struct tagMD3Surface {
    int _ident;
    char _name[64];
    int _flags;
    int _numFrames;
    int _numShaders;
    int _numVerts;
    int _numTriangles;

    int _triangleOffset;
    int _shaderOffset;
    int _stOffset;
    int _vertOffset;
    int _endOffset;

    MD3Shader _shaders[MD3_MAX_SHADERS];
    MD3Triangle _triangles[MD3_MAX_TRIANGLES];
    MD3Vertex* _vertices;
    MD3TexCoord _texCoords[MD3_MAX_VERTS];
} MD3Surface;


typedef struct tagMD3Header {
    char _ident[4];
    int _version;
    char _name[64];

    int _flags;
    int _numFrames;
    int _numTags;
    int _numSurfaces;
    int _numSkins;
    int _frameOffset;
    int _tagOffset;
    int _surfaceOffset;
    int _eofOffset;
} MD3Header;

typedef struct tagMD3Model {
    MD3Header _header;
    MD3Frame _frames[MD3_MAX_FRAMES];
    MD3Tag _tags[MD3_MAX_TAGS * MD3_MAX_FRAMES];
    MD3Surface _surfaces[MD3_MAX_SURFACES];
} MD3Model;

int loadMD3Model(kmModel* pModel, const char* filename);

#ifdef __cplusplus
}
#endif

#endif // MD3_MODEL_H_INCLUDED
