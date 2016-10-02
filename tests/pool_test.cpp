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
    char* p1 = (char*)pool.alloc(20);
    char* p2 = (char*)pool.alloc(20);

    if(sizeof(size_t) == 4) //32bit system
    {
	    subtest = "There is 12 bytes of overhead between each block";
	    if((p2-32) != p1)
		    return false;
    }
    if(sizeof(size_t) == 8) //64bit system
    {
        subtest = "There is 20 bytes of overhead for each block";
        if((p2-44) != p1)
            return false;
    }

    auto pto = make_pool_ptr<Object>(pool);

    subtest = "Returns valid pointer";
    if(!pto)
        return false;

    subtest = "Underlying object works ok";
    if(pto->get_ss() != "Hello world!")
        return false;

    return true;
}
