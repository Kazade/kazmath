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

#include <assert.h>
#include <memory.h>

#include "utility.h"
#include "mat4.h"
#include "vec3.h"
#include "quaternion.h"

///< Returns pOut, sets pOut to the conjugate of pIn
kmQuaternion* kmQuaternionConjugate(kmQuaternion* pOut, const kmQuaternion* pIn)
{
	pOut->x = -pIn->x;
	pOut->y = -pIn->y;
	pOut->z = -pIn->z;
	pOut->w = pIn->w;

	return pOut;
}

///< Returns the dot product of the 2 quaternions
kmScalar kmQuaternionDot(const kmQuaternion* q1, const kmQuaternion* q2)
{
	// A dot B = B dot A = AtBt + AxBx + AyBy + AzBz

	return (q1->w * q2->w +
			q1->x * q2->x +
			q1->y * q2->y +
			q1->z * q2->z);
}

///< Returns the exponential of the quaternion
kmQuaternion* kmQuaternionExp(kmQuaternion* pOut, const kmQuaternion* pIn)
{
	assert(0);

	return pOut;
}

///< Makes the passed quaternion an identity quaternion
kmQuaternion* kmQuaternionIdentity(kmQuaternion* pOut)
{
	pOut->x = 0.0;
	pOut->y = 0.0;
	pOut->z = 0.0;
	pOut->w = 1.0;

	return pOut;
}

///< Returns the inverse of the passed Quaternion
kmQuaternion* kmQuaternionInverse(kmQuaternion* pOut,
											const kmQuaternion* pIn)
{
	kmScalar l = kmQuaternionLength(pIn);

	if (fabs(l) > kmEpsilon)
	{
		pOut->x = 0.0;
		pOut->y = 0.0;
		pOut->z = 0.0;
		pOut->w = 0.0;

		return pOut;
	}

	kmQuaternion* tmp = 0;

	///Get the conjugute and divide by the length
	kmQuaternionScale(pOut,
				kmQuaternionConjugate(tmp, pIn), 1.0 / l);

	return pOut;
}

///< Returns true if the quaternion is an identity quaternion
bool kmQuaternionIsIdentity(const kmQuaternion* pIn)
{
	return (pIn->x == 0.0 && pIn->y == 0.0 && pIn->z == 0.0 &&
				pIn->w == 1.0);
}

///< Returns the length of the quaternion
kmScalar kmQuaternionLength(const kmQuaternion* pIn)
{
	return sqrtf(kmQuaternionLengthSq(pIn));
}

///< Returns the length of the quaternion squared (prevents a sqrt)
kmScalar kmQuaternionLengthSq(const kmQuaternion* pIn)
{
	return pIn->x * pIn->x + pIn->y * pIn->y +
						pIn->z * pIn->z + pIn->w * pIn->w;
}

///< Returns the natural logarithm
kmQuaternion* kmQuaternionLn(kmQuaternion* pOut,
										const kmQuaternion* pIn)
{
	/*
		A unit quaternion, is defined by:
		Q == (cos(theta), sin(theta) * v) where |v| = 1
		The natural logarithm of Q is, ln(Q) = (0, theta * v)
	*/

	assert(0);

	return pOut;
}

///< Multiplies 2 quaternions together
extern
kmQuaternion* kmQuaternionMultiply(kmQuaternion* pOut,
								 const kmQuaternion* q1,
								 const kmQuaternion* q2)
{
	pOut->w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
	pOut->x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
	pOut->y = q1->w * q2->y + q1->y * q2->w + q1->z * q2->x - q1->x * q2->z;
	pOut->z = q1->w * q2->z + q1->z * q2->w + q1->x * q2->y - q1->y * q2->x;

	return pOut;
}

///< Normalizes a quaternion
kmQuaternion* kmQuaternionNormalize(kmQuaternion* pOut,
											const kmQuaternion* pIn)
{
	kmScalar length = kmQuaternionLength(pIn);
	assert(fabs(length) > kmEpsilon);
	kmQuaternionScale(pOut, pIn, 1.0f / length);

	return pOut;
}

