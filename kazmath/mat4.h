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

#ifndef MAT4_H_INCLUDED
#define MAT4_H_INCLUDED

#include "utility.h"

struct kmVec3;
struct kmMat3;
struct kmQuaternion;
struct kmPlane;

/*
A 4x4 matrix

      | 0   4   8  12 |
mat = | 1   5   9  13 |
      | 2   6  10  14 |
      | 3   7  11  15 |
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kmMat4 {
	kmScalar mat[16];
} kmMat4;

/**
 * Fills a kmMat4 structure with the values from a 16
 * element array of kmScalars
 * @Params pOut - A pointer to the destination matrix
 * 		   pMat - A 16 element array of kmScalars
 * @Return Returns pOut so that the call can be nested
 */
kmMat4* kmMat4Fill(kmMat4* pOut, const kmScalar* pMat);


/**
 * Sets pOut to an identity matrix returns pOut
 * @Params pOut - A pointer to the matrix to set to identity
 * @Return Returns pOut so that the call can be nested
 */
kmMat4* kmMat4Identity(kmMat4* pOut);

/**
 * Calculates the inverse of pM and stores the result in
 * pOut.
 * @Return Returns NULL if there is no inverse, else pOut
 */
kmMat4* kmMat4Inverse(kmMat4* pOut, const kmMat4* pM);

/**
 * Returns KM_TRUE if pIn is an identity matrix
 * KM_FALSE otherwise
 */
int kmMat4IsIdentity(const kmMat4* pIn);

/**
 * Sets pOut to the transpose of pIn, returns pOut
 */
kmMat4* kmMat4Transpose(kmMat4* pOut, const kmMat4* pIn);

/**
 * Multiplies pM1 with pM2, stores the result in pOut, returns pOut
 */
kmMat4* kmMat4Multiply(kmMat4* pOut, const kmMat4* pM1, const kmMat4* pM2);

/**
 * Assigns the value of pIn to pOut
 */
kmMat4* kmMat4Assign(kmMat4* pOut, const kmMat4* pIn);
    
kmMat4* kmMat4AssignMat3(kmMat4* pOut, const struct kmMat3* pIn);

/**
 * Returns KM_TRUE if the 2 matrices are equal (approximately)
 */
int kmMat4AreEqual(const kmMat4* pM1, const kmMat4* pM2);

/**
 * Builds an X-axis rotation matrix and stores it in pOut, returns pOut
 */
kmMat4* kmMat4RotationX(kmMat4* pOut, const kmScalar radians);

/**
 * Builds a rotation matrix using the rotation around the Y-axis
 * The result is stored in pOut, pOut is returned.
 */
kmMat4* kmMat4RotationY(kmMat4* pOut, const kmScalar radians);

/**
 * Builds a rotation matrix around the Z-axis. The resulting
 * matrix is stored in pOut. pOut is returned.
 */
kmMat4* kmMat4RotationZ(kmMat4* pOut, const kmScalar radians);

/**
 * Builds a rotation matrix from pitch, yaw and roll. The resulting
 * matrix is stored in pOut and pOut is returned
 */
kmMat4* kmMat4RotationYawPitchRoll(kmMat4* pOut, const kmScalar pitch,
                                   const kmScalar yaw, const kmScalar roll);

/** Converts a quaternion to a rotation matrix,
 * the result is stored in pOut, returns pOut
 */
kmMat4* kmMat4RotationQuaternion(kmMat4* pOut, const struct kmQuaternion* pQ);

/** Build a 4x4 OpenGL transformation matrix using a 3x3 rotation matrix,
 * and a 3d vector representing a translation. Assign the result to pOut,
 * pOut is also returned.
 */
kmMat4* kmMat4RotationTranslation(kmMat4* pOut, const struct kmMat3* rotation,
                                  const struct kmVec3* translation);

/** Builds a scaling matrix */
kmMat4* kmMat4Scaling(kmMat4* pOut, const kmScalar x, const kmScalar y,
                      const kmScalar z);

/**
 * Builds a translation matrix. All other elements in the matrix
 * will be set to zero except for the diagonal which is set to 1.0
 */
kmMat4* kmMat4Translation(kmMat4* pOut, const kmScalar x, const kmScalar y,
                          const kmScalar z);

/**
 * Get the up vector from a matrix. pIn is the matrix you
 * wish to extract the vector from. pOut is a pointer to the
 * kmVec3 structure that should hold the resulting vector
 */
struct kmVec3* kmMat4GetUpVec3(struct kmVec3* pOut, const kmMat4* pIn);

/** Extract the right vector from a 4x4 matrix. The result is
 * stored in pOut. Returns pOut.
 */
struct kmVec3* kmMat4GetRightVec3(struct kmVec3* pOut, const kmMat4* pIn);

/**
 * Extract the forward vector from a 4x4 matrix. The result is
 * stored in pOut. Returns pOut.
 */
struct kmVec3* kmMat4GetForwardVec3RH(struct kmVec3* pOut, const kmMat4* pIn);
struct kmVec3* kmMat4GetForwardVec3LH(struct kmVec3* pOut, const kmMat4* pIn);

/**
 * Creates a perspective projection matrix in the
 * same way as gluPerspective
 */
kmMat4* kmMat4PerspectiveProjection(kmMat4* pOut, kmScalar fovY,
                                    kmScalar aspect, kmScalar zNear,
                                    kmScalar zFar);

/** Creates an orthographic projection matrix like glOrtho */
kmMat4* kmMat4OrthographicProjection(kmMat4* pOut, kmScalar left,
                                     kmScalar right, kmScalar bottom,
                                     kmScalar top, kmScalar nearVal,
                                     kmScalar farVal);

/**
 * Builds a translation matrix in the same way as gluLookAt()
 * the resulting matrix is stored in pOut. pOut is returned.
 */
kmMat4* kmMat4LookAt(kmMat4* pOut, const struct kmVec3* pEye,
                     const struct kmVec3* pCenter, const struct kmVec3* pUp);

/**
 * Build a rotation matrix from an axis and an angle. Result is stored in pOut.
 * pOut is returned.
 */
kmMat4* kmMat4RotationAxisAngle(kmMat4* pOut, const struct kmVec3* axis, kmScalar radians);

/**
 * Extract a 3x3 rotation matrix from the input 4x4 transformation.
 * Stores the result in pOut, returns pOut
 */
struct kmMat3* kmMat4ExtractRotationMat3(const kmMat4* pIn,
                                         struct kmMat3* pOut);
struct kmPlane* kmMat4ExtractPlane(struct kmPlane* pOut, const kmMat4* pIn,
                                   const kmEnum plane);

/**
 * Take the rotation from a 4x4 transformation matrix, and return it
 * as an axis and an angle (in radians). Returns the output axis.
 */
struct kmVec3* kmMat4RotationToAxisAngle(struct kmVec3* pAxis,
                                         kmScalar* radians, const kmMat4* pIn);
struct kmVec3* kmMat4ExtractTranslationVec3(const kmMat4* pIn,
                                            struct kmVec3* pOut);
#ifdef __cplusplus
}
#endif
#endif /* MAT4_H_INCLUDED */
