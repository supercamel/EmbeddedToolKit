#include <etk/etk.h>
#include <iostream>


using namespace std;
using namespace etk;



int main()
{
	List<int, 5> list;
	
	list.append(4);
	list.append(642);
	list.append(-643);
	
	cout << "After appending 3 items, the list size is now: " << list.size() << endl;
	
	cout << "The list contains:\n\t";
	for(auto& l : list)
		cout << l << " ";
	cout << endl;
	
	list.remove(0);
	
	cout << "After removing the first object, the list size is: " << list.size() << endl;
	cout << "The list contains:\n\t";
	for(auto& l : list)
		cout << l << " ";
	cout << endl;
}


