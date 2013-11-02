
package kazmath;

import java.nio.FloatBuffer;

public class jkazmath {

    static {
        System.loadLibrary("Jkazmath");
    }
    
    public static int KM_PLANE_LEFT     =   0;
    public static int KM_PLANE_RIGHT    =   1;
    public static int KM_PLANE_BOTTOM   =   2;
    public static int KM_PLANE_TOP      =   3;
    public static int KM_PLANE_NEAR     =   4;
    public static int KM_PLANE_FAR      =   5;
    
    public static int POINT_INFRONT_OF_PLANE    =   0;
    public static int POINT_BEHIND_PLANE        =   1;
    public static int POINT_ON_PLANE            =   2;
    
    //vec4
    public static native FloatBuffer kmVec4Fill(FloatBuffer pOut, float x, float y, float z, float w);
    public static native FloatBuffer kmVec4Add(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2);
    public static native float kmVec4Dot(FloatBuffer pV1, FloatBuffer pV2);
    public static native float kmVec4Length(FloatBuffer pIn);
    public static native float kmVec4LengthSq(FloatBuffer pIn);
    public static native FloatBuffer kmVec4Lerp(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2, float t);
    public static native FloatBuffer kmVec4Normalize(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmVec4Scale(FloatBuffer pOut, FloatBuffer pIn, float s);
    public static native FloatBuffer kmVec4Subtract(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2);
    public static native FloatBuffer kmVec4Transform(FloatBuffer pOut, FloatBuffer pV, FloatBuffer pM);
    public static native FloatBuffer kmVec4TransformArray(FloatBuffer pOut, int outStride,
			FloatBuffer pV, int vStride, FloatBuffer pM, int count);
    public static native int kmVec4AreEqual(FloatBuffer p1, FloatBuffer p2);
    public static native FloatBuffer kmVec4Assign(FloatBuffer pOut, FloatBuffer pIn);
    
    // ray
    public static native void kmRay2Fill(FloatBuffer ray, float px, float py, float vx, float vy);
    public static native boolean kmRay2IntersectLineSegment(FloatBuffer ray, FloatBuffer p1, FloatBuffer p2, FloatBuffer intersection);
    public static native boolean kmRay2IntersectTriangle(FloatBuffer ray, FloatBuffer p1, FloatBuffer p2, FloatBuffer p3, FloatBuffer intersection, FloatBuffer normal_out, FloatBuffer distance);
    public static native boolean kmRay2IntersectBox(FloatBuffer ray, FloatBuffer p1, FloatBuffer p2, FloatBuffer p3, FloatBuffer p4, FloatBuffer intersection, FloatBuffer normal_out);
    public static native boolean kmRay2IntersectCircle(FloatBuffer ray, FloatBuffer centre, float radius, FloatBuffer intersection);
    
    // quaternion
//    public static native FloatBuffer  kmQuaternionConjugate(FloatBuffer pOut,  FloatBuffer pIn);
    public static native float 	kmQuaternionDot( FloatBuffer q1,  FloatBuffer q2); 
    public static native FloatBuffer kmQuaternionExp(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionIdentity(FloatBuffer pOut);
    public static native FloatBuffer kmQuaternionInverse(FloatBuffer pOut, FloatBuffer pIn);
    public static native int kmQuaternionIsIdentity( FloatBuffer pIn);
    public static native float kmQuaternionLength( FloatBuffer pIn);
    public static native float kmQuaternionLengthSq( FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionLn(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionMultiply(FloatBuffer pOut,  FloatBuffer q1,  FloatBuffer q2);
    public static native FloatBuffer kmQuaternionNormalize(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionRotationAxisAngle(FloatBuffer pOut,  FloatBuffer pV, float angle);
    public static native FloatBuffer kmQuaternionRotationMatrix(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionRotationPitchYawRoll(FloatBuffer pOut, float pitch, float yaw, float roll);
    public static native FloatBuffer kmQuaternionSlerp(FloatBuffer pOut,  FloatBuffer q1,  FloatBuffer q2, float t);
    public static native void kmQuaternionToAxisAngle( FloatBuffer pIn, FloatBuffer pVector, FloatBuffer pAngle);
    public static native FloatBuffer kmQuaternionScale(FloatBuffer pOut,  FloatBuffer pIn, float s);
    public static native FloatBuffer kmQuaternionAssign(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionAdd(FloatBuffer pOut,  FloatBuffer pQ1,  FloatBuffer pQ2);
    public static native FloatBuffer kmQuaternionRotationBetweenVec3(FloatBuffer pOut,  FloatBuffer vec1,  FloatBuffer vec2,  FloatBuffer fallback);
    public static native FloatBuffer kmQuaternionMultiplyVec3(FloatBuffer pOut,  FloatBuffer q,  FloatBuffer v);
    
    // plane 
    public static native float kmPlaneDot(FloatBuffer pP, FloatBuffer pV);
    public static native float kmPlaneDotCoord(FloatBuffer pP, FloatBuffer pV);
    public static native float kmPlaneDotNormal(FloatBuffer pP, FloatBuffer pV);
    public static native FloatBuffer kmPlaneFromPointAndNormal(FloatBuffer pOut, FloatBuffer pPoint, FloatBuffer pNormal);
    public static native FloatBuffer kmPlaneFromPoints(FloatBuffer pOut, FloatBuffer p1, FloatBuffer p2, FloatBuffer p3);
    public static native FloatBuffer kmPlaneIntersectLine(FloatBuffer pOut, FloatBuffer pP, FloatBuffer pV1, FloatBuffer pV2);
    public static native FloatBuffer kmPlaneNormalize(FloatBuffer pOut, FloatBuffer pP);
    public static native FloatBuffer kmPlaneScale(FloatBuffer pOut, FloatBuffer pP, float s);
    public static native int kmPlaneClassifyPoint(FloatBuffer pIn,FloatBuffer pP);
    public static native FloatBuffer kmPlaneExtractFromMat4(FloatBuffer pOut, FloatBuffer pIn, int row);
    public static native FloatBuffer kmPlaneGetIntersection(FloatBuffer pOut, FloatBuffer p1, FloatBuffer p2, FloatBuffer p3);

    // mat3
    public static native FloatBuffer kmMat3Fill(FloatBuffer pOut,  FloatBuffer pMat);
    public static native FloatBuffer kmMat3Adjugate(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmMat3Identity(FloatBuffer pOut);
    public static native FloatBuffer kmMat3Inverse(FloatBuffer pOut, FloatBuffer pM);
    public static native int kmMat3IsIdentity( FloatBuffer pIn);
    public static native FloatBuffer kmMat3Transpose(FloatBuffer pOut,  FloatBuffer pIn);
    public static native float kmMat3Determinant( FloatBuffer pIn);
    public static native FloatBuffer kmMat3Multiply(FloatBuffer pOut,  FloatBuffer pM1,  FloatBuffer pM2);
    public static native FloatBuffer kmMat3ScalarMultiply(FloatBuffer pOut,  FloatBuffer pM,  float pFactor);
    public static native FloatBuffer kmMat3RotationAxisAngle(FloatBuffer pOut,  FloatBuffer axis, float radians);
    public static native FloatBuffer kmMat3RotationToAxisAngle(FloatBuffer pAxis, FloatBuffer radians,  FloatBuffer pIn);
    public static native FloatBuffer kmMat3Assign(FloatBuffer pOut,  FloatBuffer pIn);
    public static native int kmMat3AreEqual( FloatBuffer pM1,  FloatBuffer pM2);
    public static native FloatBuffer kmMat3GetUpVec3(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmMat3GetRightVec3(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmMat3GetForwardVec3(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmMat3RotationX(FloatBuffer pOut,  float radians);
    public static native FloatBuffer kmMat3RotationY(FloatBuffer pOut,  float radians);
    public static native FloatBuffer kmMat3RotationZ(FloatBuffer pOut,  float radians);
    public static native FloatBuffer kmMat3Rotation(FloatBuffer pOut,  float radians);
    public static native FloatBuffer kmMat3Scaling(FloatBuffer pOut,  float x,  float y);
    public static native FloatBuffer kmMat3Translation(FloatBuffer pOut,  float x,  float y);
    public static native FloatBuffer kmMat3RotationQuaternion(FloatBuffer pOut,  FloatBuffer pIn);

    
    // aabb
    public static native int kmAABBContainsPoint( FloatBuffer pBox,  FloatBuffer pPoint);
    public static native FloatBuffer  kmAABBAssign(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer  kmAABBScale(FloatBuffer pOut,  FloatBuffer pIn, float s);
    public static native boolean kmAABBIntersectsTriangle(FloatBuffer box,  FloatBuffer p1,  FloatBuffer p2,  FloatBuffer p3);
    public static native int kmAABBContainsAABB( FloatBuffer container,  FloatBuffer to_check);
    public static native float kmAABBDiameterX( FloatBuffer aabb);
    public static native float kmAABBDiameterY( FloatBuffer aabb);
    public static native float kmAABBDiameterZ( FloatBuffer aabb);

    // mat4
    public static native FloatBuffer kmMat4Fill(FloatBuffer pOut, FloatBuffer pMat);
    public static native FloatBuffer kmMat4Identity(FloatBuffer mat);
    public static native FloatBuffer kmMat4Inverse(FloatBuffer pOut, FloatBuffer pM);
    public static native  int kmMat4IsIdentity(FloatBuffer pIn);
    public static native FloatBuffer kmMat4Transpose(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmMat4Multiply(FloatBuffer out,FloatBuffer mat1,FloatBuffer mat2);
    public static native FloatBuffer kmMat4Assign(FloatBuffer pOut, FloatBuffer pIn);
    public static native int kmMat4AreEqual(FloatBuffer pM1, FloatBuffer pM2);
    public static native FloatBuffer kmMat4RotationX(FloatBuffer mat, float rad);
    public static native FloatBuffer kmMat4RotationY(FloatBuffer mat, float rad);
    public static native FloatBuffer kmMat4RotationZ(FloatBuffer mat, float rad);
    public static native FloatBuffer kmMat4RotationYawPitchRoll(FloatBuffer mat, float x, float y, float z);
    public static native FloatBuffer kmMat4RotationQuaternion(FloatBuffer pOut, FloatBuffer pQ);
    public static native FloatBuffer kmMat4RotationTranslation(FloatBuffer pOut, FloatBuffer rotation,  FloatBuffer translation);
    public static native FloatBuffer kmMat4Scaling(FloatBuffer pOut, float  x, float  y, float  z);
    public static native FloatBuffer kmMat4Translation(FloatBuffer mat, float x, float y, float z);
    public static native FloatBuffer  kmMat4GetUpVec3(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer  kmMat4GetRightVec3(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmMat4PerspectiveProjection(FloatBuffer projection, float fov, float aspect, float near, float far);
    public static native FloatBuffer kmMat4OrthographicProjection(FloatBuffer pOut, float left, float right, float bottom, float top, float nearVal, float farVal);
    public static native FloatBuffer kmMat4LookAt(FloatBuffer view,FloatBuffer eye,FloatBuffer centre,FloatBuffer up);
    public static native FloatBuffer kmMat4RotationAxisAngle(FloatBuffer pOut,  FloatBuffer axis, float radians);
    public static native FloatBuffer kmMat4ExtractRotation(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmMat4ExtractPlane(FloatBuffer pOut, FloatBuffer pIn, int plane);
    public static native FloatBuffer  kmMat4RotationToAxisAngle(FloatBuffer pAxis, float radians, FloatBuffer pIn);
    public static native FloatBuffer  kmMat4GetForwardVec3RH(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer  kmMat4GetForwardVec3LH(FloatBuffer pOut, FloatBuffer pIn);

    // vec3
    public static native FloatBuffer kmVec3Fill(FloatBuffer v, float x, float y, float z);
    public static native float kmVec3Length(FloatBuffer in);
    public static native float kmVec3LengthSq(FloatBuffer in);
    public static native FloatBuffer kmVec3Normalize(FloatBuffer out,FloatBuffer in);
    public static native FloatBuffer kmVec3Cross(FloatBuffer out,FloatBuffer p1,FloatBuffer p2);
    public static native float kmVec3Dot(FloatBuffer pV1, FloatBuffer pV2);
    public static native FloatBuffer kmVec3Add(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2);
    public static native FloatBuffer kmVec3Subtract(FloatBuffer out,FloatBuffer p1,FloatBuffer p2);
    public static native FloatBuffer kmVec3Transform(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pM);
    public static native FloatBuffer kmVec3TransformNormal(FloatBuffer pOut, FloatBuffer pV, FloatBuffer pM);
    public static native FloatBuffer kmVec3TransformCoord(FloatBuffer pOut, FloatBuffer pV, FloatBuffer pM);
    public static native FloatBuffer kmVec3Scale(FloatBuffer pOut, FloatBuffer pIn, float s); 
    public static native int kmVec3AreEqual(FloatBuffer p1, FloatBuffer p2);
    public static native FloatBuffer kmVec3InverseTransform(FloatBuffer pOut, FloatBuffer pV, FloatBuffer pM);
    public static native FloatBuffer kmVec3InverseTransformNormal(FloatBuffer pOut, FloatBuffer pVect, FloatBuffer pM);
    public static native FloatBuffer kmVec3Assign(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmVec3Zero(FloatBuffer pOut);
    public static native FloatBuffer kmVec3GetHorizontalAngle(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmVec3RotationToDirection(FloatBuffer pOut, FloatBuffer pIn, FloatBuffer forwards);


    

    
    jkazmath() {
        
    }
}