///< Rotates a quaternion around an axis
kmQuaternion* kmQuaternionRotationAxis(kmQuaternion* pOut,
									const kmVec3* pV,
									kmScalar angle)
{
	kmScalar rad = angle * 0.5f;
	kmScalar scale	= sinf(rad);

	pOut->w = cosf(rad);
	pOut->x = pV->x * scale;
	pOut->y = pV->y * scale;
	pOut->z = pV->z * scale;

	return pOut;
}

///< Creates a quaternion from a rotation matrix
kmQuaternion* kmQuaternionRotationMatrix(kmQuaternion* pOut,
										const kmMat4* pIn)
{
/*
Note: The OpenGL matrices are transposed from the description below
taken from the Matrix and Quaternion FAQ

    if ( mat[0] > mat[5] && mat[0] > mat[10] )  {	// Column 0:
        S  = sqrt( 1.0 + mat[0] - mat[5] - mat[10] ) * 2;
        X = 0.25 * S;
        Y = (mat[4] + mat[1] ) / S;
        Z = (mat[2] + mat[8] ) / S;
        W = (mat[9] - mat[6] ) / S;
    } else if ( mat[5] > mat[10] ) {			// Column 1:
        S  = sqrt( 1.0 + mat[5] - mat[0] - mat[10] ) * 2;
        X = (mat[4] + mat[1] ) / S;
        Y = 0.25 * S;
        Z = (mat[9] + mat[6] ) / S;
        W = (mat[2] - mat[8] ) / S;
    } else {						// Column 2:
        S  = sqrt( 1.0 + mat[10] - mat[0] - mat[5] ) * 2;
        X = (mat[2] + mat[8] ) / S;
        Y = (mat[9] + mat[6] ) / S;
        Z = 0.25 * S;
        W = (mat[4] - mat[1] ) / S;
    }
*/

	kmScalar T = pIn->m_Mat[0] + pIn->m_Mat[5] + pIn->m_Mat[10];

	if (T > kmEpsilon) {
		//If the trace is greater than zero we always use this calculation:
		/*  S = sqrt(T) * 2;
		  X = ( mat[9] - mat[6] ) / S;
		  Y = ( mat[2] - mat[8] ) / S;
		  Z = ( mat[4] - mat[1] ) / S;
		  W = 0.25 * S;*/

		kmScalar s = sqrtf(T) * 2;
		pOut->x = (pIn->m_Mat[9] - pIn->m_Mat[6]) / s;
		pOut->y = (pIn->m_Mat[8] - pIn->m_Mat[2]) / s;
		pOut->z = (pIn->m_Mat[1] - pIn->m_Mat[4]) / s;
		pOut->w = 0.25 * s;

		kmQuaternionNormalize(pOut, pOut);
		return pOut;
	}

	//Otherwise the calculation depends on which major diagonal element has the greatest value.

	if (pIn->m_Mat[0] > pIn->m_Mat[5] && pIn->m_Mat[0] > pIn->m_Mat[10]) {
		kmScalar s = sqrtf(1 + pIn->m_Mat[0] - pIn->m_Mat[5] - pIn->m_Mat[10]) * 2;
		pOut->x = 0.25 * s;
		pOut->y = (pIn->m_Mat[1] + pIn->m_Mat[4]) / s;
		pOut->z = (pIn->m_Mat[8] + pIn->m_Mat[2]) / s;
		pOut->w = (pIn->m_Mat[9] - pIn->m_Mat[6]) / s;
	}
	else if (pIn->m_Mat[5] > pIn->m_Mat[10]) {
		kmScalar s = sqrtf(1 + pIn->m_Mat[5] - pIn->m_Mat[0] - pIn->m_Mat[10]) * 2;
		pOut->x = (pIn->m_Mat[1] + pIn->m_Mat[4]) / s;
		pOut->y = 0.25 * s;
		pOut->z = (pIn->m_Mat[9] + pIn->m_Mat[6]) / s;
		pOut->w = (pIn->m_Mat[8] - pIn->m_Mat[2]) / s;
	}
	else {
		kmScalar s = sqrt(1 + pIn->m_Mat[10] - pIn->m_Mat[0] - pIn->m_Mat[5]) * 2;
        pOut->x = (pIn->m_Mat[8] + pIn->m_Mat[2] ) / s;
        pOut->y = (pIn->m_Mat[6] + pIn->m_Mat[9] ) / s;
        pOut->z = 0.25 * s;
        pOut->w = (pIn->m_Mat[1] - pIn->m_Mat[4] ) / s;
	}

	kmQuaternionNormalize(pOut, pOut);
	return pOut;
}

