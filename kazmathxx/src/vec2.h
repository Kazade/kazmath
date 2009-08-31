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

#ifndef _KAZMATHXX_VEC2_H
#define _KAZMATHXX_VEC2_H

#include <kazmath/vec2.h>

namespace km
{
	class vec2 : public kmVec2
	{
		public:
			///< Constructors
			vec2(const kmScalar _x, const kmScalar _y)
			{
			    x = _x;
			    y = _y;
			}
			vec2()
			{
			    x = kmScalar(0.0);
			    y = kmScalar(0.0);
			}
			
			///< Returns the length of the vector
			const kmScalar length() const
			{
				return kmVec2Length(this);
			}
			
			///< Returns the square of the length of the vector
			const kmScalar lengthSq() const
			{
				return kmVec2LengthSq(this);
			}
			
			
			///< Returns the vector passed in set to unit length
			const vec2 normalize() const
			{
				vec2 result;
				kmVec2Normalize(&result, this);
				return result;
			}
			
			///< Transform the Vector
			const vec2 transform(const kmMat3& mat) const
			{
				vec2 result;
				kmVec2Transform(&result, this, &mat);
				return result;
			}

			///< Transforms a 3D vector by a given matrix, projecting the result back into w = 1.
			const vec2 transformCoord(const kmMat3& mat) const
			{
				vec2 result;
				kmVec2TransformCoord(&result, this, &mat);
				return result;
			}
	};
	
	///< Vector addition
	inline const vec2 operator+(const vec2& lhs, const vec2& rhs)
	{
		vec2 result;
		kmVec2Add(&result, &lhs, &rhs);
		return result;
	};

	///< Vector subtraction
	inline const vec2 operator-(const vec2& lhs, const vec2& rhs)
	{
		vec2 result;
		kmVec2Subtract(&result, &lhs, &rhs);
		return result;
	};
	
	///< Dot product - which is the cosine of the angle between the two vectors multiplied by their lengths
	inline const float operator*(const vec2& lhs, const vec2& rhs)
	{
		return kmVec2Dot(&lhs, &rhs);
	};
	
	///< Multiply with scalar
	inline const vec2 operator*(const kmScalar lhs, const vec2& rhs)
	{
		vec2 result;
		kmVec2Scale(&result, &rhs, lhs);
		return result;
	};

	///< Multiply with scalar	
	inline const vec2 operator*(const vec2& lhs, const kmScalar rhs)
	{
		vec2 result;
		kmVec2Scale(&result, &lhs, rhs);
		return result;
	};
	
	///< Transform through matrix	
	inline const vec2 operator*(const kmMat3& lhs, const vec2& rhs)
	{
		vec2 result;
		kmVec2Transform(&result, &rhs, &lhs);
		return result;
	};
	
	///< Checks for equality (with a small threshold epsilon)
    inline const bool operator==(const vec2& lhs, const vec2& rhs)
	{
		return kmVec2AreEqual(&lhs,&rhs);
	};
} //end of namespace km

#endif

