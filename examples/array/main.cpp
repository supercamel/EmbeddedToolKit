#include <etk/etk.h>
#include <iostream>


using namespace std;
using namespace etk;


/**
 * C-styled arrays are typically passed to functions are pointers.
 *      foo(int* arr);
 * It's not exactly clear how long arr is, and the length of arr cannot be checked. It's a bug waiting to happen.
 *
 * etk::Array explicitly states the length of the array as a template argument so it's always very clear how long the array is.
 * It also provides bounds checking so shooting yourself in the foot becomes a lot more difficult.
 *
 */

void foo(const Array<int, 3>& arr)
{
    for(auto i : range(3))
        cout << arr.get(i) << " ";
}


int main()
{
	Array<int, 3> arr;
	
	//etk::Array won't write past the end of the array
	for(auto i : range(100))
		arr[i] = i; //if this was a standard C array, it would be destroyed
	
	foo(arr);
	
	cout << endl;
}

