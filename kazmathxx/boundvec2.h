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

#ifndef _KAZMATHXX_BOUNDVEC2_H
#define _KAZMATHXX_BOUNDVEC2_H

#include "vec2.h"
#include "../kazmath/ray2.h"

namespace km
{
	class boundvec2 
	{
        public:
            vec2    pos; 
            vec2    vec;

			///< Constructors
			boundvec2( const kmScalar posx, const kmScalar posy,
                       const kmScalar vx, const kmScalar vy ) : 
            pos( posx, posy ),
            vec( vx , vy )
			{
			}

			boundvec2() : pos( 0.0, 0.0 ), vec( 0.0, 0.0 )
			{
			}

            inline bool operator==( const boundvec2& rhs ) const
            {
                return ( pos == rhs.pos && vec == rhs.vec );  
            }

            inline bool operator!=(const boundvec2& rhs ) const
            {
                return ( pos != rhs.pos || vec != rhs.vec );  
            }

            inline bool intersects( const boundvec2& other, 
                                    kmScalar &outTThis, 
                                    kmScalar &outTOther,
                                    vec2     &outPoint ) const 
            { 
                
                return (kmLine2WithLineIntersection( &pos, &vec, &(other.pos), &(other.vec), 
                                                     &outTThis, &outTOther, &outPoint ) == KM_TRUE);
            }

            inline bool intersects( const boundvec2 &other ){
                kmScalar tt;
                kmScalar to;
                vec2 pt;
                return intersects( other, tt, to, pt );
            }

            inline bool intersects( const boundvec2 &other, vec2 &outPoint ){ 
                kmScalar tt;
                kmScalar to;
                return intersects( other, tt, to, outPoint );
            }

            inline bool segmentIntersectsSegment( const boundvec2 &other,
                                                  kmScalar &outTThis, 
                                                  kmScalar &outTOther,
                                                  vec2     &outPoint) 
            {
                kmScalar tt;
                kmScalar to; 
                vec2     pt; 
                
                if( !intersects( other, tt, to, pt ) ){
                    return false;
                }

                if((0.0 <= tt) && (tt <= 1.0) && (0.0 <= to) && (to <= 1.0)) {
                    outTThis = tt;
                    outTOther = to;
                    outPoint = pt; 
                    return true;    
                }
                return false;
            }

            inline bool segmentIntersectsSegment( const boundvec2 &other ){ 
                kmScalar tt;
                kmScalar to; 
                vec2     pt; 
                return segmentIntersectsSegment( other, tt, to, pt );
            }

            inline bool segmentIntersectsSegment( const boundvec2 &other,
                                                 vec2 &outPoint ){ 
                kmScalar tt;
                kmScalar to; 
                return segmentIntersectsSegment( other, tt, to, outPoint );
            }
	};
	
} //end of namespace km

#endif