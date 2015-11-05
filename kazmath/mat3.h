#ifndef HEADER_8E9D0ABA3C76B989
#define HEADER_8E9D0ABA3C76B989

/*
Copyright (c) 2008, Luke Benstead.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef MAT3_H_INCLUDED
#define MAT3_H_INCLUDED

#include "utility.h"

struct kmVec3;
struct kmQuaternion;
struct kmMat4;

typedef struct kmMat3{
	kmScalar mat[9];
} kmMat3;

#ifdef __cplusplus
extern "C" {
#endif

kmMat3* kmMat3Fill(kmMat3* pOut, const kmScalar* pMat);
kmMat3* kmMat3Adjugate(kmMat3* pOut, const kmMat3* pIn);
kmMat3* kmMat3Identity(kmMat3* pOut);
kmMat3* kmMat3Inverse(kmMat3* pOut, const kmMat3* pM);
kmBool kmMat3IsIdentity(const kmMat3* pIn);
kmMat3* kmMat3Transpose(kmMat3* pOut, const kmMat3* pIn);
kmScalar kmMat3Determinant(const kmMat3* pIn);
kmBool kmMat3AreEqual(const kmMat3* pMat1, const kmMat3* pMat2);

kmMat3* kmMat3AssignMat3(kmMat3* pOut, const kmMat3* pIn);

kmMat3* kmMat3MultiplyMat3(kmMat3* pOut, const kmMat3* lhs, const kmMat3* rhs);
kmMat3* kmMat3MultiplyScalar(kmMat3* pOut, const kmMat3* lhs, const kmScalar rhs);

kmMat3* kmMat3FromRotationX(kmMat3* pOut, const kmScalar radians);
kmMat3* kmMat3FromRotationY(kmMat3* pOut, const kmScalar radians);
kmMat3* kmMat3FromRotationZ(kmMat3* pOut, const kmScalar radians);
kmMat3* kmMat3FromRotationXInDegrees(kmMat3* pOut, const kmScalar degrees);
kmMat3* kmMat3FromRotationYInDegrees(kmMat3* pOut, const kmScalar degrees);
kmMat3* kmMat3FromRotationZInDegrees(kmMat3* pOut, const kmScalar degrees);
kmMat3* kmMat3FromRotationQuaternion(kmMat3* pOut, const struct kmQuaternion* quaternion);
kmMat3* kmMat3FromRotationLookAt(kmMat3* pOut, const struct kmVec3* pEye, const struct kmVec3* pCentre, const struct kmVec3* pUp);
kmMat3* kmMat3FromScaling(kmMat3* pOut, const kmScalar x, const kmScalar y);
kmMat3* kmMat3FromTranslation(kmMat3* pOut, const kmScalar x, const kmScalar y);
kmMat3* kmMat3FromRotationAxisAngle(kmMat3* pOut, const struct kmVec3* axis, const kmScalar radians);
kmMat3* kmMat3FromRotationAxisAngleInDegrees(kmMat3* pOut, const struct kmVec3* axis, const kmScalar degrees);

void kmMat3ExtractRotationAxisAngle(const kmMat3* self, struct kmVec3* axis, kmScalar* radians);
void kmMat3ExtractRotationAxisAngleInDegrees(const kmMat3* self, struct kmVec3* axis, kmScalar* degrees);

struct kmVec3* kmMat3ExtractUpVec3(const kmMat3* self, struct kmVec3* pOut);
struct kmVec3* kmMat3ExtractRightVec3(const kmMat3* self, struct kmVec3* pOut);
struct kmVec3* kmMat3ExtractForwardVec3(const kmMat3* self, struct kmVec3* pOut);

#ifdef __cplusplus
}
#endif
#endif /* MAT3_H_INCLUDED */


#endif /* header guard */
