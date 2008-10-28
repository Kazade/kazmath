#ifndef KAZMATHXX_H_INCLUDED
#define KAZMATHXX_H_INCLUDED

#include "kazmath.h"

namespace kazmathxx {

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
};

struct Mat4 : public kmMat4 {
    Mat4() {
        for (int i = 0; i < 16; ++i) {
            this->mat[i] = 0.0f;
        }
    }

    Mat4(const kmMat4& m) {
        for (int i = 0; i < 16; ++i) {
            this->mat[i] = v.m[i];
        }
    }

    const Mat4& operator=(const kmMat4& m) {
        for (int i = 0; i < 16; ++i) {
            this->mat[i] = v.m[i];
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
            this->mat[i] = v.m[i];
        }
    }

    const Mat3& operator=(const kmMat3& m) {
        for (int i = 0; i < 9; ++i) {
            this->mat[i] = v.m[i];
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

        return *this;
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
    Quaternion():
    w(0.0f),
    x(0.0f),
    y(0.0f),
    z(0.0f) {

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
