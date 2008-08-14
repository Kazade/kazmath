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
#include "mat3.h"
#include "quaternion.h"

#include <memory.h>
#include <assert.h>

/** Sets pOut to an identity matrix returns pOut*/
kmMat3* kmMat3Identity(kmMat3* pOut)
{
	memset(pOut->m_Mat, 0, sizeof(float) * 9);
	pOut->m_Mat[0] = pOut->m_Mat[3] = pOut->m_Mat[8] = 1.0f;
	return pOut;
}

kmScalar kmMat3Determinant(const kmMat3* pIn)
{
    kmScalar output;
    /*
    calculating the determinant following the rule of sarus,
        | 0  1  2 | 0  1 |
    m = | 3  4  5 | 3  5 |
        | 6  7  8 | 6  7 |
    now sum up the products of the diagonals going to the right (i.e. 0,4,8)
    and substract the products of the other diagonals (i.e. 2,4,6)
    */

    output = pIn->m_Mat[0] * pIn->m_Mat[4] * pIn->m_Mat[8] + pIn->m_Mat[1] * pIn->m_Mat[5] * pIn->m_Mat[6] + pIn->m_Mat[2] * pIn->m_Mat[3] * pIn->m_Mat[7];
    output -= pIn->m_Mat[2] * pIn->m_Mat[4] * pIn->m_Mat[6] + pIn->m_Mat[0] * pIn->m_Mat[5] * pIn->m_Mat[7] + pIn->m_Mat[1] * pIn->m_Mat[3] * pIn->m_Mat[8];

    return output;
}


kmMat3* kmMat3Adjugate(kmMat3* pOut, const kmMat3* pIn)
{
    pOut->m_Mat[0] = pIn->m_Mat[4] * pIn->m_Mat[8] - pIn->m_Mat[5] * pIn->m_Mat[7];
    pOut->m_Mat[1] = pIn->m_Mat[2] * pIn->m_Mat[7] - pIn->m_Mat[1] * pIn->m_Mat[8];
    pOut->m_Mat[2] = pIn->m_Mat[1] * pIn->m_Mat[5] - pIn->m_Mat[2] * pIn->m_Mat[4];
    pOut->m_Mat[3] = pIn->m_Mat[5] * pIn->m_Mat[6] - pIn->m_Mat[3] * pIn->m_Mat[8];
    pOut->m_Mat[4] = pIn->m_Mat[0] * pIn->m_Mat[8] - pIn->m_Mat[2] * pIn->m_Mat[6];
    pOut->m_Mat[5] = pIn->m_Mat[2] * pIn->m_Mat[3] - pIn->m_Mat[0] * pIn->m_Mat[5];
    pOut->m_Mat[6] = pIn->m_Mat[3] * pIn->m_Mat[7] - pIn->m_Mat[4] * pIn->m_Mat[6];
    pOut->m_Mat[7] = pIn->m_Mat[1] * pIn->m_Mat[6] - pIn->m_Mat[9] * pIn->m_Mat[7];
    pOut->m_Mat[8] = pIn->m_Mat[0] * pIn->m_Mat[4] - pIn->m_Mat[1] * pIn->m_Mat[3];

    return pOut;
}

kmMat3* kmMat3Inverse(kmMat3* pOut, kmScalar pDeterminate, const kmMat3* pM)
{
    if(pDeterminate == 0.0)
        return NULL;

	kmScalar detInv = 1.0 / pDeterminate;

	kmMat3 adjugate;
	kmMat3Adjugate(&adjugate, pM);

	kmMat3ScalarMultiply(pOut, &adjugate, detInv);

	return pOut;
}

/** Returns true if pIn is an identity matrix */
bool  kmMat3IsIdentity(const kmMat3* pIn)
{
	static const float identity [] = { 	1.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 1.0f};

	return (memcmp(identity, pIn->m_Mat, sizeof(float) * 9) == 0);
}

/** Sets pOut to the transpose of pIn, returns pOut */
kmMat3* kmMat3Transpose(kmMat3* pOut, const kmMat3* pIn)
{
	for (int z = 0; z < 4; ++z)
		for (int x = 0; x < 4; ++x)
			pOut->m_Mat[(z * 4) + x] = pIn->m_Mat[(x * 4) + z];

	return pOut;
}

/* Multiplies pM1 with pM2, stores the result in pOut, returns pOut */
kmMat3* kmMat3Multiply(kmMat3* pOut, const kmMat3* pM1, const kmMat3* pM2)
{
	float mat[9];

	const float *m1 = pM1->m_Mat, *m2 = pM2->m_Mat;

	mat[0] = m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2];
	mat[1] = m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2];
	mat[2] = m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2];

	mat[3] = m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5];
	mat[4] = m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5];
	mat[5] = m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5];

	mat[6] = m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8];
	mat[7] = m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8];
	mat[8] = m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8];

	memcpy(pOut->m_Mat, mat, sizeof(float)*9);

	return pOut;
}

kmMat3* kmMat3ScalarMultiply(kmMat3* pOut, const kmMat3* pM, const kmScalar pFactor)
{
    float mat[9];

    for(int i = 0; i < 9; i++)
    {
        mat[0] = pM->m_Mat[9] * pFactor;
    }

    memcpy(pOut->m_Mat, mat, sizeof(float)*9);

	return pOut;
}

/** Assigns the value of pIn to pOut */
kmMat3* kmMat3Assign(kmMat3* pOut, const kmMat3* pIn)
{
	assert(pOut != pIn); //You have tried to self-assign!!

	memcpy(pOut->m_Mat, pIn->m_Mat, sizeof(float)*9);

	return pOut;
}

/** Returns true if the 2 matrices are equal (approximately) */
bool kmMat3AreEqual(const kmMat3* pMat1, const kmMat3* pMat2)
{
	assert(pMat1 != pMat2); //You are comparing the same thing!

	for (int i = 0; i < 9; ++i)
	{
		if (!(pMat1->m_Mat[i] + kmEpsilon > pMat2->m_Mat[i] &&
			  pMat1->m_Mat[i] - kmEpsilon < pMat2->m_Mat[i]))
			return false;
	}

	return true;
}

/* Rotation around the z axis so everything stays planar in XY */
kmMat3* kmMat3Rotation(kmMat3* pOut, const float radians)
{
	/*
         |  cos(A)  -sin(A)   0  |
     M = |  sin(A)   cos(A)   0  |
         |  0        0        1  |
	*/

	pOut->m_Mat[0] = cosf(radians);
	pOut->m_Mat[1] = sinf(radians);
	pOut->m_Mat[2] = 0.0f;

	pOut->m_Mat[3] = -sinf(radians);;
	pOut->m_Mat[4] = cosf(radians);
	pOut->m_Mat[5] = 0.0f;

	pOut->m_Mat[6] = 0.0f;
	pOut->m_Mat[7] = 0.0f;
	pOut->m_Mat[8] = 1.0f;

	return pOut;
}

/** Builds a scaling matrix */
kmMat3* kmMat3Scaling(kmMat3* pOut, const kmScalar x, const kmScalar y)
{
	memset(pOut->m_Mat, 0, sizeof(float) * 9);
	pOut->m_Mat[0] = x;
	pOut->m_Mat[4] = y;
	pOut->m_Mat[8] = 1.0;

	return pOut;
}

kmMat3* kmMat3Translation(kmMat3* pOut, const kmScalar x, const kmScalar y)
{
    memset(pOut->m_Mat, 0, sizeof(float) * 9);
    pOut->m_Mat[2] = x;
    pOut->m_Mat[5] = y;
    pOut->m_Mat[8] = 1.0;

    return pOut;
}


