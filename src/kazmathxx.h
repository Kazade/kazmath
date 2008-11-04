/*
Copyright (c) 2008, Luke Benstead.
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

#ifndef KAZMATHXX_H_INCLUDED
#define KAZMATHXX_H_INCLUDED

#include "kazmath.h"

namespace kazmathxx {

/**
* \Brief -  A public class that inherits kmVec3
*           and provides basic C++ operators to make it
*           behave like a built in type. There are no other methods
*           aside from the standard +, +=, -, -=, *, *=, /, /=
*           assignment operator, copy constructor and default constructor
* \Detailed -
*            Usage:
*                Vec3 myVec1(1.0f, 0.0f, 0.0f);
*                Vec3 myVec2(0.0f, 2.0f, 0.0f);
*                Vec3 cross = myVec1 * myVec2;
*                kmVec3Normalize(&cross);
*                Vec3 add = myVec1 + myVec2;
*                Vec3 scale = myVec1 * 10.0f;
*                scale = 5.0f * myVec1; //TODO!
*                float s = kmVec3Dot(&myVec1, &myVec2);
*/
struct Vec3 : public kmVec3 {
    Vec3() {
        this->x = this->y = this->z = 0.0f;
    }

    Vec3(const kmVec3& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    const Vec3& operator=(const kmVec3& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;

        return *this;
    }
    
    const Vec3 operator+(const kmVec3& v) const {
    	Vec3 result;
    	kmVec3Fill(&result, x + v.x, y + v.y, z + v.z);
    	return result;
	}
	
	const Vec3& operator+=(const kmVec3& v) {
		this->x = this->x + v.x;
		this->y = this->y + v.y;
		this->z = this->z + v.z;
		
		return *this;
	}
	
    const Vec3 operator-(const kmVec3& v) const {
    	Vec3 result;
    	kmVec3Fill(&result, x - v.x, y - v.y, z - v.z);
    	return result;
	}

	const Vec3& operator-=(const kmVec3& v) {
		this->x = this->x - v.x;
		this->y = this->y - v.y;
		this->z = this->z - v.z;
		
		return *this;
	}
		
    const Vec3 operator/(const kmScalar s) const {
    	Vec3 result;
    	kmScalar l = 1.0f / s;
    	kmVec3Fill(&result, x * l, y * l, z * l);
    	return result;
	}
	
    const Vec3& operator/=(const kmScalar s) {
    	float l = 1.0f / s;
		this->x = this->x * l;
		this->y = this->y * l;
		this->z = this->z * l;
    	return *this;
	}	
	
    /**
    * Multiplication by a scalar
    */
    const Vec3 operator*(const kmScalar s) const {
    	Vec3 result;
    	kmVec3Fill(&result, x * s, y * s, z * s);
    	return result;
	}	
	
    /** 
    * I've decided to make multiplication of 2 vectors to 
    * mean the cross product
    */
    const Vec3 operator*(const kmVec3& v) const {
        kmVec3 result;
	    result.x = (y * v.z) - (z * v.y);
	    result.y = (z * v.x) - (x * v.z);
	    result.z = (x * v.y) - (y * v.x);

        return result;
    }

    /**
    * Cross product then assign the result
    */
    const Vec3& operator*=(const kmVec3& v) {
        kmVec3 result;

	    result.x = (y * v.z) - (z * v.y);
	    result.y = (z * v.x) - (x * v.z);
	    result.z = (x * v.y) - (y * v.x);

        this->x = result.x;
        this->y = result.y;
        this->z = result.z;

        return *this;        
    }

    const Vec3& operator*=(const float s) {
		this->x = this->x * s;
		this->y = this->y * s;
		this->z = this->z * s;
    	return *this;
	}	
};

struct Vec2 : public kmVec2 {
    Vec2() {
        this->x = this->y = 0.0f;
    }

    Vec2(const kmVec2& v) {
        this->x = v.x;
        this->y = v.y;
    }

    const Vec2& operator=(const kmVec2& v) {
        this->x = v.x;
        this->y = v.y;

        return *this;
    }

    const Vec2 operator+(const kmVec2& v) const {
    	Vec2 result;
    	kmVec2Fill(&result, x + v.x, y + v.y);
    	return result;
	}
	
	const Vec2& operator+=(const kmVec2& v) {
		this->x = this->x + v.x;
		this->y = this->y + v.y;		
		return *this;
	}
	
    const Vec2 operator-(const kmVec2& v) const {
    	Vec2 result;
    	kmVec2Fill(&result, x - v.x, y - v.y);
    	return result;
	}

	const Vec2& operator-=(const kmVec2& v) {
		this->x = this->x - v.x;
		this->y = this->y - v.y;		
		return *this;
	}
		
    const Vec2 operator/(const kmScalar s) const {
    	Vec2 result;
    	kmScalar l = 1.0f / s;
    	kmVec2Fill(&result, x * l, y * l);
    	return result;
	}
	
    const Vec2& operator/=(const kmScalar s) {
    	float l = 1.0f / s;
		this->x = this->x * l;
		this->y = this->y * l;
    	return *this;
	}	
	
    /**
    * Multiplication by a scalar
    */
    const Vec2 operator*(const kmScalar s) const {
    	Vec2 result;
    	kmVec2Fill(&result, x * s, y * s);
    	return result;
	}	
	
    const Vec2& operator*=(const float s) {
		this->x = this->x * s;
		this->y = this->y * s;
    	return *this;
	}	
};

struct Mat4 : public kmMat4 {
    Mat4() {
        for (int i = 0; i < 16; ++i) {
            this->mat[i] = 0.0f;
        }
    }

    Mat4(const kmMat4& m) {
        for (int i = 0; i < 16; ++i) {
            this->mat[i] = m.mat[i];
        }
    }

    const Mat4& operator=(const kmMat4& m) {
        for (int i = 0; i < 16; ++i) {
            this->mat[i] = m.mat[i];
        }

        return *this;
    }
};

struct Mat3 : public kmMat3 {
    Mat3() {
        for (int i = 0; i < 9; ++i) {
            this->mat[i] = 0.0f;
        }
    }

    Mat3(const kmMat3& m) {
        for (int i = 0; i < 9; ++i) {
            this->mat[i] = m.mat[i];
        }
    }

    const Mat3& operator=(const kmMat3& m) {
        for (int i = 0; i < 9; ++i) {
            this->mat[i] = m.mat[i];
        }

        return *this;
    }
};

/**
* Describes a plane, publicly inherits kmPlane but implements
* object oriented methods for C++
*/
struct Plane : public kmPlane {
    Plane() {
        this->a = 0.0f;
        this->b = 0.0f;
        this->c = 0.0f;
        this->d = 0.0f;
    }

    Plane(const kmPlane& p) {
        this->a = p.a;
        this->b = p.b;
        this->c = p.c;
        this->d = p.d;
    }

    const Plane& operator=(const kmPlane& p) {
        this->a = p.a;
        this->b = p.b;
        this->c = p.c;
        this->d = p.d;

        return *this;
    }
};

struct Quaternion : public kmQuaternion {
    Quaternion() {
        this->w = 0.0f;
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    Quaternion(const kmQuaternion q) {
        this->w = q.w;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;
    }

    const Quaternion& operator=(const kmQuaternion& q) {
        this->w = q.w;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;

        return *this;
    }

};

}
#endif // KAZMATHXX_H_INCLUDED
