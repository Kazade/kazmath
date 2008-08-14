#ifndef VEC2_H_INCLUDED
#define VEC2_H_INCLUDED

struct kmMat4;

#ifndef kmScalar
#define kmScalar float
#endif

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
typedef struct kmVec2 {
    kmScalar x;
    kmScalar y;
} kmVec2;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif
kmScalar kmVec2Length(const kmVec2* pIn); ///< Returns the length of the vector
kmScalar kmVec2LengthSq(const kmVec2* pIn); ///< Returns the square of the length of the vector
kmVec2* kmVec2Normalize(kmVec2* pOut, const kmVec2* pIn); ///< Returns the vector passed in set to unit length
kmVec2* kmVec2Add(kmVec2* pOut, const kmVec2* pV1, const kmVec2* pV2); ///< Adds 2 vectors and returns the result
kmScalar kmVec2Dot(const kmVec2* pV1, const kmVec2* pV2); /** Returns the Dot product which is the cosine of the angle between the two vectors multiplied by their lengths */
kmVec2* kmVec2Subtract(kmVec2* pOut, const kmVec2* pV1, const kmVec2* pV2); ///< Subtracts 2 vectors and returns the result
kmVec2* kmVec2Transform(kmVec2* pOut, const kmVec2* pV1, const struct kmMat4* pM); /** Transform the Vector */
kmVec2* kmVec2TransformCoord(kmVec2* pOut, const kmVec2* pV, const struct kmMat4* pM); ///<Transforms a 3D vector by a given matrix, projecting the result back into w = 1.
kmVec2* kmVec2Scale(kmVec2* pOut, const kmVec2* pIn, const kmScalar s); ///< Scales a vector to length s
bool	kmVec2AreEqual(const kmVec2* p1, const kmVec2* p2); ///< Returns true if both vectors are equal

#ifdef __cplusplus
}
#endif


#endif // VEC2_H_INCLUDED
