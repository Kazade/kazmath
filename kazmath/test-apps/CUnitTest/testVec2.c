#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <kazmath/vec2.h>

void testVec2Length(void)
{
	kmVec2 vec;
	vec.x = 4.0f;
	vec.y = 3.0f;

	CU_ASSERT(5.0f == kmVec2Length(&vec));
}

int addVec2Tests(CU_pSuite suite)
{
    /* add the tests to the suite */
   if ((NULL == CU_add_test(suite, "kmVec2Length - test", testVec2Length)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   return CU_get_error();
}