///< Create a quaternion from yaw, pitch and roll
kmQuaternion* kmQuaternionRotationYawPitchRoll(kmQuaternion* pOut,
												kmScalar yaw,
												kmScalar pitch,
												kmScalar roll)
{
	kmScalar	ex, ey, ez;		// temp half euler angles
	kmScalar	cr, cp, cy, sr, sp, sy, cpcy, spsy;		// temp vars in roll,pitch yaw

	ex = kmDegreesToRadians(pitch) / 2.0;	// convert to rads and half them
	ey = kmDegreesToRadians(yaw) / 2.0;
	ez = kmDegreesToRadians(roll) / 2.0;

	cr = cosf(ex);
	cp = cosf(ey);
	cy = cosf(ez);

	sr = sinf(ex);
	sp = sinf(ey);
	sy = sinf(ez);

	cpcy = cp * cy;
	spsy = sp * sy;

	pOut->w = cr * cpcy + sr * spsy;

	pOut->x = sr * cpcy - cr * spsy;
	pOut->y = cr * sp * cy + sr * cp * sy;
	pOut->z = cr * cp * sy - sr * sp * cy;

	kmQuaternionNormalize(pOut, pOut);

	return pOut;
}

///< Interpolate between 2 quaternions
kmQuaternion* kmQuaternionSlerp(kmQuaternion* pOut,
								const kmQuaternion* q1,
								const kmQuaternion* q2,
								kmScalar t)
{

 /*float CosTheta = Q0.DotProd(Q1);
  float Theta = acosf(CosTheta);
  float SinTheta = sqrtf(1.0f-CosTheta*CosTheta);

  float Sin_T_Theta = sinf(T*Theta)/SinTheta;
  float Sin_OneMinusT_Theta = sinf((1.0f-T)*Theta)/SinTheta;

  Quaternion Result = Q0*Sin_OneMinusT_Theta;
  Result += (Q1*Sin_T_Theta);

  return Result;*/

	kmScalar ct = kmQuaternionDot(q1, q2);
	kmScalar theta = acosf(ct);
	kmScalar st = sqrtf(1.0 - kmSQR(ct));

	kmScalar stt = sinf(t * theta) / st;
	kmScalar somt = sinf((1.0 - t) * theta) / st;

	kmQuaternion temp, temp2;
	kmQuaternionScale(&temp, q1, somt);
	kmQuaternionScale(&temp2, q2, stt);
	kmQuaternionAdd(pOut, &temp, &temp2);

	return pOut;
}

///< Get the axis and angle of rotation from a quaternion
void kmQuaternionToAxisAngle(const kmQuaternion* pIn,
								kmVec3* pAxis,
								kmScalar* pAngle)
{
	kmScalar 	tempAngle;		// temp angle
	kmScalar	scale;			// temp vars

	tempAngle = acosf(pIn->w);
	scale = sqrtf(kmSQR(pIn->x) + kmSQR(pIn->y) + kmSQR(pIn->z));

	if (((scale > -kmEpsilon) && scale < kmEpsilon)
		|| (scale < 360.0f + kmEpsilon && scale > 360.0f - kmEpsilon))		// angle is 0 or 360 so just simply set axis to 0,0,1 with angle 0
	{
		*pAngle = 0.0f;

		pAxis->x = 0.0f;
		pAxis->y = 0.0f;
		pAxis->z = 1.0f;
	}
	else
	{
		*pAngle = tempAngle * 2.0;		// angle in radians

		pAxis->x = pIn->x / scale;
		pAxis->y = pIn->y / scale;
		pAxis->z = pIn->z / scale;
		kmVec3Normalize(pAxis, pAxis);
	}
}

