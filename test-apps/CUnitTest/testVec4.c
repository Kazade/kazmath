#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <kazmath/vec4.h>

void testVec4Length(void)
{
	kmVec4 vec;
	vec.x = 4.0f;
	vec.y = 3.0f;
	vec.z = 5.7f;
	vec.w = 1.0f;

	CU_ASSERT(7.6478755f == kmVec4Length(&vec));
}

int addVec4Tests(CU_pSuite suite)
{
    /* add the tests to the suite */
   if ((NULL == CU_add_test(suite, "kmVec4Length - test", testVec4Length)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   return CU_get_error();
}
