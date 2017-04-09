#include "pool_test.h"

#include "etk/etk.h"

#include <iostream>
using namespace etk;
using namespace std;

MemPool<1024> pool;


class Object
{
public:
    Object() { }
    ~Object() { }
    auto get_ss() {
        return ss;
    }

private:
    int i = 0;
    StaticString<128> ss = "Hello world!";
};

bool pool_test(string& subtest)
{
    auto pto = make_pool_ptr<Object>(pool);

    subtest = "Returns valid pointer";
    if(!pto)
        return false;

    subtest = "Underlying object works ok";
    if(pto->get_ss() != "Hello world!")
        return false;

    return true;
}
