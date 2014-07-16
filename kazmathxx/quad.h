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

#ifndef _KAZMATHXX_QUAD_H
#define _KAZMATHXX_QUAD_H

#include "vec2.h"
#include "aabb2.h" 
#include "boundvec2.h" 

#include <kazmathxx/aabb2.h>

namespace km
{
    class quad 
    {
        public: 
            vec2    pos;
            vec2    u; 
            vec2    v;

        public:
            inline quad() : pos(.0,.0), u( 1.0f, .0f ) , v( .0f, 1.0f ){};
            inline quad( const quad& other ) : pos( other.pos), u( other.u), v( other.v ){};
            inline quad( const vec2 &pos, const vec2 &u, const vec2 &v ) : pos(pos), u(u), v(v) {};
            inline quad( AABB2 &rect ) : pos( rect.min ), u( rect.max.x - rect.min.x, 0 ), v( 0, rect.max.y - rect.min.y ) {};  
            inline quad( float x, float y, float z, float w ) : pos( x, y ), u(z,0), v(0,w){};
            inline ~quad() {};

            inline const quad& operator=( const quad& other ){ 
                pos = other.pos;
                u = other.u; 
                v = other.v;
                return *this;
            }

            inline bool operator==( const quad& other ) const { 
                return ( pos == other.pos && 
                         u == other.u && 
                         v == other.v ); 
            } 

            inline bool operator!=( const quad& other ) const {
                return !( *this == other ); 
            }

            inline void center( vec2 &out ) const {
                out = pos + ( u + v ) / 2.0f; 
            }

            inline AABB2 aaBBox() const
            {
                // vertical edges of AABB2 
                vec2 max( pos );
                vec2 min( pos );
                
                if( u.x < .0f ){ 
                    min.x += u.x; 
                }else{ 
                    max.x += u.x; 
                }
                if( v.x < .0f ){ 
                    min.x += v.x; 
                }else{ 
                    max.x += v.x; 
                } 
                
                if( u.y < .0f ){ 
                    min.y += u.y; 
                }else{ 
                    max.y += u.y; 
                }
                if( v.y < .0f ){ 
                    min.y += v.y; 
                }else{ 
                    max.y += v.y; 
                }

                return AABB2( min, max );
            }
            
            inline bool contains( const vec2 &inVec ) const
            {
                // barycentric coordinates 
                float t = ( v.y * ( inVec.x - pos.x ) + v.x * ( pos.y - inVec.y ) ) / ( v.y * u.x - v.x * u.y ); 
                float s = ( inVec.y - pos.y - t * u.y ) / v.y; 

                if( t < .0f || t > 1.0f || s < .0f || s > 1.0f ){ 
                    return false; 
                }
                return true;
            }

            
            inline bool overlaps( const AABB2 &other ) const
            { 

                // check if the other box is completely enclosed by this quad, its
                // center will be containes
                if( contains( other.centre() ) ){ 
                    return true; 
                }
                
                AABB2 b = aaBBox(); 
     
                // if not 'completely' contained, 
                // check if any segment of the rect intersects this box 
                if( ortoXLineWithBox( other.min.x, b.min.x, b.max.x, other.min.y, other.max.y ) ||  
                    ortoXLineWithBox( other.max.x, b.min.x, b.max.x, other.min.y, other.max.y ) ||
                    ortoYLineWithBox( other.min.y, b.min.y, b.max.y, other.min.x, other.max.x ) || 
                    ortoYLineWithBox( other.max.y, b.min.y, b.max.y, other.min.x, other.max.x ) ){
                        return true; 
                }

                return false;
            }
            
            

