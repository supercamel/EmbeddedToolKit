#include "pool_test.h"

#include "etk/etk.h"

#include <iostream>
using namespace etk;
using namespace std;

experimental::MemPool<1024> pool;


class Object
{
public:
    Object() { }
    ~Object() { }
    StaticString<128> get_ss() {
        return ss;
    }

private:
    int i = 0;
    StaticString<128> ss = "Hello world!";
};

bool pool_test(string& subtest)
{
    auto pto = experimental::make_pool_ptr<Object>(pool);

    subtest = "Returns valid pointer";
    if(!pto)
        return false;

    subtest = "Underlying object works ok";
    if(pto->get_ss() != "Hello world!")
        return false;

    return true;
}
