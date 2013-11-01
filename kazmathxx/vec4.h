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

#ifndef _KAZMATHXX_VEC4_H
#define _KAZMATHXX_VEC4_H

#include <kazmath/vec4.h>


namespace km
{
	class vec4 : public kmVec4
	{
		public:
			///< Constructors
			vec4(const kmScalar _x, const kmScalar _y, const kmScalar _z, const kmScalar _w)
			{
                 x = _x;
                 y = _y;
                 z = _z;	
                 w = _w;		
			}
			vec4()
			{
                 x = kmScalar(0.0);
                 y = kmScalar(0.0);
                 z = kmScalar(0.0);
                 w = kmScalar(0.0);
			}
			
			///< Returns the length of the vector
			const kmScalar length() const
			{
				return kmVec4Length(this);
			}
			
			///< Returns the square of the length of the vector
			const kmScalar lengthSq() const
			{
				return kmVec4LengthSq(this);
			}
			
			
			///< Returns the vector passed in set to unit length
			const vec4 normalize() const
			{
				vec4 result;
				kmVec4Normalize(&result,this);
				return result;
			}
			
			///< Transform the Vector
			const vec4 transform(const kmMat4& mat)
			{
				vec4 result;
				kmVec4Transform(&result,this, &mat);
				return result;
			}

			static const vec4 lerp(const kmVec4& pV1, const kmVec4& pV2, kmScalar t)
			{
				vec4 result;
				kmVec4Lerp(&result, &pV1, &pV2, t);
				return result;
			}
	};
	
	///< Vector addition
	inline const vec4 operator+(const vec4& lhs, const vec4& rhs)
	{
		vec4 result;
		kmVec4Add(&result, &lhs, &rhs);
		return result;
	};

	///< Vector subtraction
	inline const vec4 operator-(const vec4& lhs, const vec4& rhs)
	{
		vec4 result;
		kmVec4Subtract(&result, &lhs, &rhs);
		return result;
	};
	
	///< Dot product - which is the cosine of the angle between the two vectors multiplied by their lengths
	inline const float operator*(const vec4& lhs, const vec4& rhs)
	{
		return kmVec4Dot(&lhs, &rhs);
	};
	
	///< Multiply with scalar
	inline const vec4 operator*(const kmScalar lhs, const vec4& rhs)
	{
		vec4 result;
		kmVec4Scale(&result, &rhs, lhs);
		return result;
	};

	///< Multiply with scalar	
	inline const vec4 operator*(const vec4& lhs, const kmScalar rhs)
	{
		vec4 result;
		kmVec4Scale(&result, &lhs, rhs);
		return result;
	};
	
	///< Transform through matrix	
	inline const vec4 operator*(const kmMat4& lhs, const vec4& rhs)
	{
		vec4 result;
		kmVec4Transform(&result, &rhs, &lhs);
		return result;
	};
	
	///< Checks for equality (with a small threshold epsilon)
	inline const bool operator==(const vec4& lhs, const vec4& rhs)
	{
		return kmVec4AreEqual(&lhs,&rhs);
	};
} //end of namespace km

#endif