            inline bool overlaps( const quad &other ) const{
                if( contains( other.pos ) || 
                    contains( other.pos + other.u ) || 
                    contains( other.pos + other.v ) || 
                    contains( other.pos + other.u + other.v ) || 
                    other.contains( pos ) || 
                    other.contains( pos + u ) || 
                    other.contains( pos + v ) || 
                    other.contains( pos + u + v ) )
                {
                    return true; 
                }

                // check if any pair of segments segmentIntersectsSegment ( named A, B, C and D for 
                // each quad ) 
                boundvec2 tA( pos.x, pos.y, u.x, u.y );                         // thisA
                boundvec2 oA( other.pos.x, other.pos.y, other.u.x, other.u.y ); // otherA
                if( tA.segmentIntersectsSegment( oA ) ){
                    return true; 
                }

                boundvec2 oB( other.pos.x, other.pos.y, other.v.x, other.v.y ); 
                if( tA.segmentIntersectsSegment( oB ) ){ 
                    return true;
                }

                boundvec2 oC( other.pos.x + other.u.x, other.pos.y + other.u.y, 
                              other.u.x + other.v.x, other.u.y + other.v.y );
                if( tA.segmentIntersectsSegment( oC ) ){
                    return true;
                }
                
                boundvec2 oD( other.pos.x + other.v.x, other.pos.y + other.v.y, 
                              other.u.x + other.v.x, other.u.y + other.v.y );
                if( tA.segmentIntersectsSegment( oD ) ){
                    return true;
                }


                boundvec2 tB( pos.x, pos.y, v.x, v.y ); 
                if( tB.segmentIntersectsSegment(oA) || tB.segmentIntersectsSegment(oB) || tB.segmentIntersectsSegment(oC) || tB.segmentIntersectsSegment(oD) ){ 
                    return true;
                }

                boundvec2 tC( pos.x + u.x, pos.y + u.y, 
                              u.x + v.x, u.y + v.y );
                if( tC.segmentIntersectsSegment(oA) || tC.segmentIntersectsSegment(oB) || tC.segmentIntersectsSegment(oC) || tC.segmentIntersectsSegment(oD) ){ 
                    return true;
                }
                
                boundvec2 tD( pos.x + v.x, pos.y + v.y, 
                              u.x + v.x, u.y + v.y );
                if( tB.segmentIntersectsSegment(oA) || tB.segmentIntersectsSegment(oB) || tB.segmentIntersectsSegment(oC) || tB.segmentIntersectsSegment(oD) ){ 
                    return true;
                }
              

                return false;
            } 

        protected: 
            inline bool ortoIntersect( float ox, float oy,      // origin 
                                       float p,                 // expected point of intersection on X 
                                       float m,                 // slope from origin
                                       float miny, float maxy   // y bounds 
                                       ) const
            {
                float y = oy + ( p - ox ) * m;
                if( y >= miny && y < maxy ){ 
                    return true; 
                }
                return false;
            }

            inline bool ortoXLineWithBox( float t, 
                                          float vMin, float vMax,  // 'margin' 
                                          float bMin, float bMax ) const // bound on the other axis
            {
                if( t >= vMin && t <= vMax ){
                    if( ortoIntersect( pos.x, pos.y, t, u.y / u.x, bMin, bMax ) || 
                        ortoIntersect( pos.x, pos.y, t, v.y / v.x, bMin, bMax ) || 
                        ortoIntersect( pos.x + u.x, pos.y + u.y, t, v.y / v.x, bMin, bMax ) || 
                        ortoIntersect( pos.x + v.x, pos.y + v.y, t, u.y / u.x, bMin, bMax ) ){
                            return true; 
                    }
                }
                return false;
            }

            inline bool ortoYLineWithBox( float t, 
                                          float vMin, float vMax,  // 'margin' 
                                          float bMin, float bMax ) const // bound on the other axis
            {
                if( t >= vMin && t <= vMax ){
                    if( ortoIntersect( pos.y, pos.x, t, u.x / u.y, bMin, bMax ) || 
                        ortoIntersect( pos.y, pos.x, t, v.x / v.y, bMin, bMax ) || 
                        ortoIntersect( pos.y + u.y, pos.x + u.x, t, v.x / v.y, bMin, bMax ) || 
                        ortoIntersect( pos.y + v.y, pos.x + v.x, t, u.x / u.y, bMin, bMax ) ){
                            return true; 
                    }
                }
                return false;
            }


        };
	
	
} //end of namespace km

#endif