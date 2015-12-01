#include <etk/stack.h>

#include <iostream>
using namespace std;

using namespace etk;


extern MemPool pool;


bool stack_test(std::string& subtest)
{
	subtest = "push pop";

	Stack<int> stack(pool);
	stack.push(100);
	if(stack.pop() != 100)
		return false;

	subtest = "bulk push pop";
	for(int i = 0; i < 10000; i++)
		stack.push(i);

	for(int i = 9999; i != 0; i--)
	{
		int p = stack.pop();
		if(p != i)
			return false;
	}
	return true;
}


