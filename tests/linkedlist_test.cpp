#include "linkedlist_test.h"

#include <etk/etk.h>

#include <iostream>
using namespace std;

using namespace etk;

bool linkedlist_test(std::string& subtest)
{
    List<uint8_t, 64> list;
    list.set_list_end(64);

    MemPool pool(list.raw_memory(), 64);
	subtest = "Ll";

	forward_list<int> fl(pool);

	fl.append();
	for(int i = 0; i < 8; i++)
		fl.append(i);

	fl.remove_item(5);
	fl.remove_n(0);
	fl.remove_n(2);

	fl.insert_before(fl.begin(), 6554);

	etk::forward_list<int>::Iterator iter = fl.begin();


	int counter = 0;
	while(iter != fl.end())
	{
		counter++;
		if(counter == 3)
			fl.insert_after(iter, 6888);
		iter++;
	}

    for(auto i : list)
        cout << "";
    cout << endl;

	etk::linked_list<float> ll(pool);

	for(float i = -0.5; i < 0.5; i += 0.2)
		ll.append(i);

	return true;
}


