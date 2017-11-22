#include "forward_list_test.h"

#include <iostream>
using namespace std;

bool forward_list_test(std::string& subtest)
{
	etk::experimental::MemPool<1024> pool;
	etk::experimental::forward_list<int, 1024> list(pool);

	subtest = "iterator tests";
	for(int i = 0; i < 10; i++)
		list.append(i);

	int count = 0;
	auto iter = list.begin();
	while(iter)
	{
		if(*iter != count++)
			return false;
		iter++;
	}

	iter = list.begin();
	while(iter)
	{
		if(*iter == 5)
			list.remove_after(iter);
		iter++;
	}

	for(auto& i : list)
	{
		if(i == 6)
			return false;
	}

	subtest = "length";
	list.free();
	if(list.length() != 0) {
		return false;
	}

	list.append(5);
	if(list.length() != 1) {
		return false;
	}

	return true;
}
