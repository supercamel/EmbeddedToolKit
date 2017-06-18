/*
 * etk::Bits will read/write bits from an integer.
 * It's easier than using bitwise operators and is just as efficient. 
 */

#include <etk/etk.h>
#include <iostream>
#include <bitset>


using namespace std;
using namespace etk;


int main()
{
	etk::Bits8 bits;
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


