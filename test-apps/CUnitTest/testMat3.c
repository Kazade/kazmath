#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <kazmath/mat3.h>

void print_matrix3(const kmMat3* mat)
{
    const int max = 3;
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

void testMat3Inverse1(void)
{
	kmMat3 mat;
	CU_ASSERT(NULL != kmMat3Identity(&mat));
	print_matrix3(&mat);
	printf("\nDeterminant of Identity matrix is: %f\n", kmMat3Determinant(&mat));

	kmMat3 adj;
	CU_ASSERT(NULL != kmMat3Adjugate(&adj, &mat));
	print_matrix3(&adj);
	CU_ASSERT(NULL != kmMat3Inverse(&mat, kmMat3Determinant(&mat), &mat));
	print_matrix3(&mat);
	CU_ASSERT(kmMat3IsIdentity(&mat));
}

void testMat3Transpose(void)
{
    kmMat3 mat;
    float temp[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
    memcpy(mat.mat, temp, sizeof(float) * 9);

    print_matrix3(&mat);

    kmMat3 transpose;
    float temp2[] = {8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f};
    memcpy(transpose.mat, temp2, sizeof(float) * 9);
    print_matrix3(&transpose);

    kmMat3 result;
    CU_ASSERT(NULL != kmMat3Transpose(&result, &mat));
    print_matrix3(&result);
    CU_ASSERT(kmMat3AreEqual(&transpose, &result));
}

void testMat3AxisAngle(void) {
    float radians = 1.0;
    kmMat3 a;
    kmVec3 axisIn;
    kmVec3Fill(&axisIn, 1.0f, 0.0f, 0.0f);
    kmMat3RotationAxis(&a, &axisIn, radians);
}

int addMat3Tests(CU_pSuite suite)
{
    /* add the tests to the suite */
   if ((NULL == CU_add_test(suite, "kmMat3Inverse Identity - test", testMat3Inverse1)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(suite, "kmMat3Transpose - test", testMat3Transpose)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   return CU_get_error();
}
