/*
Copyright 2007 Luke Benstead

This file is part of KazMath.

KazMath is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

KazMath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser Public License for more details.

You should have received a copy of the GNU Lesser Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef MAT4_H_INCLUDED
#define MAT4_H_INCLUDED

#include "utility.h"

struct kmVec3;
struct kmQuaternion;

typedef struct kmMat4{
	kmScalar m_Mat[16];
} kmMat4;

#ifdef __cplusplus
extern "C" {
#endif

kmMat4* kmMat4Identity(kmMat4* pOut);
kmMat4* kmMat4Inverse(kmMat4* pOut, const kmMat4* pM);
bool  kmMat4IsIdentity(const kmMat4* pIn);
kmMat4* kmMat4Transpose(kmMat4* pOut, const kmMat4* pIn);
kmMat4* kmMat4Multiply(kmMat4* pOut, const kmMat4* pM1, const kmMat4* pM2);
kmMat4* kmMat4MultiplyTranspose(kmMat4* pOut, const kmMat4* pM1, const kmMat4* pM2);

kmMat4* kmMat4Assign(kmMat4* pOut, const kmMat4* pIn);
bool  kmMat4AreEqual(const kmMat4* pM1, const kmMat4* pM2);

kmMat4* kmMat4RotationAxis(kmMat4* pOut, const struct kmVec3* axis, kmScalar radians);
kmMat4* kmMat4RotationX(kmMat4* pOut, const float radians);
kmMat4* kmMat4RotationY(kmMat4* pOut, const float radians);
kmMat4* kmMat4RotationZ(kmMat4* pOut, const float radians);
kmMat4* kmMat4RotationPitchYawRoll(kmMat4* pOut, const kmScalar pitch, const kmScalar yaw, const kmScalar roll);
kmMat4* kmMat4RotationQuaternion(kmMat4* pOut, const struct kmQuaternion* pQ);
kmMat4* kmMat4Scaling(kmMat4* pOut, const kmScalar x, const kmScalar y, const kmScalar z);
kmMat4* kmMat4Translation(kmMat4* pOut, const kmScalar x, const kmScalar y, const kmScalar z);

struct kmVec3* kmMat4GetUpVec3(struct kmVec3* pOut, const kmMat4* pIn);
struct kmVec3* kmMat4GetRightVec3(struct kmVec3* pOut, const kmMat4* pIn);
struct kmVec3* kmMat4GetForwardVec3(struct kmVec3* pOut, const kmMat4* pIn);

#ifdef __cplusplus
}
#endif
#endif // MAT4_H_INCLUDED
