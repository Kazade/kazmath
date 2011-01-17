#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <kazmath/mat4.h>

void print_matrix4(const kmMat4* mat)
{
    const int max = 4;
    printf("\n\n");
    for(int i = 0; i < max; i++)
    {
        printf("|");

        for(int j = 0; j < max; j++)
        {
            if(j > 0)
            {
                printf("\t");
            }

            printf("%f",mat->mat[i + max*j]);
        }

        printf("|\n");
    }
    printf("\n");
}

void testMat4Inverse1(void)
{
	kmMat4 mat;
	CU_ASSERT(NULL != kmMat4Identity(&mat));
	print_matrix4(&mat);
	CU_ASSERT(NULL != kmMat4Inverse(&mat, &mat));
	print_matrix4(&mat);
	CU_ASSERT(kmMat4IsIdentity(&mat));
}

int addMat4Tests(CU_pSuite suite)
{
    /* add the tests to the suite */
   if ((NULL == CU_add_test(suite, "kmMat4Inverse Identity - test", testMat4Inverse1)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   return CU_get_error();
}
