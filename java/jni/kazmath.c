
/*
* this file is public domain originally written by chris_c
* by using this source you agree to
* indemnify all authors from all damages and costs involved in the use of this code 
*/

#include "kazmath_jkazmath.h"

#include <aabb.h>
#include <vec3.h>
#include <vec4.h>
#include <mat3.h>
#include <mat4.h>
#include <quaternion.h>
#include <plane.h>
#include <ray2.h>

#include <assert.h>


// kmVec4* kmVec4Fill(kmVec4* pOut, kmScalar x, kmScalar y, kmScalar z, kmScalar w);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Fill
  (JNIEnv *e, jclass c, jobject jo, jfloat x, jfloat y, jfloat z, jfloat w)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4Fill(o,x,y,z,w);
    return jo;
}

//kmVec4* kmVec4Add(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Add
  (JNIEnv *e, jclass c, jobject jo, jobject jv1, jobject jv2)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* v1 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv1);
    kmVec4* v2 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv2);
    kmVec4Add(o,v1,v2);
    return jo;
}

//kmScalar kmVec4Dot(const kmVec4* pV1, const kmVec4* pV2);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmVec4Dot
  (JNIEnv *e, jclass c, jobject jv1, jobject jv2)
{
    kmVec4* v1 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv1);
    kmVec4* v2 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv2);
    return kmVec4Dot(v1,v2);
}

//kmScalar kmVec4Length(const kmVec4* pIn);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmVec4Length
  (JNIEnv *e, jclass c, jobject ji)
{
    kmVec4* i = (kmVec4*)(*e)->GetDirectBufferAddress(e, ji);
    return kmVec4Length(i);    
}

//kmScalar kmVec4LengthSq(const kmVec4* pIn);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmVec4LengthSq
  (JNIEnv *e, jclass c, jobject ji)
{
    kmVec4* i = (kmVec4*)(*e)->GetDirectBufferAddress(e, ji);
    return kmVec4LengthSq(i);
}

//kmVec4* kmVec4Lerp(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2, kmScalar t);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Lerp
  (JNIEnv *e, jclass c, jobject jo, jobject jv1, jobject jv2, jfloat t)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* v1 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv1);
    kmVec4* v2 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv2);
    kmVec4Lerp(o,v1,v2,t);
    return jo;    
}

//kmVec4* kmVec4Normalize(kmVec4* pOut, const kmVec4* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Normalize
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* i = (kmVec4*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec4Normalize(o,i);
    return jo;        
}

//kmVec4* kmVec4Scale(kmVec4* pOut, const kmVec4* pIn, const kmScalar s);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Scale
  (JNIEnv *e, jclass c, jobject jo, jobject ji, jfloat s)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* i = (kmVec4*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec4Scale(o,i,s);
    return jo;    
}

//kmVec4* kmVec4Subtract(kmVec4* pOut, const kmVec4* pV1, const kmVec4* pV2);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Subtract
  (JNIEnv *e, jclass c, jobject jo, jobject jv1, jobject jv2)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* v1 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv1);
    kmVec4* v2 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv2);
    kmVec4Subtract(o,v1,v2);
    return jo;    
}

//kmVec4* kmVec4Transform(kmVec4* pOut, const kmVec4* pV, const struct kmMat4* pM);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Transform
  (JNIEnv *e, jclass c, jobject jo, jobject jv, jobject jm)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* v = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmVec4Transform(o,v,m);
    return jo;    
}

//kmVec4* kmVec4TransformArray(kmVec4* pOut, unsigned int outStride,
//			const kmVec4* pV, unsigned int vStride, const struct kmMat4* pM, unsigned int count);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4TransformArray
  (JNIEnv *e, jclass c, jobject jo, jint os, jobject jv, jint vs, jobject jm, jint cnt)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* v = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmVec4TransformArray(o,os,v,vs,m,cnt);            
    return jo;
}

//int 	kmVec4AreEqual(const kmVec4* p1, const kmVec4* p2);
JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmVec4AreEqual
  (JNIEnv *e, jclass c, jobject jv1, jobject jv2)
{
    kmVec4* v1 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv1);
    kmVec4* v2 = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv2);
    return kmVec4AreEqual(v1,v2);   
}

//kmVec4* kmVec4Assign(kmVec4* pOut, const kmVec4* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec4Assign
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmVec4* o = (kmVec4*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec4* i = (kmVec4*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec4Assign(o,i);
    return jo;    
}

JNIEXPORT void JNICALL Java_kazmath_jkazmath_kmRay2Fill
  (JNIEnv *e, jclass c, jobject jo, jfloat px, jfloat py, jfloat vx, jfloat vy)
{
    kmRay2* o = (kmRay2*)(*e)->GetDirectBufferAddress(e, jo);
    kmRay2Fill(o,px,py,vx,vy);
}

//kmBool kmRay2IntersectLineSegment(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, kmVec2* intersection);
JNIEXPORT jboolean JNICALL Java_kazmath_jkazmath_kmRay2IntersectLineSegment
  (JNIEnv *e, jclass c, jobject jr, jobject jp1, jobject jp2, jobject ji)
{
    kmRay2* r = (kmRay2*)(*e)->GetDirectBufferAddress(e, jr);
    kmVec2* p1 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp1);       
    kmVec2* p2 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp2);
    kmVec2* i = (kmVec2*)(*e)->GetDirectBufferAddress(e, ji);
    return kmRay2IntersectLineSegment(r,p1,p2,i);
}

