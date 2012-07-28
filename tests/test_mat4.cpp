#include <cstdlib>
#include <cstdio>
#include <memory.h>
#include <unittest++/UnitTest++.h>
#include "../kazmath/mat4.h"

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


TEST(test_mat4_inverse) {
    kmMat4 mat;
    CHECK(NULL != kmMat4Identity(&mat));

    CHECK(NULL != kmMat4Inverse(&mat,&mat));
    CHECK(kmMat4IsIdentity(&mat));
}


TEST(test_mat4_transpose) {
    kmMat4 mat;
    float temp[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f };
    memcpy(mat.mat, temp, sizeof(float) * 16);

    kmMat4 transpose;
    float temp2[] = {0.0f, 4.0f, 8.0f, 12.0f, 1.0f, 5.0f, 9.0f, 13.0f, 2.0f, 6.0f, 10.0f, 14.0f, 3.0f, 7.0f, 11.0f, 15.0f };
    memcpy(transpose.mat, temp2, sizeof(float) * 16);

    kmMat4 result;
    CHECK(NULL != kmMat4Transpose(&result, &mat));
    CHECK(kmMat4AreEqual(&transpose, &result));
}

