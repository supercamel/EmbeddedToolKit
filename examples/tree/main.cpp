#include <etk/etk.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>


using namespace std;
using namespace etk;


struct node;
typedef ObjPool<node, 1024*3> pool_type;
typedef PoolPtr<pool_type, node> node_ptr;

const int max_depth = 5;


//the list node object
struct node
{
	uint32_t segment_length;
	List<node_ptr, 3> children;
};

//the object pool
pool_type pool;

//string to store tree as ascii
StaticString<1024*1024> ascii_nodes;

List<List<int, max_depth+1>, 1024*3> pathways;


int get_path_length(node_ptr ptr, List<int, max_depth+1>& path, bool print_out=false)
{
	if(ptr)
	{
		int child = path[0];
		path.remove(0);
		if(print_out)
			cout << ptr->segment_length << "\t";
		return ptr->segment_length+get_path_length(ptr->children[child], path, print_out);
	}
	return 0;
}


List<int, max_depth+1> find_shortest_path(node_ptr head)
{
	int shortest_length = 1000000; //all pathways are shorter than a million, so make this default value
	List<int, max_depth+1> shortest_path;
	for(auto& path : pathways)
	{
		auto tpath = path;
		int path_length = get_path_length(head, tpath);
		if(path_length < shortest_length)
		{
			shortest_path = path;
			shortest_length = path_length;
		}
	}
	
	return shortest_path;
}


void generate_all_pathways(node_ptr ptr, List<int, max_depth+1> path)
{
	if(!ptr)
		return;
	
	int n_children = ptr->children.size();
	if(n_children == 0) //this is a leaf node
	{
		pathways.append(path); //add pathway to pathways list
		return;
	}
	
	for(int i = 0; i < n_children; i++)
	{
		path.push_back(i);
		generate_all_pathways(ptr->children[i], path); //this will deep-copy the path list
		path.pop_back();
	}
}

//generates a read-able version of tree
void node_to_text(node_ptr ptr)
{
	static int recursion_depth = 0;
	
	if(!ptr)
		return;

	recursion_depth++;
	
	for(int i = 0; i < recursion_depth; i++)
		ascii_nodes += "\t-\t";
	
	ascii_nodes += ptr->segment_length;
	ascii_nodes += "\n";
	
	for(auto& child : ptr->children)
		node_to_text(child);
	
	recursion_depth--;
}

//recursively allocates children to the nodes
//and gives each node a random length
void add_child_nodes(node_ptr ptr)
{
	static int recursion_depth = 0;
	
	if(!ptr)
	{
		cout << "Allocations are failing. The pool is too small." << endl;
		return;
	}
		
	ptr->segment_length = rand() % 100 + 1;
	
	if(recursion_depth > max_depth)
		return;
	
	recursion_depth++;
	
	for(int i = 0; i < 3; i++)
	{
		ptr->children.append(pool.alloc());
		add_child_nodes(ptr->children[i]);
	}
	
	recursion_depth--;
}


int main()
{
	cout << "Pool size is " << sizeof(pool_type) << " bytes" << endl;
	cout << "Randomly generating a tree . . . " << endl;
	srand(time(NULL));
	//allocate a head node
	auto head = pool.alloc();
	head->segment_length = 0;
	
	add_child_nodes(head);
	node_to_text(head);
	
	cout << "Saving tree to 'tree.txt' . . . " << endl;
	ofstream node_dump("tree.txt");
	node_dump << ascii_nodes.c_str();
	node_dump.close();
	
	cout << "Calculating all possible pathways . . . " << endl;
	List<int, max_depth+1> path_head;
	generate_all_pathways(head, path_head);
	
	cout << "To take the shortest path, follow these steps: \n0\t";
	auto shortest_path = find_shortest_path(head);
	for(auto i : shortest_path)
		cout << i << "\t";
	cout << endl;
	
	cout << "\n\nThe total length is: " << get_path_length(head, shortest_path, true) << endl;
}