//kmBool kmRay2IntersectTriangle(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, const kmVec2* p3, kmVec2* intersection,
// kmVec2* normal_out, kmScalar* distance);
JNIEXPORT jboolean JNICALL Java_kazmath_jkazmath_kmRay2IntersectTriangle
  (JNIEnv *e, jclass c, jobject jr, jobject jp1, jobject jp2, jobject jp3, jobject ji, jobject jn, jobject jd)
{
    kmRay2* r = (kmRay2*)(*e)->GetDirectBufferAddress(e, jr);
    kmVec2* p1 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp1);       
    kmVec2* p2 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp2);
    kmVec2* p3 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp3);
    kmVec2* i = (kmVec2*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec2* n = (kmVec2*)(*e)->GetDirectBufferAddress(e, jn);
    float* d = (float*)(*e)->GetDirectBufferAddress(e, jd);
    return kmRay2IntersectTriangle(r,p1,p2,p3,i,n,d);
}

//kmBool kmRay2IntersectBox(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, const kmVec2* p3, const kmVec2* p4,
// kmVec2* intersection, kmVec2* normal_out);
JNIEXPORT jboolean JNICALL Java_kazmath_jkazmath_kmRay2IntersectBox
  (JNIEnv *e, jclass c, jobject jr, jobject jp1, jobject jp2, jobject jp3, jobject jp4, jobject ji, jobject jn)
{
    kmRay2* r = (kmRay2*)(*e)->GetDirectBufferAddress(e, jr);
    kmVec2* p1 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp1);       
    kmVec2* p2 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp2);
    kmVec2* p3 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp3);
    kmVec2* p4 = (kmVec2*)(*e)->GetDirectBufferAddress(e, jp4);
    kmVec2* i = (kmVec2*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec2* n = (kmVec2*)(*e)->GetDirectBufferAddress(e, jn);
    return kmRay2IntersectBox(r,p1,p2,p3,p4,i,n);  
}

//kmBool kmRay2IntersectCircle(const kmRay2* ray, const kmVec2 centre, const kmScalar radius, kmVec2* intersection);
JNIEXPORT jboolean JNICALL Java_kazmath_jkazmath_kmRay2IntersectCircle
  (JNIEnv *e, jclass c, jobject jr, jobject jc, jfloat rd, jobject ji)
{
    kmRay2* r = (kmRay2*)(*e)->GetDirectBufferAddress(e, jr);
    kmVec2* ct = (kmVec2*)(*e)->GetDirectBufferAddress(e, jc);
    kmVec2* i = (kmVec2*)(*e)->GetDirectBufferAddress(e, ji);
    return kmRay2IntersectCircle(r,*ct,rd,i);   
}

/*
//kmQuaternion* const kmQuaternionConjugate(kmQuaternion* pOut, const kmQuaternion* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionConjugate
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternionConjugate(o,i);
    return jo;    
}
*/

//const kmScalar 	kmQuaternionDot(const kmQuaternion* q1, const kmQuaternion* q2);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmQuaternionDot
  (JNIEnv *e, jclass c, jobject jq1, jobject jq2)
{
    kmQuaternion* q1 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq1);
    kmQuaternion* q2 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq2);
    return kmQuaternionDot(q1,q2);
}

//kmQuaternion* kmQuaternionExp(kmQuaternion* pOut, const kmQuaternion* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionExp
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternionExp(o,i);
    return jo; 
}

//kmQuaternion* kmQuaternionIdentity(kmQuaternion* pOut);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionIdentity
  (JNIEnv *e, jclass c, jobject jo)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternionIdentity(o);
    return jo;
}

//kmQuaternion* kmQuaternionInverse(kmQuaternion* pOut, const kmQuaternion* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionInverse
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternionInverse(o,i);
    return jo;    
}

//int kmQuaternionIsIdentity(const kmQuaternion* pIn);
JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmQuaternionIsIdentity
  (JNIEnv *e, jclass c, jobject ji)
{
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    return kmQuaternionIsIdentity(i);
}

//kmScalar kmQuaternionLength(const kmQuaternion* pIn);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmQuaternionLength
  (JNIEnv *e, jclass c, jobject ji)
{
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    return kmQuaternionLength(i);    
}

