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

#ifndef KAZMATH_AABB_H_INCLUDED
#define KAZMATH_AABB_H_INCLUDED

#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kmAABB {
	kmVec3 min; /** The max corner of the box */
	kmVec3 max; /** The min corner of the box */
} kmAABB;

int kmAABBPointInBox(const kmVec3* point);

kmAABB* kmAABBAssign(kmAABB* pOut, const kmAABB* pIn);
kmAABB* kmAABBScale(kmAABB* pOut, const kmAABB* pIn, kmScalar s);
#ifdef __cplusplus
}
#endif
#endif
