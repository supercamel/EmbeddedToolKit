#include "objpool_test.h"
#include <iostream>
using namespace std;

using namespace etk;

struct test_list
{
    StaticString<20> ss;
    experimental::PoolPtr<experimental::ObjPool<test_list, 10>, test_list> next;
};

bool objpool_test(std::string& subtest)
{

    etk::experimental::ObjPool<test_list, 10> pool;

    {
        subtest = "proving reference counting";
        auto head = pool.alloc();

        // this is why I love auto. head is of type
        //
        //    etk::PoolPtr<ObjPool<test_list, 10>, test_list
        //
        // imagine writing that out all the time!
        head->ss = "Head";

        auto last = head;
        for(auto i : range(9))
        {
            last->next = pool.alloc();
            last->next->ss = i;
            last = last->next;
        }

        if(pool.n_available() != 0)
            return false;

        last = head;
        int count = 0;
        while(last)
            last = last->next;

        last = head;
        for(auto i : range(5))
            last = last->next;
        head->next = last;

        if(pool.n_available() != 4)
            return false;
    }

    subtest = "objects free when going out of scope";
    if(pool.n_available() != 10)
        return false;

    return true;
}