//kmScalar kmQuaternionLengthSq(const kmQuaternion* pIn);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmQuaternionLengthSq
  (JNIEnv *e, jclass c, jobject ji)
{
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    return kmQuaternionLengthSq(i);        
}

//kmQuaternion* kmQuaternionLn(kmQuaternion* pOut, const kmQuaternion* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionLn
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternionLn(o,i);
    return jo;    
}

//kmQuaternion* kmQuaternionMultiply(kmQuaternion* pOut, const kmQuaternion* q1, const kmQuaternion* q2);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionMultiply
  (JNIEnv *e, jclass c, jobject jo, jobject jq1, jobject jq2)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* q1 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq1);
    kmQuaternion* q2 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq2);
    kmQuaternionMultiply(o,q1,q2); 
    return jo;       
}

//kmQuaternion* kmQuaternionNormalize(kmQuaternion* pOut, const kmQuaternion* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionNormalize
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternionNormalize(o,i);
    return jo;    
}

//kmQuaternion* kmQuaternionRotationAxisAngle(kmQuaternion* pOut, const struct kmVec3* pV, kmScalar angle);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionRotationAxisAngle
  (JNIEnv *e, jclass c, jobject jo, jobject jv, jfloat a)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv); 
    kmQuaternionRotationAxisAngle(o,v,a);
    return jo;      
}

//kmQuaternion* kmQuaternionRotationMatrix(kmQuaternion* pOut, const struct kmMat3* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionRotationMatrix
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* i = (kmMat3*)(*e)->GetDirectBufferAddress(e, ji); 
    kmQuaternionRotationMatrix(o,i);
    return jo;  
}

//kmQuaternion* kmQuaternionRotationPitchYawRoll(kmQuaternion* pOut, kmScalar yaw, kmScalar pitch, kmScalar roll);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionRotationPitchYawRoll
  (JNIEnv *e, jclass c, jobject jo, jfloat p, jfloat y, jfloat r)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternionRotationPitchYawRoll(o,p,y,r);
    return jo;    
}

//kmQuaternion* kmQuaternionSlerp(kmQuaternion* pOut, const kmQuaternion* q1, const kmQuaternion* q2, kmScalar t);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionSlerp
  (JNIEnv *e, jclass c, jobject jo, jobject jq1, jobject jq2, jfloat t)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* q1 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq1);
    kmQuaternion* q2 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq2);
    kmQuaternionSlerp(o,q1,q2,t);
    return jo;    
}

//void kmQuaternionToAxisAngle(const kmQuaternion* pIn, struct kmVec3* pVector, kmScalar* pAngle);
JNIEXPORT void JNICALL Java_kazmath_jkazmath_kmQuaternionToAxisAngle
  (JNIEnv *e, jclass c, jobject ji, jobject jv, jobject ja)
{
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    kmScalar* a = (kmScalar*)(*e)->GetDirectBufferAddress(e, ja);
    kmQuaternionToAxisAngle(i,v,a);
}

//kmQuaternion* kmQuaternionScale(kmQuaternion* pOut, const kmQuaternion* pIn, kmScalar s);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionScale
  (JNIEnv *e, jclass c, jobject jo, jobject ji, jfloat s)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternionScale(o,i,s);
    return jo;    
}

//kmQuaternion* kmQuaternionAssign(kmQuaternion* pOut, const kmQuaternion* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionAssign
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* i = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternionAssign(o,i);
    return jo;    
}

//kmQuaternion* kmQuaternionAdd(kmQuaternion* pOut, const kmQuaternion* pQ1, const kmQuaternion* pQ2);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionAdd
  (JNIEnv *e, jclass c, jobject jo, jobject jq1, jobject jq2)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* q1 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq1);
    kmQuaternion* q2 = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq2);
    kmQuaternionAdd(o,q1,q2);
    return jo;        
}

//kmQuaternion* kmQuaternionRotationBetweenVec3(kmQuaternion* pOut, const struct kmVec3* vec1, const struct kmVec3* vec2, const struct kmVec3* fallback);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionRotationBetweenVec3
  (JNIEnv *e, jclass c, jobject jo, jobject jv1, jobject jv2, jobject jf)
{
    kmQuaternion* o = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* v1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv1);
    kmVec3* v2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv2);
    kmVec3* f = (kmVec3*)(*e)->GetDirectBufferAddress(e, jf);
    kmQuaternionRotationBetweenVec3(o,v1,v2,f);
    return jo;
}

//struct kmVec3* kmQuaternionMultiplyVec3(struct kmVec3* pOut, const kmQuaternion* q, const struct kmVec3* v);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmQuaternionMultiplyVec3
  (JNIEnv *e, jclass c, jobject jo, jobject jq, jobject jv)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* q = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq);
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    kmQuaternionMultiplyVec3(o,q,v);
    return jo;
}




