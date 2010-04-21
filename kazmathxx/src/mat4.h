/*
Copyright (c) 2009, Luke Benstead, Carsten Haubold
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

#ifndef _KAZMATHXX_MAT4_H
#define _KAZMATHXX_MAT4_H

#include <kazmath/mat4.h>
#include <kazmath/utility.h>
#include "vec3.h"

namespace km
{
	class mat4 : public kmMat4
	{
	public:
		/// Constructors
		mat4()
		{
			kmMat4Identity(this);
		}
		
		mat4(const kmScalar* pIn)
		{
			kmMat4Fill(this,pIn);
		}
		
		void identity()
		{
			kmMat4Identity(this);
		}
		
		const mat4 inverse() const
		{
			mat4 result = *this;
			kmMat4Inverse(&result, &result);
			return result;
		}
		
		const bool isIdentity() const
		{
			return kmMat4IsIdentity(this);
		}
		
		const mat4 transpose() const
		{
			mat4 result = *this;
			kmMat4Transpose(&result, &result);
			return result;
		}
		
		static const mat4 rotationAxis(const kmVec3& axis, const kmScalar radians)
		{
			mat4 result;
			kmMat4RotationAxisAngle(&result, &axis, radians);
			return result;
		}
		
		static const mat4 rotationAxis(const kmScalar degrees, const kmScalar axis_x, const kmScalar axis_y, const kmScalar axis_z)
		{
			mat4 result;
			vec3 axis(axis_x, axis_y, axis_z);
			kmMat4RotationAxisAngle(&result, &axis, kmDegreesToRadians(degrees));
			return result;
		}
		
		static const mat4 rotationX(const kmScalar radians)
		{
			mat4 result;
			kmMat4RotationX(&result, radians);
			return result;
		}
		
		static const mat4 rotationY(const kmScalar radians)
		{
			mat4 result;
			kmMat4RotationY(&result, radians);
			return result;
		}
		
		static const mat4 rotationZ(const kmScalar radians)
		{
			mat4 result;
			kmMat4RotationZ(&result, radians);
			return result;
		}
		
		static const mat4 rotationPitchYawRoll(const kmScalar pitch, const kmScalar yaw, const kmScalar roll)
		{
			mat4 result;
			kmMat4RotationPitchYawRoll(&result, pitch, yaw, roll);
			return result;
		}
		
		static const mat4 rotationQuaternion(const kmQuaternion& pQ)
		{
			mat4 result;
			kmMat4RotationQuaternion(&result, &pQ);
			return result;
		}
		
		static const mat4 scaling(const kmScalar x, const kmScalar y, const kmScalar z)
		{
			mat4 result;
			kmMat4Scaling(&result, x,y,z);
			return result;
		}
		
		static const mat4 translation(const kmScalar x, const kmScalar y, const kmScalar z)
		{
			mat4 result;
			kmMat4Translation(&result, x,y,z);
			return result;
		}
		
		const vec3 getUpVec3() const
		{
			vec3 result;
			kmMat4GetUpVec3(&result, this);
			return result;
		}
		
		const vec3 getRightVec3() const
		{
			vec3 result;
			kmMat4GetRightVec3(&result, this);
			return result;
		}
		
		const vec3 getForwardVec3() const
		{
			vec3 result;
			kmMat4GetForwardVec3(&result, this);
			return result;
		}
		
		static const mat4 perspectiveProjection(const kmScalar fovY, const kmScalar aspect, const kmScalar zNear, const kmScalar zFar)
		{
			mat4 result;
			kmMat4PerspectiveProjection(&result, fovY, aspect, zNear, zFar);
			return result;
		}
		
		static const mat4 orthographicProjection(const kmScalar left, const kmScalar right, const kmScalar bottom, const kmScalar top, const kmScalar nearVal, const kmScalar farVal)
		{
			mat4 result;
			kmMat4OrthographicProjection(&result, left, right, bottom, top, nearVal, farVal);
			return result;
		}
		
		static const mat4 lookAt(const kmVec3& pEye, const kmVec3& pCenter, const kmVec3& pUp)
		{
			mat4 result;
			kmMat4LookAt(&result, &pEye, &pCenter, &pUp);
			return result;
		}
	};
	
	///< Matrix multiplication
	inline const mat4 operator*(const mat4& lhs, const mat4& rhs)
	{
		mat4 result;
		kmMat4Multiply(&result, &lhs, &rhs);
		return result;
	};
	
	///< Checks for equality (with a small threshold epsilon)
	inline const bool operator==(const mat4& lhs, const mat4& rhs)
	{
		return kmMat4AreEqual(&lhs,&rhs);
	};
}

#endif
