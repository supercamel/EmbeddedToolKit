#include "array_test.h"
#include <etk/array.h>
#include <etk/loop_range.h>
#include <iostream>
using namespace std;

using namespace etk;

bool array_test(std::string& subtest)
{
    etk::Array<int, 10> ar;

    for(auto i : range(10))
        ar[i] = i*2;

    int count = 0;
    for(auto i : ar)
    {
        if(i != (count*2))
            return false;
        count++;
    }

    ar[15] = 512;
    if(ar[9] != 512)
        return false;

    return true;
}
