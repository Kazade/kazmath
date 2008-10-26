#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <kazmath/vec3.h>
#include <kazmath/utility.h>
#include <math.h>

void testVec3AreEqual(void)
{
    kmVec3 v1, v2;
    v1.x = 2.7f;
    v1.y = 0.9f;
    v1.z = -153.68f;

    v2.x = 2.7f;
    v2.y = 0.9f;
    v2.z = -153.68f;

    CU_ASSERT(kmVec3AreEqual(&v1, &v2) == 1);
}

void testVec3Fill(void)
{
    kmVec3 v1, v2;
    v1.x = 2.7f;
    v1.y = 0.9f;
    v1.z = -153.68f;
    kmVec3Fill(&v2, 2.7f, 0.9f, -153.68f);

    CU_ASSERT(kmVec3AreEqual(&v1, &v2) == 1);
}

void testVec3Length(void)
{
	kmVec3 vec;
	vec.x = 4.0f;
	vec.y = 3.0f;
	vec.z = 5.0f;

	CU_ASSERT(7.071068f == kmVec3Length(&vec));
}

void testVec3LengthSq(void)
{
	kmVec3 vec;
	vec.x = 4.0f;
	vec.y = 3.0f;
	vec.z = 5.0f;

	CU_ASSERT(50.0f == kmVec3LengthSq(&vec));
}

void testVec3Normalize(void)
{
    kmVec3 vec;
	vec.x = 4.0f;
	vec.y = 3.0f;
	vec.z = 5.0f;

	kmScalar length = kmVec3Length(&vec);

	kmVec3 norm;
	kmVec3Fill(&norm, vec.x / length, vec.y / length, vec.z / length);

	kmVec3 testNorm;
	kmVec3Normalize(&testNorm, &vec);

	CU_ASSERT(1 == kmVec3AreEqual(&norm, &testNorm));
}

void testVec3Cross(void)
{
    kmVec3 v1, v2;
    kmVec3Fill(&v1, 1.6f, 2.7f, 3.8f);
    kmVec3Fill(&v2, 3.5f, 4.4f, 5.3f);

    kmVec3 result;
    kmVec3Cross(&result, &v1, &v2);

    kmVec3 shouldBe;
    kmVec3Fill(&shouldBe, -2.41f, 4.82f, -2.41f);
    CU_ASSERT(kmVec3AreEqual(&shouldBe, &result));
}

void testVec3Dot(void)
{
    kmVec3 v1, v2;
    kmVec3Fill(&v1, 1.6f, 2.7f, 3.8f);
    kmVec3Fill(&v2, 3.5f, 4.4f, 5.3f);

    CU_ASSERT(fabs(37.62f - kmVec3Dot(&v1, &v2)) < kmEpsilon);
}

void testVec3Add(void)
{
    kmVec3 v1, v2;
    kmVec3Fill(&v1, 1.6f, 2.7f, 3.8f);
    kmVec3Fill(&v2, 3.5f, 4.4f, 5.3f);

    kmVec3 result;
    kmVec3Add(&result, &v1, &v2);

    kmVec3 shouldBe;
    kmVec3Fill(&shouldBe, 5.1f, 7.1f, 9.1f);
    CU_ASSERT(kmVec3AreEqual(&shouldBe, &result));
}

void testVec3Subtract(void)
{
    kmVec3 v1, v2;
    kmVec3Fill(&v1, 1.6f, 2.7f, 3.8f);
    kmVec3Fill(&v2, 3.5f, 4.4f, 5.3f);

    kmVec3 result;
    kmVec3Subtract(&result, &v1, &v2);

    kmVec3 shouldBe;
    kmVec3Fill(&shouldBe, -1.9f, -1.7f, -1.5f);
    CU_ASSERT(kmVec3AreEqual(&shouldBe, &result));
}

void testVec3Transform(void)
{
    CU_ASSERT(0);
}

void testVec3TransformNormal(void)
{
    CU_ASSERT(0);
}

void testVec3TransformCoord(void)
{
    CU_ASSERT(0);
}

void testVec3Scale(void)
{
    kmVec3 test;
    kmVec3Fill(&test, 1.0f, 2.0f, 3.0f);
    kmVec3Scale(&test, &test, 3.5f);

    kmVec3 shouldBe;
    kmVec3Fill(&shouldBe, 3.5f, 7.0f, 10.5f);
    CU_ASSERT(kmVec3AreEqual(&shouldBe, &test));
}

void testVec3Zero(void)
{
    kmVec3 test;
    kmVec3Fill(&test,7.0f,3.5f,1234.6f);
    kmVec3Zero(&test);

    kmVec3 shouldBe;
    kmVec3Fill(&shouldBe, 0.0f, 0.0f, 0.0f);
    CU_ASSERT(kmVec3AreEqual(&shouldBe, &test));
}

void testVec3InverseTransform(void)
{
    CU_ASSERT(0);
}

void testVec3InverseTransformNormal(void)
{
    CU_ASSERT(0);
}

void testVec3InverseTransformCoord(void)
{
    CU_ASSERT(0);
}


int addVec3Tests(CU_pSuite suite)
{
    /* add the tests to the suite */
    if ((NULL == CU_add_test(suite, "kmVec3AreEqual - test", testVec3AreEqual)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Fill - test", testVec3Fill)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Length - test", testVec3Length)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3LengthSq - test", testVec3LengthSq)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Normalize - test", testVec3Normalize)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Dot - test", testVec3Dot)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Cross - test", testVec3Cross)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Add - test", testVec3Add)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Subtract - test", testVec3Subtract)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Scale - test", testVec3Scale)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Transform - test", testVec3Transform)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3TransformCoord - test", testVec3TransformCoord)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3TransformNormal - test", testVec3TransformNormal)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3InverseTransform - test", testVec3InverseTransform)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3InverseTransformCoord - test", testVec3InverseTransformCoord)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3InverseTransformNormal - test", testVec3InverseTransformNormal)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(suite, "kmVec3Zero - test", testVec3Zero)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   return CU_get_error();
}