//const kmScalar kmPlaneDot(const kmPlane* pP, const struct kmVec4* pV);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmPlaneDot
  (JNIEnv *e, jclass c, jobject jp, jobject jv)
{
    kmPlane* p = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp);
    kmVec4* v = (kmVec4*)(*e)->GetDirectBufferAddress(e, jv);
    return kmPlaneDot(p,v);
}

JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmPlaneDotCoord
  (JNIEnv *e, jclass c, jobject jp, jobject jv)
{
    kmPlane* p = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp);
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    return kmPlaneDotCoord(p,v);
}

JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmPlaneDotNormal
  (JNIEnv *e, jclass c, jobject jp, jobject jv)
{
    kmPlane* p = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp);
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    return kmPlaneDotNormal(p,v);
}

//kmPlane* const kmPlaneFromPointAndNormal(kmPlane* pOut, const struct kmVec3* pPoint, const struct kmVec3* pNormal);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmPlaneFromPointAndNormal
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jobject jn)
{
    kmPlane* o = (kmPlane*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    kmVec3* n = (kmVec3*)(*e)->GetDirectBufferAddress(e, jn);
    kmPlaneFromPointAndNormal(o,p,n);
    return jo;
}

//kmPlane* const kmPlaneFromPoints(kmPlane* pOut, const struct kmVec3* p1, const struct kmVec3* p2, const struct kmVec3* p3);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmPlaneFromPoints
  (JNIEnv *e, jclass c, jobject jo, jobject jp1, jobject jp2, jobject jp3)
{
    kmPlane* o = (kmPlane*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp1);    
    kmVec3* p2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp2);  
    kmVec3* p3 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp3);
    kmPlaneFromPoints(o,p1,p2,p3);
    return jo;  
}

//kmVec3*  const kmPlaneIntersectLine(struct kmVec3* pOut, const kmPlane* pP, const struct kmVec3* pV1, const struct kmVec3* pV2);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmPlaneIntersectLine
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jobject jv1, jobject jv2)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmPlane* p = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp);
    kmVec3* v1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv1);
    kmVec3* v2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv2);
    kmPlaneIntersectLine(o,p,v1,v2);
    return jo;
}

//kmPlane* const kmPlaneNormalize(kmPlane* pOut, const kmPlane* pP);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmPlaneNormalize
  (JNIEnv *e, jclass c, jobject jo, jobject jp)
{
    kmPlane* o = (kmPlane*)(*e)->GetDirectBufferAddress(e, jo);
    kmPlane* p = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp);    
    kmPlaneNormalize(o,p);
    return jo;
}

//kmPlane* const kmPlaneScale(kmPlane* pOut, const kmPlane* pP, kmScalar s);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmPlaneScale
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jfloat s)
{
    kmPlane* o = (kmPlane*)(*e)->GetDirectBufferAddress(e, jo);
    kmPlane* p = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp);
    kmPlaneScale(o,p,s);
    return jo;
}

//const POINT_CLASSIFICATION kmPlaneClassifyPoint(const kmPlane* pIn, const kmVec3* pP);
JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmPlaneClassifyPoint
  (JNIEnv *e, jclass c, jobject jo, jobject jv)
{
    kmPlane* o = (kmPlane*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    return kmPlaneClassifyPoint(o,v);
}

//kmPlane* kmPlaneExtractFromMat4(kmPlane* pOut, const struct kmMat4* pIn, kmInt row);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmPlaneExtractFromMat4
  (JNIEnv *e, jclass c, jobject jo, jobject ji, jint r)
{
    kmPlane* o = (kmPlane*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat4* i = (kmMat4*)(*e)->GetDirectBufferAddress(e, ji);
    kmPlaneExtractFromMat4(o,i,r);
    return jo;
}

//kmVec3* kmPlaneGetIntersection(kmVec3* pOut, const kmPlane* p1, const kmPlane* p2, const kmPlane* p3);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmPlaneGetIntersection
  (JNIEnv *e, jclass c, jobject jo, jobject jp1, jobject jp2, jobject jp3)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmPlane* p1 = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp1);
    kmPlane* p2 = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp2);
    kmPlane* p3 = (kmPlane*)(*e)->GetDirectBufferAddress(e, jp3);
    kmPlaneGetIntersection(o,p1,p2,p3);
    return jo;
}





//kmMat3* const kmMat3Fill(kmMat3* pOut, const kmScalar* pMat);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Fill
  (JNIEnv *e, jclass c, jobject jo, jobject jm)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmScalar* m = (kmScalar*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat3Fill(o,m);
    return jo;
}

//kmMat3* const kmMat3Adjugate(kmMat3* pOut, const kmMat3* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Adjugate
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* i = (kmMat3*)(*e)->GetDirectBufferAddress(e, ji);
    kmMat3Adjugate(o,i);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Identity
  (JNIEnv *e, jclass c, jobject jm)
{
    kmMat3* m = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat3Identity(m);
    return jm;
}

