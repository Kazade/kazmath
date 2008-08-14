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

#include "vec3.h"
#include "vec4.h"
#include "plane.h"

kmScalar kmPlaneDot(const kmPlane* pP, const kmVec4* pV)
{
	//a*x + b*y + c*z + d*w

	return (pP->a * pV->x +
			pP->b * pV->y +
			pP->c * pV->z +
			pP->d * pV->w);
}

kmScalar kmPlaneDotCoord(const kmPlane* pP, const kmVec3* pV)
{
	return (pP->a * pV->x +
			pP->b * pV->y +
			pP->c * pV->z + pP->d);
}

kmScalar kmPlaneDotNormal(const kmPlane* pP, const kmVec3* pV)
{
	return (pP->a * pV->x +
			pP->b * pV->y +
			pP->c * pV->z);
}

kmPlane* kmPlaneFromPointNormal(kmPlane* pOut, const kmVec3* pPoint, const kmVec3* pNormal)
{
	/*
		Planea = Nx
		Planeb = Ny
		Planec = Nz
		Planed = −N⋅P
	*/


	pOut->a = pNormal->x;
	pOut->b = pNormal->y;
	pOut->c = pNormal->z;
	pOut->d = -kmVec3Dot(pNormal, pPoint);

	return pOut;
}

kmPlane* kmPlaneFromPoints(kmPlane* pOut, const kmVec3* p1, const kmVec3* p2, const kmVec3* p3)
{
	/*
	v = (B − A) × (C − A)
	n = 1⁄|v| v
	Outa = nx
	Outb = ny
	Outc = nz
	Outd = −n⋅A
	*/

	kmVec3 n, v1, v2;
	kmVec3Subtract(&v1, p2, p1); //Create the vectors for the 2 sides of the triangle
	kmVec3Subtract(&v2, p3, p1);
	kmVec3Cross(&n, &v1, &v2); //Use the cross product to get the normal

	kmVec3Normalize(&n, &n); //Normalize it and assign to pOut->m_N

	pOut->a = n.x;
	pOut->b = n.y;
	pOut->c = n.z;
	pOut->d = kmVec3Dot(kmVec3Scale(&n, &n, -1.0), p1);

	return pOut;
}

kmVec3*  kmPlaneIntersectLine(kmVec3* pOut, const kmPlane* pP, const kmVec3* pV1, const kmVec3* pV2)
{
	/*
		n = (Planea, Planeb, Planec)
		d = V − U
		Out = U − d⋅(Pd + n⋅U)⁄(d⋅n) [iff d⋅n ≠ 0]
	*/

	assert(0);

	kmVec3 d;
	kmVec3Subtract(&d, pV2, pV1); //Get the direction vector


	//TODO: Continue here!
	/*if (fabs(kmVec3Dot(&pP->m_N, &d)) > kmEpsilon)
	{
		//If we get here then the plane and line are parallel (i.e. no intersection)
		pOut = nullptr; //Set to nullptr

		return pOut;
	} */
}

kmPlane* kmPlaneNormalize(kmPlane* pOut, const kmPlane* pP)
{
	kmVec3 n;
	n.x = pP->a;
	n.y = pP->b;
	n.z = pP->c;

	kmScalar l = 1.0 / kmVec3Length(&n); //Get 1/length
	kmVec3Normalize(&n, &n); //Normalize the vector and assign to pOut

	pOut->a = n.x;
	pOut->b = n.y;
	pOut->c = n.z;

	pOut->d = pP->d * l; //Scale the D value and assign to pOut

	return pOut;
}

kmPlane* kmPlaneScale(kmPlane* pOut, const kmPlane* pP, kmScalar s)
{
	assert(0);
}

POINT_CLASSIFICATION kmPlaneClassifyPoint(const kmPlane* pIn, const kmVec3* pP)
{
   // This function will determine if a point is on, in front of, or behind
   // the plane.  First we store the dot product of the plane and the point.
   float distance = pIn->a * pP->x + pIn->b * pP->y + pIn->c * pP->z + pIn->d;

   // Simply put if the dot product is greater than 0 then it is infront of it.
   // If it is less than 0 then it is behind it.  And if it is 0 then it is on it.
   if(distance > 0.001) return POINT_INFRONT_OF_PLANE;
   if(distance < -0.001) return POINT_BEHIND_PLANE;

   return POINT_ON_PLANE;
}

