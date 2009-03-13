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
			vec2(const kmScalar& x, const kmScalar& y) : _x(x), _y(y) {};
			
			///< Returns the length of the vector
			kmScalar length()
			{
				return kmVec2Length(this);
			}
			
			///< Returns the square of the length of the vector
			kmScalar lengthSq()
			{
				return kmVec2LengthSq(this);
			}
			
			
			///< Returns the vector passed in set to unit length
			void normalize()
			{
				kmVec2Normalize(this,this);
			}
			
			///< Transform the Vector
			void transform(const &kmMat3 mat)
			{
				kmVec2Transform(this, this, mat)
			}

			///< Transforms a 3D vector by a given matrix, projecting the result back into w = 1.
			void transformCoord(const &kmMat3 mat)
			{
				kmVec2TransformCoord(this, this, mat)
			}
	};
	
	///< Vector addition
	const vec2 operator+(const vec2& lhs, const vec2& rhs)
	{
		vec2 result;
		kmVec2Add(&result, &lhs, &rhs);
		return result;
	};

	///< Vector subtraction
	const vec2 operator-(const vec2& lhs, const vec2& rhs)
	{
		vec2 result;
		kmVec2Subtract(&result, &lhs, &rhs);
		return result;
	};
	
	///< Dot product - which is the cosine of the angle between the two vectors multiplied by their lengths
	const float operator*(const vec2& lhs, const vec2& rhs)
	{
		return kmVec2Dot(&lhs, &rhs);
	};
	
	///< Multiply with scalar
	const vec2 operator*(const float lhs, const vec2& rhs)
	{
		vec2 result;
		kmVec2Scale(&result, &rhs, lhs);
		return result;
	};

	///< Multiply with scalar	
	const vec2 operator*(const vec2& lhs, const float rhs)
	{
		vec2 result;
		kmVec2Scale(&result, &lhs, rhs);
		return result;
	};
	
	///< Checks for equality (with a small threshold epsilon)
	const bool operator==(const vec2& lhs, const vec2& rhs)
	{
		return kmVec2AreEqual(&lhs,&rhs);
	};
} //end of namespace km

#endif