kmQuaternion* kmQuaternionScale(kmQuaternion* pOut,
										const kmQuaternion* pIn,
										kmScalar s)
{
	pOut->x = pIn->x * s;
	pOut->y = pIn->y * s;
	pOut->z = pIn->z * s;
	pOut->w = pIn->w * s;

	return pOut;
}

kmQuaternion* kmQuaternionAssign(kmQuaternion* pOut, const kmQuaternion* pIn)
{
	memcpy(pOut, pIn, sizeof(float) * 4);

	return pOut;
}

kmQuaternion* kmQuaternionAdd(kmQuaternion* pOut, const kmQuaternion* pQ1, const kmQuaternion* pQ2)
{
	pOut->x = pQ1->x + pQ2->x;
	pOut->y = pQ1->y + pQ2->y;
	pOut->z = pQ1->z + pQ2->z;
	pOut->w = pQ1->w + pQ2->w;

	return pOut;
}

/** Adapted from the OGRE engine!

	Gets the shortest arc quaternion to rotate this vector to the destination
	vector.
@remarks
	If you call this with a dest vector that is close to the inverse
	of this vector, we will rotate 180 degrees around the 'fallbackAxis'
	(if specified, or a generated axis if not) since in this case
	ANY axis of rotation is valid.
*/

kmQuaternion* kmQuaternionRotationBetweenVec3(kmQuaternion* pOut, const kmVec3* vec1, const kmVec3* vec2, const kmVec3* fallback) {

	kmVec3 v1, v2;
	kmVec3Assign(&v1, vec1);
	kmVec3Assign(&v2, vec2);

	kmVec3Normalize(&v1, &v1);
	kmVec3Normalize(&v2, &v2);

	kmScalar a = kmVec3Dot(&v1, &v2);

	if (a >= 1.0) {
		kmQuaternionIdentity(pOut);
		return pOut;
	}

	if (a < (1e-6f - 1.0f))	{
		if (fabs(kmVec3LengthSq(fallback)) < kmEpsilon) {
			kmQuaternionRotationAxis(pOut, fallback, kmPI);
		} else {
			kmVec3 axis;
			kmVec3 X;
			X.x = 1.0;
			X.y = 0.0;
			X.z = 0.0;


			kmVec3Cross(&axis, &X, vec1);

			//If axis is zero
			if (fabs(kmVec3LengthSq(&axis)) < kmEpsilon) {
				kmVec3 Y;
				Y.x = 0.0;
				Y.y = 1.0;
				Y.z = 0.0;

				kmVec3Cross(&axis, &Y, vec1);
			}

			kmVec3Normalize(&axis, &axis);

			kmQuaternionRotationAxis(pOut, &axis, kmPI);
		}
	} else {
		kmScalar s = sqrtf((1+a) * 2);
		kmScalar invs = 1 / s;

		kmVec3 c;
		kmVec3Cross(&c, &v1, &v2);

		pOut->x = c.x * invs;
		pOut->y = c.y * invs;
        pOut->z = c.z * invs;
        pOut->w = s * 0.5;

		kmQuaternionNormalize(pOut, pOut);
	}

	return pOut;

}

kmVec3* kmQuaternionMultiplyVec3(kmVec3* pOut, const kmQuaternion* q, const kmVec3* v) {
	kmVec3 uv, uuv, qvec;

	qvec.x = q->x;
	qvec.y = q->y;
	qvec.z = q->z;

	kmVec3Cross(&uv, &qvec, v);
	kmVec3Cross(&uuv, &qvec, &uv);

	kmVec3Scale(&uv, &uv, (2.0 * q->w));
	kmVec3Scale(&uuv, &uuv, 2.0);

	kmVec3Add(pOut, v, &uv);
	kmVec3Add(pOut, pOut, &uuv);

	return pOut;
}

