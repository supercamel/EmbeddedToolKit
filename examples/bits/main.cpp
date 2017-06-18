/*
 * A memory pool is just a blob of memory that can be used to dynamically 
 * allocate memory a bit like new or malloc. 
 */

#include <etk/etk.h>
#include <iostream>
#include <bitset>


using namespace std;
using namespace etk;


int main()
{
	etk::Bits<uint8> bits;
	bits.set_bit(0, true);
	bits.set_bit(3, true);
	
	char buf[4];
	etk::Rope rope(buf, 4);
	rope.make_hex(bits.get());
	cout << "0x" << buf << " " << bits.to_string().c_str() << endl;
	
	std::bitset<8> b;
	b = 0x09;
	cout << b << endl;
}


