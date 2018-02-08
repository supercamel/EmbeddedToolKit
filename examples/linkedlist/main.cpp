#include <etk/etk.h>
#include <iostream>

/*
	This example demonstrates use of Object Pools. 
	
	As with everything in ETK, object pools do not dynamically allocate heap memory and cannot cause memory fragmentation. 
	
	Object pools contain and manage a pool of memory. They can only allocate one type of object from the pool. The object type is specified by the template parameter.
	
	When pool.alloc() is called, it constructs the object and returns a special 'pool pointer'. This pool pointer is a reference counting smart pointer. 
	It's purpose is to know when an allocation is no longer required, and return it to the pool.
	If all pool pointers to a particular allocation go out of scope or are otherwise destructed, the allocation is returned to the pool and the objects destructor is called. 
	This makes it very difficult for a memory pool to 'leak' memory. 
	
	
	Object pools are great for building linked lists, tree structures and so on where all nodes are the same type of object. 
	
	
	This particular example uses a linked list. 
	
	OUTPUT:
	
	Head node 1 2 3 4 5 6 7 8 9 
	Head node 4 5 6 7 8 9 
	There are 3 objects available in the pool.

	
*/

using namespace std;
using namespace etk;

//pool pointer declarations are very long-winded, so i've typedef'd it down to something easier to read
struct node;
typedef PoolPtr<ObjPool<node, 10>, node> node_ptr;


//the list node object
struct node
{
	StaticString<20> text; //each node will contain some text
	node_ptr next; //and a pointer to the next node
};


int main()
{
	//declare a pool. this pool can allocate a maximum of 10 node objects
	ObjPool<node, 10> pool;
	
	//allocate a head node
	auto head = pool.alloc();
	head->text = "Head node";
	
	//this loop actually consumes all allocations from the pool
	auto next = head;
	int count = 1;
	while(next)
	{
		next->next = pool.alloc(); //get an allocation
		if(next->next == nullptr) //check if the call to pool.alloc() has worked
			break; //if not, break the loop
			
		next->text = count; //set the text to node counter
		next = next->next; //move on to next node
		count++; //increment node counter
	}
	
	//now to print all of the nodes out
	//reset to the head node
	next = head;
	while(next) //while the node pointer is valid
	{
		cout << next->text.c_str() << " "; //print it's text
		next = next->next; //and move to the next node
	}
	cout << endl;
	
	//we're going to unlink a few nodes to prove memory management works
	//reset to head again
	next = head;
	for(int i = 0; i < 4; i++) //this loop counts to the fourth node
		next = next->next;
	
	head->next = next; //then assigns the fourth node to head->next.
	//this sets off a little chain reaction. the original head->next now has no pointers pointing at it, so it's destructed and returned to the pool.
	//head->next->next now has no pointers pointing at it, so it's destructed and returned to the pool.
	//head->next->next->next ditto
	//head->next->next->next->next is pointed to by what is now head->next, so it's ok.
	
	
	//print off the list again. this time 1,2 and 3 are missing
	next = head;
	while(next)
	{
		cout << next->text.c_str() << " ";
		next = next->next;
	}
	cout << endl;
	
	//and there's three allocations availabe in the pool.
	cout << "There are " << pool.n_available() << " objects available in the pool." << endl;
}


