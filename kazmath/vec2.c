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

#include <assert.h>
#include <stdlib.h>

#include "mat3.h"
#include "vec2.h"
#include "utility.h"

const kmVec2 KM_VEC2_POS_Y = { 0, 1 };
const kmVec2 KM_VEC2_NEG_Y = { 0, -1 };
const kmVec2 KM_VEC2_NEG_X = { -1, 0 };
const kmVec2 KM_VEC2_POS_X = { 1, 0 };
const kmVec2 KM_VEC2_ZERO = { 0, 0 };

kmVec2* kmVec2Fill(kmVec2* pOut, kmScalar x, kmScalar y)
{
    pOut->x = x;
    pOut->y = y;
    return pOut;
}

kmScalar kmVec2Length(const kmVec2* pIn)
{
    return sqrtf(kmSQR(pIn->x) + kmSQR(pIn->y));
}

kmScalar kmVec2LengthSq(const kmVec2* pIn)
{
    return kmSQR(pIn->x) + kmSQR(pIn->y);
}

kmVec2* kmVec2Lerp(kmVec2* pOut, const kmVec2* pV1, const kmVec2* pV2, kmScalar t) {
    pOut->x = pV1->x + t * ( pV2->x - pV1->x ); 
    pOut->y = pV1->y + t * ( pV2->y - pV1->y ); 
    return pOut;
}

kmVec2* kmVec2Normalize(kmVec2* pOut, const kmVec2* pIn)
{
	kmScalar l;
	kmVec2 v;
        if (!pIn->x && !pIn->y)
                return kmVec2Assign(pOut, pIn);

	l = 1.0f / kmVec2Length(pIn);

	v.x = pIn->x * l;
	v.y = pIn->y * l;
    
	pOut->x = v.x;
	pOut->y = v.y;

	return pOut;
}

kmVec2* kmVec2Add(kmVec2* pOut, const kmVec2* pV1, const kmVec2* pV2)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;

	return pOut;
}

kmScalar kmVec2Dot(const kmVec2* pV1, const kmVec2* pV2)
{
    return pV1->x * pV2->x + pV1->y * pV2->y;
}

kmScalar kmVec2Cross(const kmVec2* pV1, const kmVec2* pV2) 
{
    return pV1->x * pV2->y - pV1->y * pV2->x;
}

kmVec2* kmVec2Subtract(kmVec2* pOut, const kmVec2* pV1, const kmVec2* pV2)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;

	return pOut;
}

kmVec2* kmVec2Mul( kmVec2* pOut,const kmVec2* pV1, const kmVec2* pV2 ) {
    pOut->x = pV1->x * pV2->x;
    pOut->y = pV1->y * pV2->y;
    return pOut;
}

kmVec2* kmVec2Div( kmVec2* pOut,const kmVec2* pV1, const kmVec2* pV2 ) {
    if ( pV2->x && pV2->y ){
        pOut->x = pV1->x / pV2->x;
        pOut->y = pV1->y / pV2->y;
    }
    return pOut;
}

kmVec2* kmVec2Transform(kmVec2* pOut, const kmVec2* pV, const kmMat3* pM)
{
    kmVec2 v;

    v.x = pV->x * pM->mat[0] + pV->y * pM->mat[3] + pM->mat[6];
    v.y = pV->x * pM->mat[1] + pV->y * pM->mat[4] + pM->mat[7];

    pOut->x = v.x;
    pOut->y = v.y;

    return pOut;
}

kmVec2* kmVec2TransformCoord(kmVec2* pOut, const kmVec2* pV, const kmMat3* pM)
{
	assert(0);
    return NULL;
}

kmVec2* kmVec2Scale(kmVec2* pOut, const kmVec2* pIn, const kmScalar s)
{
	pOut->x = pIn->x * s;
	pOut->y = pIn->y * s;

	return pOut;
}

kmBool kmVec2AreEqual(const kmVec2* p1, const kmVec2* p2)
{
    if((!kmAlmostEqual(p1->x, p2->x)) || (!kmAlmostEqual(p1->y, p2->y))) {
        return KM_FALSE;
    }

    return KM_TRUE;
}

kmVec2* kmVec2Assign(kmVec2* pOut, const kmVec2* pIn) {
	if (pOut == pIn) {
		return pOut;
	}

	pOut->x = pIn->x;
	pOut->y = pIn->y;

	return pOut;
}

/*
 * Code ported from Irrlicht: http://irrlicht.sourceforge.net/
 */
kmVec2* kmVec2RotateBy(kmVec2* pOut, const kmVec2* pIn,
      const kmScalar degrees, const kmVec2* center)
{
   kmScalar x, y;
   const kmScalar radians = kmDegreesToRadians(degrees);
   const kmScalar cs = cosf(radians);
   const kmScalar sn = sinf(radians);

   pOut->x = pIn->x - center->x;
   pOut->y = pIn->y - center->y;

   x = pOut->x * cs - pOut->y * sn;
   y = pOut->x * sn + pOut->y * cs;

   pOut->x = x + center->x;
   pOut->y = y + center->y;

   return pOut;
}

kmScalar kmVec2DegreesBetween(const kmVec2* v1, const kmVec2* v2) {
	kmVec2 t1, t2;
	kmScalar cross;
	kmScalar dot;

	if(kmVec2AreEqual(v1, v2)) {
		return 0.0;
	}
	
	kmVec2Normalize(&t1, v1);
	kmVec2Normalize(&t2, v2);
	
	cross = kmVec2Cross(&t1, &t2);
	dot = kmVec2Dot(&t1, &t2);

	/*
	 * acos is only defined for -1 to 1. Outside the range we 
	 * get NaN even if that's just because of a floating point error
	 * so we clamp to the -1 - 1 range
	 */

	if(dot > 1.0) dot = 1.0;
	if(dot < -1.0) dot = -1.0;

	return kmRadiansToDegrees(atan2(cross, dot));
}

kmScalar kmVec2DistanceBetween(const kmVec2* v1, const kmVec2* v2) {
	kmVec2 diff;
	kmVec2Subtract(&diff, v2, v1);
	return fabs(kmVec2Length(&diff));
}

kmVec2* kmVec2MidPointBetween(kmVec2* pOut, const kmVec2* v1, const kmVec2* v2) {
	kmVec2 sum;
    kmVec2Add(&sum, v1, v2);
    pOut->x = sum.x / 2.0f;
    pOut->y = sum.y / 2.0f;

	return pOut;
}

kmVec2* kmVec2Reflect(kmVec2* pOut, const kmVec2* pIn, const kmVec2* normal) {
	kmVec2 tmp;
	kmVec2Scale(&tmp, normal, 2.0f * kmVec2Dot(pIn, normal));
	kmVec2Subtract(pOut, pIn, &tmp);

	return pOut;
}

void kmVec2Swap(kmVec2* pA, kmVec2* pB) {
  kmScalar x = pA->x;
  kmScalar y = pA->y;
  pA->x = pB->x;
  pA->y = pB->y;
  pB->x = x;
  pB->y = y;
}
