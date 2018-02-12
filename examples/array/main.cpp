#include <etk/etk.h>
#include <iostream>


using namespace std;
using namespace etk;


/**
 * C-styled arrays are typically passed to functions as pointers.
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
	
	//some alternative constructors/initialisers
	//note that the number of parameters to the constructor can be between zero and the length of the array
	//providing more parameters will cause a compile time error
	//auto a = Array<int, 5>(1, 2, 3); //this is valid
	auto a = Array<int, 5>(1, 2, 3, 4, 5);
	for(auto i : a)
	{
		cout << i << " ";
	}
	cout << endl;

	Array<int, 5> b = {1, 2, 3, 4, 5};

	//warning: when initialising from an existing array or pointer, the etk::Array class expects 
	//the existing array is of the same size
	int c_array[] = {1, 2, 3, 4, 5};
	Array<int, 5> c = c_array;
}

