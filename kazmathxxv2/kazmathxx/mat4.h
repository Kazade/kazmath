#include <kazmath/mat4.h>

namespace kazmath {

class Mat4 : public kmMat4 {
    Mat4() {
        Identity();
    }
    
    Mat4(const Mat4& rhs) {
        Assign(rhs);
    }

    const Mat4& operator=(const Mat4& rhs) {
        return Assign(rhs);
    }

    const Mat4& operator*=(const Mat4& rhs) {
        return Multiply(rhs);
    }

    const Mat4& Identity() {
        kmMat4Identity(this);
        return *this;
    }

    const Mat4& Multiply(const Mat4& rhs) {
       kmMat4Multiply(this, this, &rhs);
       return *this;
    }
    
    const Mat4& Transpose() {
        kmMat4Transpose(this, this);
        return *this;
    }
    
    bool IsIdentity() const {
        return kmMat4IsIdentity(this);
    }
    
    const Mat4& Assign(const Mat4& rhs) {
        kmMat4Assign(this, &rhs);
        return *this;
    }
};

Mat4 operator*(const Mat4& lhs, const Mat4& rhs) {
    Mat4 result = lhs;
    return result.Multiply(rhs);
}

}
