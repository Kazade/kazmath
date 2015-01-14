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

            inline bool operator==( const vec4& rhs ) const
            {
                return (kmVec4AreEqual( this, &rhs) != 0);
            }

            inline bool operator!=(const vec4& rhs ) const
            {
                return (kmVec4AreEqual( this, &rhs) == 0);
            }

            inline vec4 operator+( const vec4& rhs ) const
            {
                return vec4( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
            }

            inline vec4 operator+( kmScalar rhs ) const
            {
                return vec4( x + rhs, y + rhs, z + rhs, w + rhs );
            }

            inline vec4& operator+=( const vec4& rhs )
            {
                x += rhs.x;
                y += rhs.y;
                z += rhs.z;
                w += rhs.w;
                return *this;
            }

            inline vec4& operator+=( kmScalar rhs )
            {
                x += rhs;
                y += rhs;
                z += rhs;
                w += rhs;
                return *this;
            }

            inline vec4 operator-( const vec4& rhs ) const
            {
                return vec4( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
            }

            inline vec4 operator-( kmScalar rhs ) const
            {
                return vec4( x - rhs, y - rhs, z - rhs, w - rhs );
            }

            inline vec4& operator-=( const vec4& rhs )
            {
                x -= rhs.x;
                y -= rhs.y;
                z -= rhs.z;
                w -= rhs.w;
                return *this;
            }

            inline vec4& operator-=( kmScalar rhs )
            {
                x -= rhs;
                y -= rhs;
                z -= rhs;
                w -= rhs;
                return *this;
            }

            inline vec4 operator*( const vec4& rhs ) const
            {
                return vec4( x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w );
            }

            inline vec4 operator*( kmScalar rhs ) const
            {
                return vec4( x * rhs, y * rhs, z * rhs, w * rhs );
            }

            inline vec4& operator*=( const vec4& rhs )
            {
                this->x *= rhs.x;
                this->y *= rhs.y;
                this->z *= rhs.z;
                this->w *= rhs.w;
                return *this;
            }

            inline vec4& operator*=( kmScalar rhs  )
            {
                this->x *= rhs;
                this->y *= rhs;
                this->z *= rhs;
                this->w *= rhs;
                return *this;
            }

            inline vec4 operator/( const vec4& rhs ) const
            {
                if( rhs.x && rhs.y && rhs.z && rhs.w ){
                    return vec4( x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w );
                }else{
                    return vec4( x, y, z, w );
                }
            }

            inline vec4 operator/( kmScalar rhs ) const
            {
                if( rhs != 0.0 ){
                    return vec4( x / rhs, y / rhs, z / rhs, w / rhs );
                }else{
                    return vec4( x, y, z, w );
                }
            }

            inline vec4& operator/=( const vec4& rhs )
            {
                if( rhs.x && rhs.y && rhs.z && rhs.w ){
                    x /= rhs.x;
                    y /= rhs.y;
                    z /= rhs.z;
                    w /= rhs.w;
                }
                return *this;
            }

            inline vec4& operator/=( kmScalar rhs  )
            {
                if( rhs ){
                    x /= rhs;
                    y /= rhs;
                    z /= rhs;
                    w /= rhs;
                }
                return *this;
            }

            inline vec4 operator-(){ 
                return vec4( -x, -y, -z, -w );
            }

            inline kmScalar dot( const vec4& rhs )
            {
                return kmVec4Dot(this, &rhs);
            }
	};
	

	
	///< Multiply with scalar
	inline const vec4 operator*(const kmScalar lhs, const vec4& rhs)
	{
        return rhs * lhs;
	}


	///< Transform through matrix	
	inline const vec4 operator*(const kmMat4& lhs, const vec4& rhs)
	{
		vec4 result;
		kmVec4Transform(&result, &rhs, &lhs);
		return result;
	}
	
} //end of namespace km

#endif

