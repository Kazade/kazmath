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
#include "vec4.h"
#include "mat4.h"
#include "vec3.h"

///< Returns the length of the vector
kmScalar kmVec3Length(const kmVec3* pIn)
{
	return sqrtf(kmSQR(pIn->x) + kmSQR(pIn->y) + kmSQR(pIn->z));
}

///< Returns the square of the length of the vector
kmScalar kmVec3LengthSq(const kmVec3* pIn)
{
	return kmSQR(pIn->x) + kmSQR(pIn->y) + kmSQR(pIn->z);
}

 ///< Returns the vector passed in set to unit length
kmVec3* kmVec3Normalize(kmVec3* pOut, const kmVec3* pIn)
{
	kmScalar l = 1.0 / kmVec3Length(pIn);

	kmVec3 v;
	v.x = pIn->x * l;
	v.y = pIn->y * l;
	v.z = pIn->z * l;

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

	return pOut;
}

///< Returns a vector perpendicular to 2 other vectors
kmVec3* kmVec3Cross(kmVec3* pOut, const kmVec3* pV1, const kmVec3* pV2)
{

	kmVec3 v;

	v.x = (pV1->y * pV2->z) - (pV1->z * pV2->y);
	v.y = (pV1->z * pV2->x) - (pV1->x * pV2->z);
	v.z = (pV1->x * pV2->y) - (pV1->y * pV2->x);

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

	return pOut;
}

///< Returns the cosine of the angle between 2 vectors
kmScalar kmVec3Dot(const kmVec3* pV1, const kmVec3* pV2)
{
	return (  pV1->x * pV2->x
			+ pV1->y * pV2->y
			+ pV1->z * pV2->z );
}

///< Adds 2 vectors and returns the result
kmVec3* kmVec3Add(kmVec3* pOut, const kmVec3* pV1, const kmVec3* pV2)
{
	kmVec3 v;

	v.x = pV1->x + pV2->x;
	v.y = pV1->y + pV2->y;
	v.z = pV1->z + pV2->z;

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

	return pOut;
}

 ///< Subtracts 2 vectors and returns the result
kmVec3* kmVec3Subtract(kmVec3* pOut, const kmVec3* pV1, const kmVec3* pV2)
{
	kmVec3 v;

	v.x = pV1->x - pV2->x;
	v.y = pV1->y - pV2->y;
	v.z = pV1->z - pV2->z;

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

	return pOut;
}

 ///< Transforms vector (x, y, z, 1) by a given matrix.
kmVec3* kmVec3Transform(kmVec3* pOut, const kmVec3* pV, const kmMat4* pM)
{
	/*
		a = (Vx, Vy, Vz, 1)
		b = (a×M)T
		Out = (bx, by, bz)
	*/

	kmVec3 v;

	v.x = pV->x * pM->m_Mat[0] + pV->y * pM->m_Mat[4] + pV->z * pM->m_Mat[8] + pM->m_Mat[12];
	v.y = pV->x * pM->m_Mat[1] + pV->y * pM->m_Mat[5] + pV->z * pM->m_Mat[9] + pM->m_Mat[13];
	v.z = pV->x * pM->m_Mat[2] + pV->y * pM->m_Mat[6] + pV->z * pM->m_Mat[10] + pM->m_Mat[14];

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

	return pOut;
}

kmVec3* kmVec3InverseTransform(kmVec3* pOut, const kmVec3* pVect, const kmMat4* pM)
{
	kmVec3 v1, v2;

	v1.x = pVect->x - pM->m_Mat[12];
	v1.y = pVect->y - pM->m_Mat[13];
	v1.z = pVect->z - pM->m_Mat[14];

	v2.x = v1.x * pM->m_Mat[0] + v1.y * pM->m_Mat[1] + v1.z * pM->m_Mat[2];
	v2.y = v1.x * pM->m_Mat[4] + v1.y * pM->m_Mat[5] + v1.z * pM->m_Mat[6];
	v2.z = v1.x * pM->m_Mat[8] + v1.y * pM->m_Mat[9] + v1.z * pM->m_Mat[10];

	pOut->x = v2.x;
	pOut->y = v2.y;
	pOut->z = v2.z;

	return pOut;
}

kmVec3* kmVec3InverseTransformNormal(kmVec3* pOut, const kmVec3* pVect, const kmMat4* pM)
{
	kmVec3 v;

	v.x = pVect->x * pM->m_Mat[0] + pVect->y * pM->m_Mat[1] + pVect->z * pM->m_Mat[2];
	v.y = pVect->x * pM->m_Mat[4] + pVect->y * pM->m_Mat[5] + pVect->z * pM->m_Mat[6];
	v.z = pVect->x * pM->m_Mat[8] + pVect->y * pM->m_Mat[9] + pVect->z * pM->m_Mat[10];

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

	return pOut;
}

kmVec3* kmVec3TransformCoord(kmVec3* pOut, const kmVec3* pV, const kmMat4* pM)
{
	/*
        a = (Vx, Vy, Vz, 1)
        b = (a×M)T
        Out = 1⁄bw(bx, by, bz)
	*/

    assert(0);

	return pOut;
}

kmVec3* kmVec3TransformNormal(kmVec3* pOut, const kmVec3* pV, const kmMat4* pM)
{
/*
    a = (Vx, Vy, Vz, 0)
    b = (a×M)T
    Out = (bx, by, bz)
*/

	kmVec3 v;

	v.x = pV->x * pM->m_Mat[0] + pV->y * pM->m_Mat[4] + pV->z * pM->m_Mat[8];
	v.y = pV->x * pM->m_Mat[1] + pV->y * pM->m_Mat[5] + pV->z * pM->m_Mat[9];
	v.z = pV->x * pM->m_Mat[2] + pV->y * pM->m_Mat[6] + pV->z * pM->m_Mat[10];

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

    return pOut;

}

///< Scales a vector to length s
kmVec3* kmVec3Scale(kmVec3* pOut, const kmVec3* pIn, const kmScalar s)
{
	pOut->x *= s;
	pOut->y *= s;
	pOut->z *= s;

	return pOut;
}

int kmVec3AreEqual(const kmVec3* p1, const kmVec3* p2)
{
	if ((p1->x < (p2->x + kmEpsilon) && p1->x > (p2->x - kmEpsilon)) &&
		(p1->y < (p2->y + kmEpsilon) && p1->y > (p2->y - kmEpsilon)) &&
		(p1->z < (p2->z + kmEpsilon) && p1->z > (p2->z - kmEpsilon))) {
		return 1;		
	}
	
	return 0;
}

kmVec3* kmVec3Assign(kmVec3* pOut, const kmVec3* pIn) {
	if (pOut == pIn) {
		return pOut;
	}

	pOut->x = pIn->x;
	pOut->y = pIn->y;
	pOut->z = pIn->z;

	return pOut;
}

kmVec3* kmVec3Zero(kmVec3* pOut) {
	pOut->x = 0.0f;
	pOut->y = 0.0f;
	pOut->z = 0.0f;

	return pOut;
}
