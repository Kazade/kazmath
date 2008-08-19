#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <kazmath/vec3.h>

void testVec3Length(void)
{
	kmVec3 vec;
	vec.x = 4.0f;
	vec.y = 3.0f;
	vec.z = 5.0f;

	CU_ASSERT(7.071068f == kmVec3Length(&vec));
}

int addVec3Tests(CU_pSuite suite)
{
    /* add the tests to the suite */
   if ((NULL == CU_add_test(suite, "kmVec3Length - test", testVec3Length)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   return CU_get_error();
}
