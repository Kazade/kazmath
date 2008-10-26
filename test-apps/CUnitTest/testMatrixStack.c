#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <kazmath/GL/matrix.h>
#include <kazmath/kazmath.h>

void testMatrixMode(void)
{
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadIdentity();

	kmMat4 modelview;
	kmMat4RotationX(&modelview, kmDegreesToRadians(45.0f));
	kmGLMultMatrix(&modelview);

	kmMat4 out;
	kmGLGetMatrix(KM_GL_MODELVIEW, &out);

	CU_ASSERT(kmMat4AreEqual(&modelview, &out));

	kmGLMatrixMode(KM_GL_PROJECTION);
	kmMat4 identity;
	kmMat4Identity(&identity);

	kmGLGetMatrix(KM_GL_PROJECTION, &out);
	CU_ASSERT(kmMat4AreEqual(&identity, &out));
}

int addMat4StackTests(CU_pSuite suite)
{
    /* add the tests to the suite */
   if ((NULL == CU_add_test(suite, "kmGLMatrixMode - test", testMatrixMode)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   return CU_get_error();
}
