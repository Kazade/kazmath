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

#ifndef KAZMATH_AABB2D_H_INCLUDED
#define KAZMATH_AABB2D_H_INCLUDED

#include "vec2.h"
#include "utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A struture that represents an axis-aligned
 * bounding box.
 */
typedef struct kmAABB2 {
    kmVec2 min; /** The max corner of the box */
    kmVec2 max; /** The min corner of the box */
} kmAABB2;


kmAABB2* kmAABB2Initialize(kmAABB2* pBox, const kmVec2* centre, const kmScalar width, const kmScalar height, const kmScalar depth);
kmAABB2* kmAABB2Sanitize(kmAABB2* pOut, const kmAABB2* pIn );
int kmAABB2ContainsPoint(const kmAABB2* pBox, const kmVec2* pPoint);
kmAABB2* kmAABB2Assign(kmAABB2* pOut, const kmAABB2* pIn);
kmAABB2* kmAABB2Translate(kmAABB2* pOut, const kmAABB2* pIn, const kmVec2 *translation );
kmAABB2* kmAABB2Scale(kmAABB2* pOut, const kmAABB2* pIn, kmScalar s);
kmAABB2* kmAABB2ScaleWithPivot( kmAABB2* pOut, const kmAABB2* pIn, const kmVec2 *pivot, kmScalar s );
kmEnum kmAABB2ContainsAABB(const kmAABB2* container, const kmAABB2* to_check);
kmScalar kmAABB2DiameterX(const kmAABB2* aabb);
kmScalar kmAABB2DiameterY(const kmAABB2* aabb);
kmVec2* kmAABB2Centre(const kmAABB2* aabb, kmVec2* pOut);
kmAABB2* kmAABB2ExpandToContain(kmAABB2* pOut, const kmAABB2* pIn, const kmAABB2* other);

#ifdef __cplusplus
}
#endif

#endif
