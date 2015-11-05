
package kazmath;

import java.nio.FloatBuffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class jkazmath {

    static {
        System.loadLibrary("Jkazmath");
    }
    
    public static FloatBuffer kmCreateFloatBuffer(int size) {
		return ByteBuffer.allocateDirect(size*4).order(ByteOrder.nativeOrder()).asFloatBuffer();
	}
	
	public static FloatBuffer kmCreateKmVec3() {
		return kmCreateFloatBuffer(3);
	}
	
	public static FloatBuffer kmCreateKmVec4() {
		return kmCreateFloatBuffer(4);
	}
	
	public static FloatBuffer kmCreateKmMat3() {
		return kmCreateFloatBuffer(9);
	}

	public static FloatBuffer kmCreateKmMat4() {
		return kmCreateFloatBuffer(16);
	}
	
	public static FloatBuffer kmCreateKmRay2() {
		return kmCreateFloatBuffer(4);
	}

	public static FloatBuffer kmCreateKmRay3() {
		return kmCreateFloatBuffer(6);
	}
		
	public static FloatBuffer kmCreateKmPlane() {
		return kmCreateFloatBuffer(4);
	}
	
	public static FloatBuffer kmCreateKmQuaternion() {
		return kmCreateFloatBuffer(4);
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
    

    //util
    public static native float kmLerp(float x, float y, float factor);
    public static native boolean kmAlmostEqual(float lhs, float rhs);
    public static native float kmClamp(float x, float min, float max);
    public static native float kmSQR(float s);
    public static native float kmDegreesToRadians(float degrees);
    public static native float kmRadiansToDegrees(float radians);
    
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
    
    // ray2
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
    public static native FloatBuffer kmPlaneFill(FloatBuffer pOut, float a, float b, float c, float d);
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
    public static native FloatBuffer kmPlaneFromNormalAndDistance(FloatBuffer plane, FloatBuffer normal, float dist);

    // mat3
    public static native FloatBuffer kmMat3Fill(FloatBuffer pOut,  FloatBuffer pMat);
    public static native FloatBuffer kmMat3Adjugate(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer kmMat3Identity(FloatBuffer pOut);
    public static native FloatBuffer kmMat3Inverse(FloatBuffer pOut, FloatBuffer pM);
    public static native int kmMat3IsIdentity( FloatBuffer pIn);
    public static native FloatBuffer kmMat3Transpose(FloatBuffer pOut,  FloatBuffer pIn);
    public static native float kmMat3Determinant( FloatBuffer pIn);
    public static native FloatBuffer kmMat3MultiplyMat3(FloatBuffer pOut,  FloatBuffer pM1,  FloatBuffer pM2);
    public static native FloatBuffer kmMat3MultiplyScalar(FloatBuffer pOut,  FloatBuffer pM,  float pFactor);
    public static native FloatBuffer kmMat3FromRotationAxisAngle(FloatBuffer pOut,  FloatBuffer axis, float radians);
    public static native FloatBuffer kmMat3ExtractRotationAxisAngle(FloatBuffer pIn, FloatBuffer pAxis, FloatBuffer radians);
    public static native FloatBuffer kmMat3AssignMat3(FloatBuffer pOut,  FloatBuffer pIn);
    public static native int kmMat3AreEqual( FloatBuffer pM1,  FloatBuffer pM2);
    public static native FloatBuffer kmMat3ExtractUpVec3(FloatBuffer pIn,  FloatBuffer pOut);
    public static native FloatBuffer kmMat3ExtractRightVec3(FloatBuffer pIn,  FloatBuffer pOut);
    public static native FloatBuffer kmMat3ExtractForwardVec3(FloatBuffer pIn,  FloatBuffer pOut);
    public static native FloatBuffer kmMat3FromRotationX(FloatBuffer pOut,  float radians);
    public static native FloatBuffer kmMat3FromRotationY(FloatBuffer pOut,  float radians);
    public static native FloatBuffer kmMat3FromRotationZ(FloatBuffer pOut,  float radians);
    public static native FloatBuffer kmMat3FromScaling(FloatBuffer pOut,  float x,  float y);
    public static native FloatBuffer kmMat3FromTranslation(FloatBuffer pOut,  float x,  float y);
    public static native FloatBuffer kmMat3FromRotationQuaternion(FloatBuffer pOut,  FloatBuffer pIn);

    
    // aabb
    public static native FloatBuffer kmAABBInitialize(FloatBuffer pBox, FloatBuffer centre, float width, float height, float depth);
    public static native int kmAABBContainsPoint( FloatBuffer pBox,  FloatBuffer pPoint);
    public static native FloatBuffer  kmAABBAssign(FloatBuffer pOut,  FloatBuffer pIn);
    public static native FloatBuffer  kmAABBScale(FloatBuffer pOut,  FloatBuffer pIn, float s);
    public static native boolean kmAABBIntersectsAABB(FloatBuffer box, FloatBuffer to_check);
    public static native boolean kmAABBIntersectsTriangle(FloatBuffer box,  FloatBuffer p1,  FloatBuffer p2,  FloatBuffer p3);
    public static native int kmAABBContainsAABB( FloatBuffer container,  FloatBuffer to_check);
    public static native float kmAABBDiameterX( FloatBuffer aabb);
    public static native float kmAABBDiameterY( FloatBuffer aabb);
    public static native float kmAABBDiameterZ( FloatBuffer aabb);
    public static native FloatBuffer kmAABBCentre(FloatBuffer aabb, FloatBuffer pOut); 

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
    public static native FloatBuffer kmMat4GetUpVec3(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmMat4GetRightVec3(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmMat4PerspectiveProjection(FloatBuffer projection, float fov, float aspect, float near, float far);
    public static native FloatBuffer kmMat4OrthographicProjection(FloatBuffer pOut, float left, float right, float bottom, float top, float nearVal, float farVal);
    public static native FloatBuffer kmMat4LookAt(FloatBuffer view,FloatBuffer eye,FloatBuffer centre,FloatBuffer up);
    public static native FloatBuffer kmMat4RotationAxisAngle(FloatBuffer pOut,  FloatBuffer axis, float radians);
    public static native FloatBuffer kmMat4ExtractRotation(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmMat4ExtractPlane(FloatBuffer pOut, FloatBuffer pIn, int plane);
    public static native FloatBuffer kmMat4RotationToAxisAngle(FloatBuffer pAxis, float radians, FloatBuffer pIn);
    public static native FloatBuffer kmMat4GetForwardVec3RH(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmMat4GetForwardVec3LH(FloatBuffer pOut, FloatBuffer pIn);

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
    public static native FloatBuffer kmVec3Lerp(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2, float t);

    
    public static native FloatBuffer kmQuaternionSubtract(FloatBuffer pOut, FloatBuffer pQ1, FloatBuffer pQ2);
    public static native int kmQuaternionAreEqual(FloatBuffer p1, FloatBuffer p2);
    public static native FloatBuffer kmQuaternionFill(FloatBuffer pOut, float x, float y, float z, float w);
    public static native FloatBuffer kmQuaternionGetUpVec3(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionGetRightVec3(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionGetForwardVec3RH(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmQuaternionGetForwardVec3LH(FloatBuffer pOut, FloatBuffer pIn);
    public static native float kmQuaternionGetPitch(FloatBuffer q);
    public static native float kmQuaternionGetYaw(FloatBuffer q);
    public static native float kmQuaternionGetRoll(FloatBuffer q);
    public static native FloatBuffer kmRay3Fill(FloatBuffer ray, float px, float py, float pz, float vx, float vy, float vz);
    public static native FloatBuffer kmRay3FromPointAndDirection(FloatBuffer ray, FloatBuffer point, FloatBuffer direction);
    public static native boolean kmRay3IntersectPlane(FloatBuffer pOut, FloatBuffer ray,  FloatBuffer plane);
    public static native FloatBuffer kmVec3MultiplyMat3(FloatBuffer pOut, FloatBuffer pV,  FloatBuffer pM);
    public static native FloatBuffer kmVec3MultiplyMat4(FloatBuffer pOut, FloatBuffer pV,  FloatBuffer pM);
    public static native FloatBuffer kmVec3ProjectOnToPlane(FloatBuffer pOut, FloatBuffer point,  FloatBuffer plane);
    public static native FloatBuffer kmVec4MultiplyMat4(FloatBuffer pOut, FloatBuffer pV,  FloatBuffer pM);
    public static native FloatBuffer kmVec2Fill(FloatBuffer pOut, float x, float y);
    public static native float kmVec2Length(FloatBuffer pIn); ///< Returns the length of the vector
    public static native float kmVec2LengthSq(FloatBuffer pIn); ///< Returns the square of the length of the vector
    public static native FloatBuffer kmVec2Normalize(FloatBuffer pOut, FloatBuffer pIn); ///< Returns the vector passed in set to unit length
    public static native FloatBuffer kmVec2Lerp(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2, float t);
    public static native FloatBuffer kmVec2Add(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2); ///< Adds 2 vectors and returns the result
    public static native float kmVec2Dot(FloatBuffer pV1, FloatBuffer pV2); /** Returns the Dot product which is the cosine of the angle between the two vectors multiplied by their lengths */
    public static native float kmVec2Cross(FloatBuffer pV1, FloatBuffer pV2);
    public static native FloatBuffer kmVec2Subtract(FloatBuffer pOut, FloatBuffer pV1, FloatBuffer pV2); ///< Subtracts 2 vectors and returns the result
    public static native FloatBuffer kmVec2Transform(FloatBuffer pOut, FloatBuffer pV1,  FloatBuffer pM); /** Transform the Vector */
    public static native FloatBuffer kmVec2TransformCoord(FloatBuffer pOut, FloatBuffer pV,  FloatBuffer pM); ///<Transforms a 2D vector by a given matrix, projecting the result back into w = 1.
    public static native FloatBuffer kmVec2Scale(FloatBuffer pOut, FloatBuffer pIn, float s); ///< Scales a vector to length s
    public static native int	kmVec2AreEqual(FloatBuffer p1, FloatBuffer p2); ///< Returns 1 if both vectors are equal
    public static native FloatBuffer kmVec2Assign(FloatBuffer pOut, FloatBuffer pIn);
    public static native FloatBuffer kmVec2RotateBy(FloatBuffer pOut, FloatBuffer pIn, float degrees, FloatBuffer center); ///<Rotates the point anticlockwise around a center by an amount of degrees.
    public static native float kmVec2DegreesBetween(FloatBuffer v1, FloatBuffer v2);
    public static native float kmVec2DistanceBetween(FloatBuffer v1, FloatBuffer v2);
    public static native FloatBuffer kmVec2MidPointBetween(FloatBuffer pOut, FloatBuffer v1, FloatBuffer v2);










    
    jkazmath() {
        
    }
}
