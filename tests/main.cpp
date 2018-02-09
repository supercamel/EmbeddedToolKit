#include <iostream>

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
#include "pool_test.h"
#include "array_test.h"
#include "tokeniser_test.h"
#include "objpool_test.h"
#include "forward_list_test.h"
#include "dynamic_list_test.h"



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
   	std::cout << "Version string: " << etk::Version::version << std::endl;
   	etk::unused(etk::Version::year);
   	etk::unused(etk::Version::month);
   	etk::unused(etk::Version::day);

    th.add_module(pool_test, "Pool test");

    
    th.add_module(test_rope, "Rope Test");
    th.add_module(list_test, "List Test");
    th.add_module(static_string_test, "Static String");
    th.add_module(bits_test, "Bits test");
    th.add_module(limiter_test, "Limiter test");
    th.add_module(navigation_test, "Navigation test");
    th.add_module(array_test, "Array test");
    th.add_module(tokeniser_test, "Tokeniser test");
    th.add_module(objpool_test, "Object pools");
    th.add_module(forward_list_test, "Forward list");
    th.add_module(dynamic_list_test, "Dynamic list");

    if(th.run())
        return 0;


    return -1;
}
