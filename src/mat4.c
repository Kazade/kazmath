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
#include "utility.h"
#include "vec3.h"
#include "mat4.h"
#include "quaternion.h"

#include <memory.h>
#include <assert.h>

/** Sets pOut to an identity matrix returns pOut*/
kmMat4* kmMat4Identity(kmMat4* pOut)
{
	memset(pOut->m_Mat, 0, sizeof(float) * 16);
	pOut->m_Mat[0] = pOut->m_Mat[5] = pOut->m_Mat[10] = pOut->m_Mat[15] = 1.0f;
	return pOut;
}

kmMat4* kmMat4Inverse(kmMat4* pOut, const kmMat4* pM)
{
	float mat[16];

	for (int i = 0; i < 16; i++)
	{
		mat[i] = pM->m_Mat[i];
	}

	kmMat4Identity(pOut);


	for (int j = 0; j < 4; ++j) // Find largest pivot in column j among rows j..3
	{
		int i1 = j;		 // Row with largest pivot candidate

		for (int i = j + 1; i < 4; ++i)
		{
			if (fabs(mat[i*4 + j]) > fabs(mat[i1*4 + j]))
				i1 = i;
		}

		// Swap rows i1 and j in a and b to put pivot on diagonal
		float temp[4];
		for(int k = 0; k < 4; k++)
		{
		    temp[k] = mat[i1 * 4 + k];
		}

		for(int k = 0; k < 4; k++)
		{
		    mat[i1 * 4 + k] = mat[j * 4 + k];
		    mat[j * 4 + k] = temp[k];
		}

        for(int k = 0; k < 4; k++)
		{
		    temp[k] = pOut->m_Mat[i1 * 4 + k];
		}

		for(int k = 0; k < 4; k++)
		{
		    pOut->m_Mat[i1 * 4 + k] = pOut->m_Mat[j * 4 + k];
		    pOut->m_Mat[j * 4 + k] = temp[k];
		}

		// Scale row j to have a unit diagonal
		if (!mat[j*4 + j])
		{
			// Singular matrix - can't invert

			return pOut;
		}

        for(int k = 0; k < 4; k++)
		{
            pOut->m_Mat[j * 4 + k] /= mat[j * 4 + j];
            mat[j * 4 + k] /= mat[j * 4 + j];
		}

		// Eliminate off-diagonal elems in col j of a, doing identical ops to b
		for (int i = 0; i < 4; ++i)
		{
			if (i != j)
			{
				for(int k = 0; k < 4; k++)
				{
                    pOut->m_Mat[i*4 + k] -= mat[i*4 + j] * pOut->m_Mat[j*4 + k];
                    mat[i*4 + k] -= mat[i*4 + j] * mat[j*4 + k];
				}
			}
		}
	}

	return pOut;
}

/** Returns true if pIn is an identity matrix */
bool  kmMat4IsIdentity(const kmMat4* pIn)
{
	static const float identity [] = { 	1.0f, 0.0f, 0.0f, 0.0f,
	                                    0.0f, 1.0f, 0.0f, 0.0f,
	                                    0.0f, 0.0f, 1.0f, 0.0f,
	                                    0.0f, 0.0f, 0.0f, 1.0f
	                                 };

	return (memcmp(identity, pIn->m_Mat, sizeof(float) * 16) == 0);
}

/** Sets pOut to the transpose of pIn, returns pOut */
kmMat4* kmMat4Transpose(kmMat4* pOut, const kmMat4* pIn)
{
	for (int z = 0; z < 4; ++z)
		for (int x = 0; x < 4; ++x)
			pOut->m_Mat[(z * 4) + x] = pIn->m_Mat[(x * 4) + z];

	return pOut;
}

