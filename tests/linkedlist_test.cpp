#include "linkedlist_test.h"

#include <etk/forward_list.h>
#include <etk/linked_list.h>

#include <iostream>
using namespace std;

using namespace etk;

extern MemPool pool;

bool linkedlist_test(std::string& subtest)
{
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

	iter = fl.begin();
	while(iter != fl.end())
		cout << *(iter++) << " ";
	cout << endl;


	etk::linked_list<float> ll(pool);

	for(float i = -0.5; i < 0.5; i += 0.1)
	{
		cout << i << endl;
		ll.append(i);
	}

	etk::linked_list<float>::Iterator fiter = ll.begin();
	while(fiter != ll.end())
		cout << *(fiter++) << " ";
	cout << endl;

	fiter--;
	fiter--;

	ll.insert_before(fiter, 0.36565);
	fiter = ll.begin();
	while(fiter != ll.end())
		cout << *(fiter++) << " ";
	cout << endl;

	return true;
}


