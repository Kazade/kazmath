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

#ifndef VEC4_H_INCLUDED
#define VEC4_H_INCLUDED

#include "utility.h"

struct kmMat4;

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

typedef struct kmVec4 {
	kmScalar x;
	kmScalar y;
	kmScalar z;
	kmScalar w;
} kmVec4;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

kmVec4* kmVec4Add(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2);
kmScalar kmVec4Dot(const kmVec4* pV1, const kmVec4* pV2);
kmScalar kmVec4Length(const kmVec4* pIn);
kmScalar kmVec4LengthSq(const kmVec4* pIn);
kmVec4* kmVec4Lerp(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2, kmScalar t);
kmVec4* kmVec4Normalize(kmVec4* pOut, const kmVec4* pIn);
kmVec4* kmVec4Scale(kmVec4* pOut, const kmVec4* pIn, const kmScalar s); ///< Scales a vector to length s
kmVec4* kmVec4Subtract(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2);
kmVec4* kmVec4Transform(kmVec4* pOut, const kmVec4* pV, const struct kmMat4* pM);
kmVec4* kmVec4TransformArray(kmVec4* pOut, unsigned int outStride,
			const kmVec4* pV, unsigned int vStride, const struct kmMat4* pM, unsigned int count);
bool 	kmVec4AreEqual(const kmVec4* p1, const kmVec4* p2);
kmVec4* kmVec4Assign(kmVec4* pOut, const kmVec4* pIn);

#ifdef __cplusplus
}
#endif

#endif // VEC4_H_INCLUDED
