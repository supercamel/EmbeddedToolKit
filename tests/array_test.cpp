#include "array_test.h"
#include <etk/array.h>
#include <etk/loop_range.h>
#include <etk/math_util.h>
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


    for(int i = 0; i < 6; i++)
        ar[i] = i;

    etk::reverse(ar, 6);
    for(int i = 0; i < 6; i++)
    {
        if(ar[i] != 5-i)
            return false;
    }
    
    etk::Array<int, 5> t = {0, 1, 2, 3};
    for(int i = 0; i < 4; i++)
    {
    	if(t[i] != i)
    		return false;
   	}

    return true;
}
