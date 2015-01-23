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

#include "aabb3.h"


/**
    Initializes the AABB around a central point. If centre is NULL then the origin
    is used. Returns pBox.
*/
kmAABB3* kmAABB3Initialize(kmAABB3* pBox, const kmVec3* centre, const kmScalar width, const kmScalar height, const kmScalar depth) {
    if(!pBox) return 0;
    
    kmVec3 origin;
    kmVec3* point = centre ? (kmVec3*) centre : &origin;
    kmVec3Zero(&origin);
    
    pBox->min.x = point->x - (width / 2);
    pBox->min.y = point->y - (height / 2);
    pBox->min.z = point->z - (depth / 2);
    
    pBox->max.x = point->x + (width / 2);
    pBox->max.y = point->y + (height / 2);
    pBox->max.z = point->z + (depth / 2);
    
    return pBox;
}

/**
 * Returns KM_TRUE if point is in the specified AABB, returns
 * KM_FALSE otherwise.
 */
int kmAABB3ContainsPoint(const kmAABB3* pBox, const kmVec3* pPoint)
{
    if(pPoint->x >= pBox->min.x && pPoint->x <= pBox->max.x &&
       pPoint->y >= pBox->min.y && pPoint->y <= pBox->max.y &&
       pPoint->z >= pBox->min.z && pPoint->z <= pBox->max.z) {
        return KM_TRUE;
    }
       
    return KM_FALSE;
}

/**
 * Assigns pIn to pOut, returns pOut.
 */
kmAABB3* kmAABB3Assign(kmAABB3* pOut, const kmAABB3* pIn)
{
    kmVec3Assign(&pOut->min, &pIn->min);
    kmVec3Assign(&pOut->max, &pIn->max);
    return pOut;
}

/**
 * Scales pIn by s, stores the resulting AABB in pOut. Returns pOut
 */
kmAABB3* kmAABB3Scale(kmAABB3* pOut, const kmAABB3* pIn, kmScalar s)
{
	assert(0 && "Not implemented");
    return pOut;
}

kmBool kmAABB3IntersectsTriangle(kmAABB3* box, const kmVec3* p1, const kmVec3* p2, const kmVec3* p3) {
    assert(0 && "Not implemented");
    return KM_TRUE;
}

kmBool kmAABB3IntersectsAABB(const kmAABB3* box, const kmAABB3* other) {
    return kmAABB3ContainsAABB(box, other) != KM_CONTAINS_NONE;
}

kmEnum kmAABB3ContainsAABB(const kmAABB3* container, const kmAABB3* to_check) {
    kmVec3 corners[8];
    kmEnum result = KM_CONTAINS_ALL;
    kmBool found = KM_FALSE;
        
    kmVec3Fill(&corners[0], to_check->min.x, to_check->min.y, to_check->min.z);
    kmVec3Fill(&corners[1], to_check->max.x, to_check->min.y, to_check->min.z);
    kmVec3Fill(&corners[2], to_check->max.x, to_check->max.y, to_check->min.z);
    kmVec3Fill(&corners[3], to_check->min.x, to_check->max.y, to_check->min.z);
    kmVec3Fill(&corners[4], to_check->min.x, to_check->min.y, to_check->max.z);
    kmVec3Fill(&corners[5], to_check->max.x, to_check->min.y, to_check->max.z);
    kmVec3Fill(&corners[6], to_check->max.x, to_check->max.y, to_check->max.z);
    kmVec3Fill(&corners[7], to_check->min.x, to_check->max.y, to_check->max.z);
        
    for(kmUchar i = 0; i < 8; ++i) {
        if(!kmAABB3ContainsPoint(container, &corners[i])) {
            result = KM_CONTAINS_PARTIAL;
            if(found) {
                /*If we previously found a corner that was within the container*/
                /*We know that partial is the final result*/
                return result;
            }
        } else {
            found = KM_TRUE;
        }
    }
    
    if(!found) {
        result = KM_CONTAINS_NONE;
    }
    
    return result;
}

kmScalar kmAABB3DiameterX(const kmAABB3* aabb) {
    return fabs(aabb->max.x - aabb->min.x);
}

kmScalar kmAABB3DiameterY(const kmAABB3* aabb) {
    return fabs(aabb->max.y - aabb->min.y);
}

kmScalar kmAABB3DiameterZ(const kmAABB3* aabb) {
    return fabs(aabb->max.z - aabb->min.z);
}

kmVec3* kmAABB3Centre(const kmAABB3* aabb, kmVec3* pOut) {
    kmVec3Add(pOut, &aabb->min, &aabb->max);
    kmVec3Scale(pOut, pOut, 0.5);
    return pOut;
}

/**
 * @brief kmAABB3ExpandToContain
 * @param pOut - The resulting AABB
 * @param pIn - The original AABB
 * @param other - Another AABB that you want pIn expanded to contain
 * @return
 */
kmAABB3* kmAABB3ExpandToContain(kmAABB3* pOut, const kmAABB3* pIn, const kmAABB3* other) {
    kmAABB3 result;

    result.min.x = (pIn->min.x < other->min.x)?pIn->min.x:other->min.x;
    result.max.x = (pIn->max.x > other->max.x)?pIn->max.x:other->max.x;
    result.min.y = (pIn->min.y < other->min.y)?pIn->min.y:other->min.y;
    result.max.y = (pIn->max.y > other->max.y)?pIn->max.y:other->max.y;
    result.min.z = (pIn->min.z < other->min.z)?pIn->min.z:other->min.z;
    result.max.z = (pIn->max.z > other->max.z)?pIn->max.z:other->max.z;

    kmAABB3Assign(pOut, &result);

    return pOut;
}
