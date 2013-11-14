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

#ifndef _KAZMATHXX_MAT3_H
#define _KAZMATHXX_MAT3_H

#include <kazmath/mat3.h>
#include "vec3.h"

namespace km
{
	class mat3 : public kmMat3
	{
		/// Constructors
		mat3()
		{
			kmMat3Identity(this);
		}
		
		mat3(const kmScalar* pIn)
		{
			kmMat3Fill(this,pIn);
		}
		
		void identity()
		{
			kmMat3Identity(this);
		}
		
		const mat3 inverse() const
		{
			mat3 result = *this;
			kmMat3Inverse(&result, &result);
			return result;
		}
		
		const bool isIdentity() const
		{
			return kmMat3IsIdentity(this);
		}
		
		const mat3 transpose() const
		{
			mat3 result = *this;
			kmMat3Transpose(&result, &result);
			return result;
		}
		
		static const mat3 rotation(const kmScalar radians)
		{
			mat3 result;
			kmMat3Rotation(&result, radians);
			return result;
		}
		
		static const mat3 scaling(const kmScalar x, const kmScalar y)
		{
			mat3 result;
			kmMat3Scaling(&result, x,y);
			return result;
		}
		
		static const mat3 translation(const kmScalar x, const kmScalar y)
		{
			mat3 result;
			kmMat3Translation(&result, x,y);
			return result;
		}
		
		/* still missing:
		kmMat3* kmMat3Adjugate(kmMat3* pOut, const kmMat3* pIn);
		kmScalar kmMat3Determinant(const kmMat3* pIn);
		kmMat3* kmMat3ScalarMultiply(kmMat3* pOut, const kmMat3* pM, const kmScalar pFactor);
		*/
	};
	
	///< Matrix multiplication
	inline const mat3 operator*(const mat3& lhs, const mat3& rhs)
	{
		mat3 result;
		kmMat3Multiply(&result, &lhs, &rhs);
		return result;
    }
	
	///< Checks for equality (with a small threshold epsilon)
	inline const bool operator==(const mat3& lhs, const mat3& rhs)
	{
		return kmMat3AreEqual(&lhs,&rhs);
    }
}
