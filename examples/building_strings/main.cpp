#include <etk/etk.h>
#include <iostream>


using namespace std;
using namespace etk;

int main()
{
	StaticString<120> ss;
	auto rope = ss.get_rope();
	rope << "Today the temp is " << 23.6 << " degrees C";
	
	cout << ss.c_str() << endl;
}

