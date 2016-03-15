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
	list.remove(0);
	if(list.count(4) != 1)
		return false;

	subtest = "Insert";
	list.insert(4, 0);
	if(list.raw_memory()[0] != 4)
		return false;

	subtest = "Erase";
	list.erase(0, 3);

	list.insert(3, 0);
	list.insert(0, 0);
	list.insert(4, 0);

	list.erase(2, 3);

	etk::List<int, 5>::Iterator iter = list.begin();

    //syntax demo - it does work, really!
    //TODO actually run this test
	for(auto i : list)
	{
		//cout << i << endl;
	}
	
	etk::List<etk::Coordinate, 64> clist;
	auto c = etk::Coordinate(34.65, 134.4);
	clist.append(c);
	clist.append(c);
	
	clist.remove(0);
	
	auto p = clist[0];
	if(!etk::compare(p.get_lat(), 34.65, 0.001))
		return false;
	

    return true;
}