/* Multiplies pM1 with pM2, stores the result in pOut, returns pOut */
kmMat4* kmMat4Multiply(kmMat4* pOut, const kmMat4* pM1, const kmMat4* pM2)
{
	float mat[16];

	const float *m1 = pM1->m_Mat, *m2 = pM2->m_Mat;

	mat[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	mat[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	mat[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	mat[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

	mat[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	mat[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	mat[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	mat[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

	mat[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	mat[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	mat[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	mat[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

	mat[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	mat[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	mat[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	mat[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];


	memcpy(pOut->m_Mat, mat, sizeof(float)*16);

	return pOut;
}

kmMat4* kmMat4MultiplyTranspose(kmMat4* pOut, const kmMat4* pM1, const kmMat4* pM2)
{
	assert(0);
	return pOut;
}

/** Assigns the value of pIn to pOut */
kmMat4* kmMat4Assign(kmMat4* pOut, const kmMat4* pIn)
{
	assert(pOut != pIn); //You have tried to self-assign!!

	memcpy(pOut->m_Mat, pIn->m_Mat, sizeof(float)*16);

	return pOut;
}

/** Returns true if the 2 matrices are equal (approximately) */
bool kmMat4AreEqual(const kmMat4* pMat1, const kmMat4* pMat2)
{
	assert(pMat1 != pMat2); //You are comparing the same thing!

	for (int i = 0; i < 16; ++i)
	{
		if (!(pMat1->m_Mat[i] + kmEpsilon > pMat2->m_Mat[i] &&
		        pMat1->m_Mat[i] - kmEpsilon < pMat2->m_Mat[i]))
			return false;
	}

	return true;
}

/// Build a matrix from an axis and an angle. Result is stored in pOut. pOut is returned.
kmMat4* kmMat4RotationAxis(kmMat4* pOut, const kmVec3* axis, kmScalar radians)
{
	float rcos = cosf(radians);
	float rsin = sinf(radians);

	pOut->m_Mat[0] = rcos + axis->x * axis->x * (1 - rcos);
	pOut->m_Mat[1] = axis->z * rsin + axis->y * axis->x * (1 - rcos);
	pOut->m_Mat[2] = -axis->y * rsin + axis->z * axis->x * (1 - rcos);
	pOut->m_Mat[3] = 0.0f;

	pOut->m_Mat[4] = -axis->z * rsin + axis->x * axis->y * (1 - rcos);
	pOut->m_Mat[5] = rcos + axis->y * axis->y * (1 - rcos);
	pOut->m_Mat[6] = axis->x * rsin + axis->z * axis->y * (1 - rcos);
	pOut->m_Mat[7] = 0.0f;

	pOut->m_Mat[8] = axis->y * rsin + axis->x * axis->z * (1 - rcos);
	pOut->m_Mat[9] = -axis->x * rsin + axis->y * axis->z * (1 - rcos);
	pOut->m_Mat[10] = rcos + axis->z * axis->z * (1 - rcos);
	pOut->m_Mat[11] = 0.0f;

	pOut->m_Mat[12] = 0.0f;
	pOut->m_Mat[13] = 0.0f;
	pOut->m_Mat[14] = 0.0f;
	pOut->m_Mat[15] = 1.0f;

	return pOut;
}

/// Builds an X-axis rotation matrix and stores it in pOut, returns pOut
kmMat4* kmMat4RotationX(kmMat4* pOut, const float radians)
{
	/*
		 |  1  0       0       0 |
	 M = |  0  cos(A) -sin(A)  0 |
	     |  0  sin(A)  cos(A)  0 |
	     |  0  0       0       1 |

	*/

	pOut->m_Mat[0] = 1.0f;
	pOut->m_Mat[1] = 0.0f;
	pOut->m_Mat[2] = 0.0f;
	pOut->m_Mat[3] = 0.0f;

	pOut->m_Mat[4] = 0.0f;
	pOut->m_Mat[5] = cosf(radians);
	pOut->m_Mat[6] = sinf(radians);
	pOut->m_Mat[7] = 0.0f;

	pOut->m_Mat[8] = 0.0f;
	pOut->m_Mat[9] = -sinf(radians);
	pOut->m_Mat[10] = cosf(radians);
	pOut->m_Mat[11] = 0.0f;

	pOut->m_Mat[12] = 0.0f;
	pOut->m_Mat[13] = 0.0f;
	pOut->m_Mat[14] = 0.0f;
	pOut->m_Mat[15] = 1.0f;

	return pOut;
}

kmMat4* kmMat4RotationY(kmMat4* pOut, const float radians)
{
	/*
	     |  cos(A)  0   sin(A)  0 |
	 M = |  0       1   0       0 |
	     | -sin(A)  0   cos(A)  0 |
	     |  0       0   0       1 |
	*/

	pOut->m_Mat[0] = cosf(radians);
	pOut->m_Mat[1] = 0.0f;
	pOut->m_Mat[2] = -sinf(radians);
	pOut->m_Mat[3] = 0.0f;

	pOut->m_Mat[4] = 0.0f;
	pOut->m_Mat[5] = 1.0f;
	pOut->m_Mat[6] = 0.0f;
	pOut->m_Mat[7] = 0.0f;

	pOut->m_Mat[8] = sinf(radians);
	pOut->m_Mat[9] = 0.0f;
	pOut->m_Mat[10] = cosf(radians);
	pOut->m_Mat[11] = 0.0f;

	pOut->m_Mat[12] = 0.0f;
	pOut->m_Mat[13] = 0.0f;
	pOut->m_Mat[14] = 0.0f;
	pOut->m_Mat[15] = 1.0f;

	return pOut;
}

kmMat4* kmMat4RotationZ(kmMat4* pOut, const float radians)
{
	/*
	     |  cos(A)  -sin(A)   0   0 |
	 M = |  sin(A)   cos(A)   0   0 |
	     |  0        0        1   0 |
	     |  0        0        0   1 |
	*/

	pOut->m_Mat[0] = cosf(radians);
	pOut->m_Mat[1] = sinf(radians);
	pOut->m_Mat[2] = 0.0f;
	pOut->m_Mat[3] = 0.0f;

	pOut->m_Mat[4] = -sinf(radians);;
	pOut->m_Mat[5] = cosf(radians);
	pOut->m_Mat[6] = 0.0f;
	pOut->m_Mat[7] = 0.0f;

	pOut->m_Mat[8] = 0.0f;
	pOut->m_Mat[9] = 0.0f;
	pOut->m_Mat[10] = 1.0f;
	pOut->m_Mat[11] = 0.0f;

	pOut->m_Mat[12] = 0.0f;
	pOut->m_Mat[13] = 0.0f;
	pOut->m_Mat[14] = 0.0f;
	pOut->m_Mat[15] = 1.0f;

	return pOut;
}

kmMat4* kmMat4RotationPitchYawRoll(kmMat4* pOut, const kmScalar pitch, const kmScalar yaw, const kmScalar roll)
{
	double cr = cos(pitch);
	double sr = sin(pitch);
	double cp = cos(yaw);
	double sp = sin(yaw);
	double cy = cos(roll);
	double sy = sin(roll);

	pOut->m_Mat[0] = (kmScalar) cp * cy;
	pOut->m_Mat[4] = (kmScalar) cp * sy;
	pOut->m_Mat[8] = (kmScalar) - sp;

	double srsp = sr * sp;
	double crsp = cr * sp;

	pOut->m_Mat[1] = (kmScalar) srsp * cy - cr * sy;
	pOut->m_Mat[5] = (kmScalar) srsp * sy + cr * cy;
	pOut->m_Mat[9] = (kmScalar) sr * cp;

	pOut->m_Mat[2] = (kmScalar) crsp * cy + sr * sy;
	pOut->m_Mat[6] = (kmScalar) crsp * sy - sr * cy;
	pOut->m_Mat[10] = (kmScalar) cr * cp;

	pOut->m_Mat[3] = pOut->m_Mat[7] = pOut->m_Mat[11] = 0.0;
	pOut->m_Mat[15] = 1.0;

	return pOut;
}

/** Converts a quaternion to a rotation matrix, stored in pOut, returns pOut */
kmMat4* kmMat4RotationQuaternion(kmMat4* pOut, const kmQuaternion* pQ)
{
	pOut->m_Mat[ 0] = 1.0f - 2.0f * (pQ->y * pQ->y + pQ->z * pQ->z );
	pOut->m_Mat[ 4] = 2.0f * (pQ->x * pQ->y + pQ->z * pQ->w);
	pOut->m_Mat[ 8] = 2.0f * (pQ->x * pQ->z - pQ->y * pQ->w);
	pOut->m_Mat[12] = 0.0f;

	// Second row
	pOut->m_Mat[ 1] = 2.0f * ( pQ->x * pQ->y - pQ->z * pQ->w );
	pOut->m_Mat[ 5] = 1.0f - 2.0f * ( pQ->x * pQ->x + pQ->z * pQ->z );
	pOut->m_Mat[ 9] = 2.0f * (pQ->z * pQ->y + pQ->x * pQ->w );
	pOut->m_Mat[13] = 0.0f;

	// Third row
	pOut->m_Mat[ 2] = 2.0f * ( pQ->x * pQ->z + pQ->y * pQ->w );
	pOut->m_Mat[ 6] = 2.0f * ( pQ->y * pQ->z - pQ->x * pQ->w );
	pOut->m_Mat[10] = 1.0f - 2.0f * ( pQ->x * pQ->x + pQ->y * pQ->y );
	pOut->m_Mat[14] = 0.0f;

	// Fourth row
	pOut->m_Mat[ 3] = 0;
	pOut->m_Mat[ 7] = 0;
	pOut->m_Mat[11] = 0;
	pOut->m_Mat[15] = 1.0f;

	return pOut;
}

/** Builds a scaling matrix */
kmMat4* kmMat4Scaling(kmMat4* pOut, const kmScalar x, const kmScalar y, const kmScalar z)
{
	memset(pOut->m_Mat, 0, sizeof(float) * 16);
	pOut->m_Mat[0] = x;
	pOut->m_Mat[5] = y;
	pOut->m_Mat[10] = z;
	pOut->m_Mat[15] = 1.0f;

	return pOut;
}

kmMat4* kmMat4Translation(kmMat4* pOut, const kmScalar x, const kmScalar y, const kmScalar z)
{
	//FIXME: Write a test for this
	//assert(0);
	memset(pOut->m_Mat, 0, sizeof(float) * 16);

	pOut->m_Mat[12] = x;
	pOut->m_Mat[13] = y;
	pOut->m_Mat[14] = z;

	return pOut;
}

kmVec3* kmMat4GetUpVec3(kmVec3* pOut, const kmMat4* pIn)
{
	pOut->x = pIn->m_Mat[4];
	pOut->y = pIn->m_Mat[5];
	pOut->z = pIn->m_Mat[6];

	kmVec3Normalize(pOut, pOut);

	return pOut;
}

kmVec3* kmMat4GetRightVec3(kmVec3* pOut, const kmMat4* pIn)
{
	pOut->x = pIn->m_Mat[0];
	pOut->y = pIn->m_Mat[1];
	pOut->z = pIn->m_Mat[2];

	kmVec3Normalize(pOut, pOut);

	return pOut;
}

kmVec3* kmMat4GetForwardVec3(kmVec3* pOut, const kmMat4* pIn)
{
	pOut->x = pIn->m_Mat[8];
	pOut->y = pIn->m_Mat[9];
	pOut->z = pIn->m_Mat[10];

	kmVec3Normalize(pOut, pOut);

	return pOut;
}