//kmMat3* const kmMat3Inverse(kmMat3* pOut, const kmMat3* pM);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Inverse
  (JNIEnv *e, jclass c, jobject jo, jobject jm)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);    
    kmMat3* m = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm);  
    kmMat3Inverse(o,m);
    return jo;  
}

//const int  kmMat3IsIdentity(const kmMat3* pIn);
JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmMat3IsIdentity
  (JNIEnv *e, jclass c, jobject jm)
{
    kmMat3* m = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm);
    return kmMat3IsIdentity(m);
}

//kmMat3* const kmMat3Transpose(kmMat3* pOut, const kmMat3* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Transpose
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* i = (kmMat3*)(*e)->GetDirectBufferAddress(e, ji);
    kmMat3Transpose(o,i);
    return jo;    
}

//const kmScalar kmMat3Determinant(const kmMat3* pIn);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmMat3Determinant
  (JNIEnv *e, jclass c, jobject ji)
{
    kmMat3* i = (kmMat3*)(*e)->GetDirectBufferAddress(e, ji);
    return kmMat3Determinant(i);
}

//kmMat3* const kmMat3Multiply(kmMat3* pOut, const kmMat3* pM1, const kmMat3* pM2);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Multiply
  (JNIEnv *e, jclass c, jobject jo, jobject jm1, jobject jm2)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* m1 = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm1);    
    kmMat3* m2 = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm2);
    kmMat3Multiply(o,m1,m2);
    return jo;
}

// kmMat3* const kmMat3ScalarMultiply(kmMat3* pOut, const kmMat3* pM, const kmScalar pFactor);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3ScalarMultiply
  (JNIEnv *e, jclass c, jobject jo, jobject jm, jfloat f)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* m = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat3ScalarMultiply(o,m,f);
    return jo;
}

//kmMat3* const kmMat3RotationAxisAngle(kmMat3* pOut, const struct kmVec3* axis, kmScalar radians);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3RotationAxisAngle
  (JNIEnv *e, jclass c, jobject jo, jobject ja, jfloat r)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* a = (kmVec3*)(*e)->GetDirectBufferAddress(e, ja);
    kmMat3RotationAxisAngle(o,a,r);
    return jo;    
}

//struct kmVec3* const kmMat3RotationToAxisAngle(struct kmVec3* pAxis, kmScalar* radians, const kmMat3* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3RotationToAxisAngle
  (JNIEnv *e, jclass c, jobject ja, jobject jr, jobject ji)
{
    kmVec3* a = (kmVec3*)(*e)->GetDirectBufferAddress(e, ja);
    kmScalar* r = (kmScalar*)(*e)->GetDirectBufferAddress(e, jr);
    kmMat3* i = (kmMat3*)(*e)->GetDirectBufferAddress(e, ji);
    kmMat3RotationToAxisAngle(a,r,i);
    return ja;
}

// kmMat3* const kmMat3Assign(kmMat3* pOut, const kmMat3* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Assign
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* i = (kmMat3*)(*e)->GetDirectBufferAddress(e, ji);
    kmMat3Assign(o,i);
    return jo;    
}

//const int  kmMat3AreEqual(const kmMat3* pM1, const kmMat3* pM2);
JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmMat3AreEqual
  (JNIEnv *e, jclass c, jobject jm1, jobject jm2)
{
    kmMat3* m1 = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm1);
    kmMat3* m2 = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm2);
    return kmMat3AreEqual(m1,m2);
}

