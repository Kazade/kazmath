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

#include <memory.h>
#include <assert.h>

#include "utility.h"
#include "vec4.h"
#include "mat4.h"

/// Adds 2 4D vectors together. The result is store in pOut, the function returns
/// pOut so that it can be nested in another function.
kmVec4* kmVec4Add(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2) {
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	pOut->w = pV1->w - pV2->w;

	return pOut;
}

/*
kmVec4* kmVec4Cross(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2, const kmVec4* pV3)
{
	pOut->x = pV1->y * (pV2->z * pV3->w - pV3->z * pV2->w) - pV1->z * (pV2->
	pOut->y =
	pOut->z =
	pOut->w =


| i  j  k  l  |   | ay*(bz*cw - cz*bw) - az*(by*cw - cy*bw) + aw*(by*cz - cy*bz) |
| ax ay az aw |   |-ax*(bz*cw - cz*bw) + az*(bx*cw - cx*bw) - aw*(bx*cz - cx*bz) |
| bx by bz bw | = | ax*(by*cw - cy*bw) - ay*(bx*cw - cx*bw) + aw*(bx*cy - cx*by) |
| cx cy cz cw |   |-ax*(by*cz - cy*bz) + ay*(bx*cz - cx*bz) - az*(bx*cy - cx*by) |

	return pOut;
}*/

/// Returns the dot product of 2 4D vectors
kmScalar kmVec4Dot(const kmVec4* pV1, const kmVec4* pV2) {
	return (  pV1->x * pV2->x
			+ pV1->y * pV2->y
			+ pV1->z * pV2->z
			+ pV1->w * pV2->w );
}

/// Returns the length of a 4D vector, this uses a sqrt so if the squared length will do use
/// kmVec4LengthSq
kmScalar kmVec4Length(const kmVec4* pIn) {
	return sqrtf(kmSQR(pIn->x) + kmSQR(pIn->y) + kmSQR(pIn->z) + kmSQR(pIn->w));
}

/// Returns the length of the 4D vector squared.
kmScalar kmVec4LengthSq(const kmVec4* pIn) {
	return kmSQR(pIn->x) + kmSQR(pIn->y) + kmSQR(pIn->z) + kmSQR(pIn->w);
}

/// Returns the interpolation of 2 4D vectors based on t. Currently not implemented!
kmVec4* kmVec4Lerp(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2, kmScalar t) {
    assert(0);
    return pOut;
}

/// Normalizes a 4D vector. The result is stored in pOut. pOut is returned
kmVec4* kmVec4Normalize(kmVec4* pOut, const kmVec4* pIn) {
	kmScalar l = 1.0 / kmVec4Length(pIn);

	pOut->x *= l;
	pOut->y *= l;
	pOut->z *= l;
	pOut->w *= l;

	return pOut;
}

/// Scales a vector to the required length. This performs a Normalize before multiplying by S.
kmVec4* kmVec4Scale(kmVec4* pOut, const kmVec4* pIn, const kmScalar s) {
	kmVec4Normalize(pOut, pIn);

	pOut->x *= s;
	pOut->y *= s;
	pOut->z *= s;
	pOut->w *= s;
	return pOut;
}

/// Subtracts one 4D pV2 from pV1. The result is stored in pOut. pOut is returned
kmVec4* kmVec4Subtract(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2) {
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	pOut->w = pV1->w - pV2->w;

	return pOut;
}

/// Transforms a 4D vector by a matrix, the result is stored in pOut, and pOut is returned.
kmVec4* kmVec4Transform(kmVec4* pOut, const kmVec4* pV, const kmMat4* pM) {
	pOut->x = pV->x * pM->m_Mat[0] + pV->y * pM->m_Mat[4] + pV->z * pM->m_Mat[8] + pV->w * pM->m_Mat[12];
	pOut->y = pV->x * pM->m_Mat[1] + pV->y * pM->m_Mat[5] + pV->z * pM->m_Mat[9] + pV->w * pM->m_Mat[13];
	pOut->z = pV->x * pM->m_Mat[2] + pV->y * pM->m_Mat[6] + pV->z * pM->m_Mat[10] + pV->w * pM->m_Mat[14];
    pOut->w = pV->x * pM->m_Mat[3] + pV->y * pM->m_Mat[7] + pV->z * pM->m_Mat[11] + pV->w * pM->m_Mat[15];
	return pOut;
}

/// Loops through an input array transforming each vec4 by the matrix.
kmVec4* kmVec4TransformArray(kmVec4* pOut, unsigned int outStride,
			const kmVec4* pV, unsigned int vStride, const kmMat4* pM, unsigned int count) {
    unsigned int i = 0;
    //Go through all of the vectors
    while (i < count) {
        const kmVec4* in = pV + (i * vStride); //Get a pointer to the current input
        kmVec4* out = pOut + (i * outStride); //and the current output
        kmVec4Transform(out, in, pM); //Perform transform on it
        ++i;
    }

    return pOut;
}

bool kmVec4AreEqual(const kmVec4* p1, const kmVec4* p2) {
	return (
		(p1->x < p2->x + kmEpsilon && p1->x > p2->x - kmEpsilon) &&
		(p1->y < p2->y + kmEpsilon && p1->y > p2->y - kmEpsilon) &&
		(p1->z < p2->z + kmEpsilon && p1->z > p2->z - kmEpsilon) &&
		(p1->w < p2->w + kmEpsilon && p1->w > p2->w - kmEpsilon)
	);
}

kmVec4* kmVec4Assign(kmVec4* pOut, const kmVec4* pIn) {
	assert(pOut != pIn);

	memcpy(pOut, pIn, sizeof(float) * 4);

	return pOut;
}

