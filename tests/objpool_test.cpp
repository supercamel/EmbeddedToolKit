#include "objpool_test.h"
#include <iostream>
using namespace std;

using namespace etk;

struct testobj
{
    StaticString<20> ss;
};

bool objpool_test(std::string& subtest)
{

    etk::ObjPool<testobj, 5> pool;

    {
        cout << pool.n_available() << endl;
        auto ptr = pool.alloc();
        ptr->ss = "Testing";

        testobj* rptr = pool.raw_alloc();

        cout << pool.n_available() << endl;

        cout << "going down in scope now" << endl;
    }
    cout << pool.n_available() << endl;


    return true;
}
