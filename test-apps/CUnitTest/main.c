/*
CUnit tests for Kazmath
 */

#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>

/* The suite initialization function.
 * Nothing to do here
 */
int init_suite1(void)
{
	return 0;
}

/* The suite cleanup function.
 *
 */
int clean_suite1(void)
{
	return 0;
}

extern int addVec2Tests(CU_pSuite suite);
extern int addVec3Tests(CU_pSuite suite);
extern int addVec4Tests(CU_pSuite suite);
extern int addMat3Tests(CU_pSuite suite);
extern int addMat4Tests(CU_pSuite suite);
extern int addMat4StackTests(CU_pSuite suite);

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add the suites to the registry */
	CU_pSuite vec2testSuite = CU_add_suite("Vec2 - Test - Suite", init_suite1, clean_suite1);
	if (NULL == vec2testSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	addVec2Tests(vec2testSuite);

	CU_pSuite vec3testSuite = CU_add_suite("Vec3 - Test - Suite", init_suite1, clean_suite1);
	if (NULL == vec3testSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	addVec3Tests(vec3testSuite);

	CU_pSuite vec4testSuite = CU_add_suite("Vec4 - Test - Suite", init_suite1, clean_suite1);
	if (NULL == vec4testSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	addVec4Tests(vec4testSuite);

	CU_pSuite mat3testSuite = CU_add_suite("Mat3 - Test - Suite", init_suite1, clean_suite1);
	if (NULL == mat3testSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	addMat3Tests(mat3testSuite);

	CU_pSuite mat4testSuite = CU_add_suite("Mat4 - Test - Suite", init_suite1, clean_suite1);
	if (NULL == mat4testSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	addMat4Tests(mat4testSuite);

	CU_pSuite mat4stackTestSuite = CU_add_suite("Mat4 Stack - Test - Suite", init_suite1, clean_suite1);
	if (NULL == mat4stackTestSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	addMat4StackTests(mat4stackTestSuite);

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

