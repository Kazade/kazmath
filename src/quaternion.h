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

#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include "utility.h"
#include "mat4.h"

struct kmVec3;
struct kmMat4;

typedef struct kmQuaternion {
	kmScalar x;
	kmScalar y;
	kmScalar z;
	kmScalar w;
} kmQuaternion;

kmQuaternion* kmQuaternionConjugate(kmQuaternion* pOut,
											const kmQuaternion* pIn); ///< Returns pOut, sets pOut to the conjugate of pIn



kmScalar 	kmQuaternionDot(const kmQuaternion* q1,
											const kmQuaternion* q2); ///< Returns the dot product of the 2 quaternions



kmQuaternion* kmQuaternionExp(kmQuaternion* pOut, const kmQuaternion* pIn); ///< Returns the exponential of the quaternion

///< Makes the passed quaternion an identity quaternion

kmQuaternion* kmQuaternionIdentity(kmQuaternion* pOut);

///< Returns the inverse of the passed Quaternion

kmQuaternion* kmQuaternionInverse(kmQuaternion* pOut,
											const kmQuaternion* pIn);

///< Returns true if the quaternion is an identity quaternion

bool kmQuaternionIsIdentity(const kmQuaternion* pIn);

///< Returns the length of the quaternion

kmScalar kmQuaternionLength(const kmQuaternion* pIn);

///< Returns the length of the quaternion squared (prevents a sqrt)

kmScalar kmQuaternionLengthSq(const kmQuaternion* pIn);

///< Returns the natural logarithm

kmQuaternion* kmQuaternionLn(kmQuaternion* pOut, const kmQuaternion* pIn);

///< Multiplies 2 quaternions together

kmQuaternion* kmQuaternionMultiply(kmQuaternion* pOut,
								 const kmQuaternion* q1,
								 const kmQuaternion* q2);

///< Normalizes a quaternion

kmQuaternion* kmQuaternionNormalize(kmQuaternion* pOut,
											const kmQuaternion* pIn);

///< Rotates a quaternion around an axis

kmQuaternion* kmQuaternionRotationAxis(kmQuaternion* pOut,
									const kmVec3* pV,
									kmScalar angle);

///< Creates a quaternion from a rotation matrix

kmQuaternion* kmQuaternionRotationMatrix(kmQuaternion* pOut,
										const kmMat4* pIn);

///< Create a quaternion from yaw, pitch and roll

kmQuaternion* kmQuaternionRotationYawPitchRoll(kmQuaternion* pOut,
												kmScalar yaw,
												kmScalar pitch,
												kmScalar roll);
///< Interpolate between 2 quaternions

kmQuaternion* kmQuaternionSlerp(kmQuaternion* pOut,
								const kmQuaternion* q1,
								const kmQuaternion* q2,
								kmScalar t);

///< Get the axis and angle of rotation from a quaternion

void kmQuaternionToAxisAngle(const kmQuaternion* pIn,
								kmVec3* pVector,
								kmScalar* pAngle);

///< Scale a quaternion

kmQuaternion* kmQuaternionScale(kmQuaternion* pOut,
										const kmQuaternion* pIn,
										kmScalar s);


kmQuaternion* kmQuaternionAssign(kmQuaternion* pOut, const kmQuaternion* pIn);
kmQuaternion* kmQuaternionAdd(kmQuaternion* pOut, const kmQuaternion* pQ1, const kmQuaternion* pQ2);

kmQuaternion* kmQuaternionRotationBetweenVec3(kmQuaternion* pOut, const kmVec3* vec1, const kmVec3* vec2, const kmVec3* fallback);
kmVec3* kmQuaternionMultiplyVec3(kmVec3* pOut, const kmQuaternion* q, const kmVec3* v);

#ifdef __cplusplus
}
#endif

#endif
