
#include "harness.h"
#include "rope_tests.h"
#include "static_string_tests.h"
#include "list_tests.h"
#include "bits_test.h"
#include "vector_test.h"
#include "matrix_test.h"
#include "quaternion_test.h"
#include "navigation_tests.h"
//#include "string_test.h"
//#include "stack_test.h"
#include "out.h"
//#include "linkedlist_test.h"
//#include "pool_test.h"


bool limiter_test(std::string& subtest)
{
	subtest = "Rate limiter";
	using namespace etk;
	RateLimiter rl(1.0, 0.0);

	for(int i = 0; i < 50; i += 2)
	{
		rl.step(i);
	}
	for(int i = 0; i < 29; i++)
		rl.step(50);

	if(rl.step(50) != 50)
		return false;
	return true;


}


int main()
{
    TestHarness th("ETK Test Harness");
	th.add_module(test_rope, "Rope Test");
	th.add_module(list_test, "List Test");
	th.add_module(static_string_test, "Static String");
	th.add_module(bits_test, "Bits test");
	th.add_module(vector_test, "Vector test");
	th.add_module(matrix_test, "Matrix test");
	th.add_module(quaternion_test, "Quaternion test");
	th.add_module(limiter_test, "Limiter test");
	th.add_module(navigation_test, "Navigation test");
	//th.add_module(string_test, "String test");
	//th.add_module(stack_test, "Stack test");
	//th.add_module(pool_test, "Pool test");
	//th.add_module(linkedlist_test, "Linked lists");

	if(th.run())
		return 0;


    return -1;
}


