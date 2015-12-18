#include <etk/etk.h>
#include <iostream>


using namespace std;
using namespace etk;

int main()
{
	Array<int, 3> arr;
	
	//etk::Array won't write past the end of the array
	for(auto i : range(100))
		arr[i] = i; //if this was a standard C array, it would be destroyed
	
	for(auto i : range(3))
		cout << arr[i] << " ";
	cout << endl;
}

