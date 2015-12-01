#include "list_tests.h"
#include <etk/list.h>
#include "out.h"

#include <iostream>
using namespace std;


bool list_test(std::string& subtest)
{
	subtest = "List creation";
    etk::List<int, 5> list;

    if(list.size())
		return false;

	subtest = "List append";
	list.append(4);
	if(list.size() != 1)
		return false;

	list.append(0);
	list.append(3);

	if(list.size() != 3)
		return false;

	subtest = "count";
	list.append(4);
	if(list.count(4) != 2)
		return false;

	list.append(62);
	list.append(413);
	if(list.size() != 5)
		return false;

	subtest = "Remove";
	list.remove(0, 0);
	if(list.count(4) != 1)
		return false;

	subtest = "Insert";
	list.insert(4, 0);
	if(list.raw_memory()[0] != 4)
		return false;

	subtest = "Erase";
	list.erase(0, 3, 0);

	if(list.raw_memory()[0] != 4)
		return false;

	list.insert(3, 0);
	list.insert(0, 0);
	list.insert(4, 0);

	list.erase(2, 3, 1);
	for(auto i : range(2, 5))
	{
		if(list.raw_memory()[i] != 1)
			return false;
	}

	etk::List<int, 5>::Iterator iter = list.begin();

    //syntax demo - it does work, really!
    //TODO actually run this test
	for(auto i : list)
	{
		//cout << i << endl;
	}

    return true;
}