//struct kmVec3* const kmMat3GetUpVec3(struct kmVec3* pOut, const kmMat3* pIn);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3GetUpVec3
  (JNIEnv *e, jclass c, jobject jo, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* m = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat3GetUpVec3(o,m);
    return jo; 
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3GetRightVec3
  (JNIEnv *e, jclass c, jobject jo, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* m = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat3GetRightVec3(o,m);
    return jo;     
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3GetForwardVec3
  (JNIEnv *e, jclass c, jobject jo, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3* m = (kmMat3*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat3GetForwardVec3(o,m);
    return jo;     
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3RotationX
  (JNIEnv *e, jclass c, jobject jo, jfloat r)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3RotationX(o,r);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3RotationY
  (JNIEnv *e, jclass c, jobject jo, jfloat r)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3RotationY(o,r);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3RotationZ
  (JNIEnv *e, jclass c, jobject jo, jfloat r)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3RotationZ(o,r);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Rotation
  (JNIEnv *e, jclass c, jobject jo, jfloat r)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3Rotation(o,r);
    return jo;    
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Scaling
  (JNIEnv *e, jclass c, jobject jo, jfloat x, jfloat y)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3Scaling(o,x,y);
    return jo;    
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3Translation
  (JNIEnv *e, jclass c, jobject jo, jfloat x, jfloat y)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat3Translation(o,x,y);
    return jo;        
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat3RotationQuaternion
  (JNIEnv *e, jclass c, jobject jo, jobject jq)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmQuaternion* q = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq);
    kmMat3RotationQuaternion(o,q);
    return jo;        
}




JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmAABBContainsPoint
  (JNIEnv *e, jclass c, jobject jb, jobject jp)
{
    kmAABB* b = (kmAABB*)(*e)->GetDirectBufferAddress(e, jb);    
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    return kmAABBContainsPoint(b,p);
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmAABBAssign
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmAABB* o = (kmAABB*)(*e)->GetDirectBufferAddress(e, jo);  
    kmAABB* i = (kmAABB*)(*e)->GetDirectBufferAddress(e, ji);
    kmAABBAssign(o,i);
    return jo;  
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmAABBScale
  (JNIEnv *e, jclass c, jobject jo, jobject ji, jfloat s)
{
    kmAABB* o = (kmAABB*)(*e)->GetDirectBufferAddress(e, jo);  
    kmAABB* i = (kmAABB*)(*e)->GetDirectBufferAddress(e, ji);
    kmAABBScale(o,i,s);
    return jo;      
}

// kmBool kmAABBIntersectsTriangle(kmAABB* box, const kmVec3* p1, const kmVec3* p2, const kmVec3* p3);
JNIEXPORT jboolean JNICALL Java_kazmath_jkazmath_kmAABBIntersectsTriangle
  (JNIEnv *e, jclass c, jobject jb, jobject jp1, jobject jp2, jobject jp3)
{
    kmAABB* b = (kmAABB*)(*e)->GetDirectBufferAddress(e, jb);
    kmVec3* p1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp1);
    kmVec3* p2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp2);
    kmVec3* p3 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp3);
    return kmAABBIntersectsTriangle(b,p1,p2,p3);
}

//kmEnum kmAABBContainsAABB(const kmAABB* container, const kmAABB* to_check);
JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmAABBContainsAABB
  (JNIEnv *e, jclass c, jobject ja, jobject jb)
{
    kmAABB* a = (kmAABB*)(*e)->GetDirectBufferAddress(e, ja);
    kmAABB* b = (kmAABB*)(*e)->GetDirectBufferAddress(e, jb);
    return kmAABBContainsAABB(a,b);
}

// kmScalar kmAABBDiameterX(const kmAABB* aabb);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmAABBDiameterX
  (JNIEnv *e, jclass c, jobject jb)
{
    kmAABB* b = (kmAABB*)(*e)->GetDirectBufferAddress(e, jb);
    return kmAABBDiameterX(b);    
}

JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmAABBDiameterY
  (JNIEnv *e, jclass c, jobject jb)
{
    kmAABB* b = (kmAABB*)(*e)->GetDirectBufferAddress(e, jb);
    return kmAABBDiameterY(b);    
}

JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmAABBDiameterZ
  (JNIEnv *e, jclass c, jobject jb)
{
    kmAABB* b = (kmAABB*)(*e)->GetDirectBufferAddress(e, jb);
    return kmAABBDiameterZ(b);    
}



JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Fill
  (JNIEnv *e, jclass c, jobject v, jfloat x, jfloat y, jfloat z)
{
    kmVec3* vec = (kmVec3*)(*e)->GetDirectBufferAddress(e, v);
    kmVec3Fill(vec,(float)x,(float)y,(float)z);
    return v;
}

JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmVec3Length
  (JNIEnv *e, jclass c, jobject i)
{
    kmVec3* in = (kmVec3*)(*e)->GetDirectBufferAddress(e, i);
    return (jfloat)kmVec3Length(in);
}

JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmVec3LengthSq
  (JNIEnv *e, jclass c, jobject i)
{
    kmVec3* in = (kmVec3*)(*e)->GetDirectBufferAddress(e, i);
    return (jfloat)kmVec3LengthSq(in);
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Normalize
  (JNIEnv *e, jclass c, jobject o, jobject i)
{
    kmVec3* out = (kmVec3*)(*e)->GetDirectBufferAddress(e, o);
    kmVec3* in = (kmVec3*)(*e)->GetDirectBufferAddress(e, i);
    kmVec3Normalize(out,in);
    return o;    
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Cross
  (JNIEnv *e, jclass c, jobject jo, jobject jp1, jobject jp2)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp1);
    kmVec3* p2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp2);
    kmVec3Cross(o,p1,p2);
    return jo;
}

JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmVec3Dot
  (JNIEnv *e, jclass c, jobject jp1, jobject jp2)
{
    kmVec3* p1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp1);
    kmVec3* p2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp2);
    return kmVec3Dot(p1,p2);
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Add
  (JNIEnv *e, jclass c, jobject jo, jobject a, jobject b)
{
    kmVec3* out = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, a);
    kmVec3* p2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, b);
    kmVec3Add(out,p1,p2);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Subtract
  (JNIEnv *e, jclass c, jobject o, jobject a, jobject b)
{
    kmVec3* out = (kmVec3*)(*e)->GetDirectBufferAddress(e, o);
    kmVec3* p1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, a);
    kmVec3* p2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, b);
    kmVec3Subtract(out,p1,p2);
    return o;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Transform
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmVec3Transform(o, p, m);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3TransformNormal
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmVec3TransformNormal(o,p,m);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3TransformCoord
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmVec3TransformCoord(o,p,m);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Scale
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jfloat s)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    kmVec3Scale(o, p, s);
    return jo;
}

JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmVec3AreEqual
  (JNIEnv *e, jclass c, jobject jp1, jobject jp2)
{
    kmVec3* p1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp1);
    kmVec3* p2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp2);
    return kmVec3AreEqual(p1,p2);
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3InverseTransform
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmVec3InverseTransform(o, p, m);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3InverseTransformNormal
  (JNIEnv *e, jclass c, jobject jo, jobject jp, jobject jm)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* p = (kmVec3*)(*e)->GetDirectBufferAddress(e, jp);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmVec3InverseTransformNormal(o, p, m);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Assign
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* i = (kmVec3*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec3Assign(o,i);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Zero
  (JNIEnv *e, jclass c, jobject jo)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3Zero(o);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3GetHorizontalAngle
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* i = (kmVec3*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec3GetHorizontalAngle(o,i);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3RotationToDirection
  (JNIEnv *e, jclass c, jobject jo, jobject ji, jobject jf)
{
    kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
    kmVec3* i = (kmVec3*)(*e)->GetDirectBufferAddress(e, ji);
    kmVec3* f = (kmVec3*)(*e)->GetDirectBufferAddress(e, jf);
    kmVec3RotationToDirection(o,i,f);
    return jo;
}





// kmMat4

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Fill
  (JNIEnv *e, jclass c, jobject jo, jobject jm)
{
    kmMat4* o = (kmMat4*)(*e)->GetDirectBufferAddress(e, jo);
    kmScalar* m = (kmScalar*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat4Fill(o, m);
    return jm;    
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Identity
  (JNIEnv * e, jclass c, jobject m)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, m);
    kmMat4Identity(mat);
    return m;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Inverse
  (JNIEnv *e, jclass c, jobject jo, jobject jm)
{
    kmMat4* o = (kmMat4*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat4Inverse(o,m);
    return jo;
}

JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmMat4IsIdentity
  (JNIEnv *e, jclass c, jobject ji)
{
    kmMat4* i = (kmMat4*)(*e)->GetDirectBufferAddress(e, ji);
    return kmMat4IsIdentity(i);
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Transpose
  (JNIEnv *e,jclass c, jobject jo, jobject jm)
{
    kmMat4* o = (kmMat4*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat4* m = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);
    kmMat4Transpose(o,m);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Multiply
  (JNIEnv *e, jclass c, jobject o, jobject m1, jobject m2)
{
    kmMat4* out = (kmMat4*)(*e)->GetDirectBufferAddress(e, o);
    kmMat4* mat1 = (kmMat4*)(*e)->GetDirectBufferAddress(e, m1);
    kmMat4* mat2 = (kmMat4*)(*e)->GetDirectBufferAddress(e, m2);
    kmMat4Multiply(out,mat1,mat2);
    return o;    
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Assign
  (JNIEnv *e, jclass c, jobject m1, jobject m2)
{
    kmMat4* mat1 = (kmMat4*)(*e)->GetDirectBufferAddress(e, m1);
    kmMat4* mat2 = (kmMat4*)(*e)->GetDirectBufferAddress(e, m2);   
    kmMat4Assign(mat1,mat2);
    return m1;
}

JNIEXPORT jint JNICALL Java_kazmath_jkazmath_kmMat4AreEqual
  (JNIEnv *e, jclass c, jobject m1, jobject m2)
{
    kmMat4* mat1 = (kmMat4*)(*e)->GetDirectBufferAddress(e, m1);
    kmMat4* mat2 = (kmMat4*)(*e)->GetDirectBufferAddress(e, m2);   
    return kmMat4AreEqual(mat1,mat2);
}


JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4RotationX
  (JNIEnv *e, jclass c, jobject m, jfloat rad)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, m);
    kmMat4RotationX(mat,rad);
    return m;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4RotationY
  (JNIEnv *e, jclass c, jobject m, jfloat rad)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, m);
    kmMat4RotationY(mat,rad);
    return m;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4RotationZ
  (JNIEnv *e, jclass c, jobject m, jfloat rad)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, m);
    kmMat4RotationZ(mat,rad);
    return m;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4RotationYawPitchRoll
  (JNIEnv *e, jclass c, jobject m, jfloat x, jfloat y, jfloat z)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, m);
    kmMat4RotationYawPitchRoll(mat,x,y,z);
    return m;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4RotationQuaternion
  (JNIEnv *e, jclass c, jobject ji, jobject jq)
{
    kmMat4* i = (kmMat4*)(*e)->GetDirectBufferAddress(e, ji);
    kmQuaternion* q = (kmQuaternion*)(*e)->GetDirectBufferAddress(e, jq);
    kmMat4RotationQuaternion(i,q);
    return ji;    
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Translation
  (JNIEnv *e, jclass c, jobject m, jfloat x, jfloat y, jfloat z)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, m);
    kmMat4Translation(mat,x,y,z);
    return m;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4Scaling
  (JNIEnv *e, jclass c, jobject m, jfloat x, jfloat y, jfloat z)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, m);
    kmMat4Scaling(mat,x,y,z);
    return m;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4GetUpVec3
  (JNIEnv *e, jclass c, jobject jv, jobject jm)
{
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);   
    kmMat4GetUpVec3(v,mat);
    return jv;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4GetRightVec3
  (JNIEnv *e, jclass c, jobject jv, jobject jm)
{
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);   
    kmMat4GetRightVec3(v,mat);
    return jv;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4GetForwardVec3RH
  (JNIEnv *e, jclass c, jobject jv, jobject jm)
{
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);   
    kmMat4GetForwardVec3RH(v,mat);
    return jv;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4GetForwardVec3LH
  (JNIEnv *e, jclass c, jobject jv, jobject jm)
{
    kmVec3* v = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv);
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);   
    kmMat4GetForwardVec3LH(v,mat);
    return jv;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4PerspectiveProjection
  (JNIEnv *e, jclass c, jobject p, jfloat fov, jfloat aspect, jfloat near, jfloat far)
{
    kmMat4* proj = (kmMat4*)(*e)->GetDirectBufferAddress(e, p);
    kmMat4PerspectiveProjection(proj,fov,aspect,near,far);
    return p;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4OrthographicProjection
  (JNIEnv *e, jclass c, jobject jm, jfloat l, jfloat r, jfloat b, jfloat t, jfloat n, jfloat f)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);       
    kmMat4OrthographicProjection(mat,l,r,b,t,n,f);
    return jm;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4LookAt
  (JNIEnv *env, jclass cls, jobject v, jobject e, jobject c, jobject u)
{
    kmMat4* view = (kmMat4*)(*env)->GetDirectBufferAddress(env, v);
    kmVec3* eye = (kmVec3*)(*env)->GetDirectBufferAddress(env, e);    
    kmVec3* centre = (kmVec3*)(*env)->GetDirectBufferAddress(env, c);    
    kmVec3* up = (kmVec3*)(*env)->GetDirectBufferAddress(env, u);
    kmMat4LookAt(view,eye,centre,up);
    return v;    
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4RotationAxisAngle
  (JNIEnv *e, jclass c, jobject jm, jobject ja, jfloat r)
{
    kmMat4* mat = (kmMat4*)(*e)->GetDirectBufferAddress(e, jm);       
    kmVec3* a = (kmVec3*)(*e)->GetDirectBufferAddress(e, ja);           
    kmMat4RotationAxisAngle(mat,a,r);
    return jm;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4ExtractRotation
  (JNIEnv *e, jclass c, jobject jo, jobject ji)
{
    kmMat3* o = (kmMat3*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat4* i = (kmMat4*)(*e)->GetDirectBufferAddress(e, ji);   
    kmMat4ExtractRotation(o,i);
    return jo;
}

JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmMat4ExtractPlane
  (JNIEnv *e, jclass c, jobject jo, jobject ji, jint p)
{
    kmPlane* o = (kmPlane*)(*e)->GetDirectBufferAddress(e, jo);
    kmMat4* i = (kmMat4*)(*e)->GetDirectBufferAddress(e, ji);       
    kmMat4ExtractPlane(o, i, p);
    return jo;
}

// kmScalar kmLerp(kmScalar x, kmScalar y, kmScalar factor);
JNIEXPORT jfloat JNICALL Java_kazmath_jkazmath_kmLerp
  (JNIEnv *e, jclass c, jfloat x, jfloat y, jfloat f)
{
	return kmLerp(x,y,f);
	
}

// kmVec3* kmVec3Lerp(kmVec3* pOut, const kmVec3* pV1, const kmVec3* pV2, kmScalar t);
JNIEXPORT jobject JNICALL Java_kazmath_jkazmath_kmVec3Lerp
   (JNIEnv *e , jclass c, jobject jo, jobject jv1, jobject jv2, jfloat t)
{
	kmVec3* o = (kmVec3*)(*e)->GetDirectBufferAddress(e, jo);
	kmVec3* v1 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv1);
	kmVec3* v2 = (kmVec3*)(*e)->GetDirectBufferAddress(e, jv2);
	kmVec3Lerp(o,v1,v2,t);
	return jo;
}
