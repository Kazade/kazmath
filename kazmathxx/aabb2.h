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

#ifndef _KAZMATHXX_AABB2D_H
#define _KAZMATHXX_AABB2D_H

#include "vec2.h"
#include <kazmath/aabb2.h>

namespace km
{
	class AABB2
	{
    public:
        vec2 min;
        vec2 max;

    public:
        AABB2() : min( -.5f, -.5f ), max( .5f, .5f ){};
        AABB2( const vec2 &minV, const vec2 &maxV ) : min( minV ), max( maxV ){ 
            sanitize(); 
        }
        
        AABB2( kmScalar x, kmScalar y, 
                kmScalar width, kmScalar height ) : min( x, y ), 
                                                  max( x + width, y + height ){ 
            sanitize();
        }

        inline bool operator==( const AABB2& rhs ) const
        {
            return ( min == rhs.min && max == rhs.max );
        }

        inline bool operator!=(const AABB2& rhs ) const
        {
            return ( min != rhs.min || max != rhs.max );
        }

        inline void sanitize()
        {
            kmScalar tmp; 
            if( min.x > max.x ){ 
                tmp = max.x; 
                max.x = min.x; 
                min.x = tmp; 
            }

            if( min.y > max.y ){ 
                tmp = max.y;
                max.y = min.y;
                min.y = tmp;
            }
        }

        inline vec2 centre() const {
            return ( min + max ) * 0.5;
        }

        inline vec2 size() const { 
            return max - min; 
        }

        inline bool overlaps( const AABB2 &other ) const
        {
            if( other.min.x > max.x ){
                return false;
            }
            if( other.max.x < min.x ){
                return false;
            }

            if( other.min.y > max.y ){
                return false;
            }
            if( other.max.y < min.y ){
                return false;
            }

            return true;
        }

        inline bool contains( const vec2 &point ) const
        {
            if( point.x >= min.x && point.x <= (max.x)  &&
                point.y >= min.y && point.y <= (max.y) ){ 
                return true; 
            }
            return false;
        }

        inline void scale( const vec2 &factor )
        {
            vec2 pivot;
            scale( factor, pivot );
        } 

        inline void scale( const vec2 &factor, const vec2 pivot )
        {
            min = (( min - pivot ) * factor ) + pivot; 
            max = (( max - pivot ) * factor ) + pivot;
        }

        inline void translate( const vec2 &displacement )
        {
            min += displacement; 
            max += displacement; 
        }

        /* 
           Expands the box by the amount specified mantaining its centre in the same
           position.
           (it's like scaling from the centre but more efficient ( and less verbose ) 
        */ 
        inline void expand( const vec2 &amount )
        {
            vec2 halfAmount( amount * 0.5 );
            min -= halfAmount; 
            max += halfAmount; 
        }

        /* 
           Expands the box to be of targetSize, mantaining its centre in the same
           position. 
        */ 
        inline void expandToSize( const vec2 &targetSize )
        {
            vec2 halfDiff( (targetSize - size()) * 0.5 );  
            min -= halfDiff;
            max += halfDiff; 
        }
	};
	
	
} //end of namespace km

#endif