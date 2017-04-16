/*
 * A memory pool is just a blob of memory that can be used to dynamically 
 * allocate memory a bit like new or malloc. 
 */

#include <etk/etk.h>
#include <iostream>


using namespace std;
using namespace etk;


experimental::MemPool<1024> pool;


class Obj
{
public:
	Obj(const char* hw)
	{
		cout << "Created an object!" << endl;
		cout << hw << endl;
	}
	
	~Obj()
	{
		cout << "object destroyed" << endl;
	}
	
	void foo()
	{
		cout << "Foo!" << endl;
	}
};


int main()
{
	auto ptr = experimental::make_pool_ptr<Obj>(pool, "Hello world!");
	
	//ALWAYS check pointers after allocation.
	//memory runs out, especially with small pools. 
	if(ptr)
		ptr->foo();
}


