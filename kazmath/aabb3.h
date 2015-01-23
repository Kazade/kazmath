/*
Copyright (c) 2008, Luke Benstead.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef KAZMATH_AABB3D_H_INCLUDED
#define KAZMATH_AABB3D_H_INCLUDED

#include "vec3.h"
#include "utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A struture that represents an axis-aligned
 * bounding box.
 */
typedef struct kmAABB3 {
    kmVec3 min; /** The max corner of the box */
    kmVec3 max; /** The min corner of the box */
} kmAABB3;


kmAABB3* kmAABB3Initialize(kmAABB3* pBox, const kmVec3* centre, const kmScalar width, const kmScalar height, const kmScalar depth);
int kmAABB3ContainsPoint(const kmAABB3* pBox, const kmVec3* pPoint);
kmAABB3* kmAABB3Assign(kmAABB3* pOut, const kmAABB3* pIn);
kmAABB3* kmAABB3Scale(kmAABB3* pOut, const kmAABB3* pIn, kmScalar s);
kmBool kmAABB3IntersectsTriangle(kmAABB3* box, const kmVec3* p1, const kmVec3* p2, const kmVec3* p3);
kmBool kmAABB3IntersectsAABB(const kmAABB3* box, const kmAABB3* other);
kmEnum kmAABB3ContainsAABB(const kmAABB3* container, const kmAABB3* to_check);
kmScalar kmAABB3DiameterX(const kmAABB3* aabb);
kmScalar kmAABB3DiameterY(const kmAABB3* aabb);
kmScalar kmAABB3DiameterZ(const kmAABB3* aabb);
kmVec3* kmAABB3Centre(const kmAABB3* aabb, kmVec3* pOut);
kmAABB3* kmAABB3ExpandToContain(kmAABB3* pOut, const kmAABB3* pIn, const kmAABB3* other);

#ifdef __cplusplus
}
#endif

#endif
