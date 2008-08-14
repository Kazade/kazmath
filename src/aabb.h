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

#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

typedef struct kmAABB {
	kmVec3 m_Min; /** The max corner of the box */
	kmVec3 m_Max; /** The min corner of the box */
} kmAABB;

bool kmAABBPointInBox(const kmVec3* point);

kmAABB* kmAABBAssign(kmAABB* pOut, const kmAABB* pIn);
kmAABB* kmAABBScale(kmAABB* pOut, const kmAABB* pIn, kmScalar s);

#endif AABB_H_INCLUDED
