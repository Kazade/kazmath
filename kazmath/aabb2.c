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

#include "aabb2.h"


/**
    Initializes the AABB around a central point. If centre is NULL then the origin
    is used. Returns pBox.
*/
kmAABB2* kmAABB2Initialize( kmAABB2* pBox, const kmVec2* centre, const kmScalar width, const kmScalar height, const kmScalar depth) {
    if(!pBox) return 0;
    
    kmVec2 origin;
    kmVec2* point = centre ? (kmVec2*) centre : &origin;
    kmVec2Fill(&origin, .0f, .0f);
    
    pBox->min.x = point->x - (width / 2);
    pBox->min.y = point->y - (height / 2);
    
    pBox->max.x = point->x + (width / 2);
    pBox->max.y = point->y + (height / 2);
    
    return pBox;
}

/** 
 *  Makes sure that min corresponds to the minimum values and max 
 *  to the maximum
 */
kmAABB2* kmAABB2Sanitize(kmAABB2* pOut, const kmAABB2* pIn)
{
    if( pIn->min.x <= pIn->max.x ){
        pOut->min.x = pIn->min.x;
        pOut->max.x = pIn->max.x; 
    }else{
        pOut->min.x = pIn->max.x;
        pOut->max.x = pIn->min.x; 
    }

    if( pIn->min.y <= pIn->max.y ){
        pOut->min.y = pIn->min.y;
        pOut->max.y = pIn->max.y; 
    }else{
        pOut->min.y = pIn->max.y;
        pOut->max.y = pIn->min.y; 
    }

    return pOut;
}

/**
 * Returns KM_TRUE if point is in the specified AABB, returns
 * KM_FALSE otherwise.
 */
int kmAABB2ContainsPoint(const kmAABB2* pBox, const kmVec2* pPoint)
{
    if(pPoint->x >= pBox->min.x && pPoint->x <= pBox->max.x &&
       pPoint->y >= pBox->min.y && pPoint->y <= pBox->max.y ) {
        return KM_TRUE;
    }
       
    return KM_FALSE;
}

/**
 * Assigns pIn to pOut, returns pOut.
 */
kmAABB2* kmAABB2Assign(kmAABB2* pOut, const kmAABB2* pIn)
{
    kmVec2Assign(&pOut->min, &pIn->min);
    kmVec2Assign(&pOut->max, &pIn->max);
    return pOut;
}

kmAABB2* kmAABB2Translate( kmAABB2* pOut, const kmAABB2* pIn, const kmVec2 *translation )
{
    kmVec2Add( &(pOut->min), &(pIn->min), translation );
    kmVec2Add( &(pOut->max), &(pIn->max), translation );
    return pOut;
}

/**
 * Scales pIn by s, stores the resulting AABB in pOut. Returns pOut. 
 * It modifies both points, so position of the box will be changed. Use
 * kmAABB2ScaleWithPivot to specify the origin of the scale.
 */
kmAABB2* kmAABB2Scale(kmAABB2* pOut, const kmAABB2* pIn, kmScalar s)
{
    kmVec2Scale( &(pOut->max), &(pIn->max), s ); 
    kmVec2Scale( &(pOut->min), &(pIn->min), s ); 
    return pOut;
}

/** 
 * Scales pIn by s, using pivot as the origin for the scale.
 */
kmAABB2* kmAABB2ScaleWithPivot( kmAABB2* pOut, const kmAABB2* pIn, const kmVec2 *pivot, kmScalar s )
{
    kmVec2 translate;
    translate.x = -pivot->x;
    translate.y = -pivot->y;

    kmAABB2Translate( pOut, pIn, &translate ); 
    kmAABB2Scale( pOut, pIn, s );
    kmAABB2Translate( pOut, pIn, pivot ); 

    return pOut;
}

kmEnum kmAABB2ContainsAABB(const kmAABB2* container, const kmAABB2* to_check) {
    kmVec2 corners[4];
    kmVec2Fill(&corners[0], to_check->min.x, to_check->min.y);
    kmVec2Fill(&corners[1], to_check->max.x, to_check->min.y);
    kmVec2Fill(&corners[2], to_check->max.x, to_check->max.y);
    kmVec2Fill(&corners[3], to_check->min.x, to_check->max.y);
    
    // since KM_TRUE equals 1 , we can count the number of contained points
    // by actually adding the results: 
    int nContains = kmAABB2ContainsPoint( container, &corners[0] ) +
                    kmAABB2ContainsPoint( container, &corners[1] ) +
                    kmAABB2ContainsPoint( container, &corners[2] ) +
                    kmAABB2ContainsPoint( container, &corners[3] );

    if( nContains == 0 ){ 
        return KM_CONTAINS_NONE; 
    }else if( nContains < 4 ){
        return KM_CONTAINS_PARTIAL;
    }else{ 
        return KM_CONTAINS_ALL;
    }
}

kmScalar kmAABB2DiameterX(const kmAABB2* aabb) {
    return aabb->max.x - aabb->min.x;
}

kmScalar kmAABB2DiameterY(const kmAABB2* aabb) {
    return aabb->max.y - aabb->min.y;
}

kmVec2* kmAABB2Centre(const kmAABB2* aabb, kmVec2* pOut) {
    kmVec2Add(pOut, &aabb->min, &aabb->max);
    kmVec2Scale(pOut, pOut, 0.5);
    return pOut;
}

/**
 * @brief kmAABB2ExpandToContain
 * @param pOut - The resulting AABB
 * @param pIn - The original AABB
 * @param other - Another AABB that you want pIn expanded to contain
 * @return
 */
kmAABB2* kmAABB2ExpandToContain(kmAABB2* pOut, const kmAABB2* pIn, const kmAABB2* other) {
    kmAABB2 result;

    result.min.x = (pIn->min.x < other->min.x)?pIn->min.x:other->min.x;
    result.max.x = (pIn->max.x > other->max.x)?pIn->max.x:other->max.x;
    result.min.y = (pIn->min.y < other->min.y)?pIn->min.y:other->min.y;
    result.max.y = (pIn->max.y > other->max.y)?pIn->max.y:other->max.y;

    kmAABB2Assign(pOut, &result);

    return pOut;
}