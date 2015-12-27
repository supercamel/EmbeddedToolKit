#include "bits_test.h"
#include <iostream>
using namespace std;

bool bits_test(std::string& subtest)
{
	subtest = "Setting bits";
    //create an 8bit bits object
    etk::Bits<uint8_t> bits;

    //set the first bit to ON
    //set_bit only returns false if the bit to set is out of range
    //  (ie. setting bit 9 of an 8bit object)
    //there is no reason why this should ever fail
    if(!bits.set_bit(0, true))
        return false;

    //set the last bit to on
    //set bit takes a default argument, so it turns the bit on by default
    //again, no reason why this should ever fail
    if(!bits.set_bit(7))
        return false;

    //confirm that bit 7 was actually set
    if(!bits.read_bit(7))
        return false;

    //now turn the bit off again
    if(!bits.set_bit(7, false))
        return false;
    //confirm that bit 7 was actually turned off
    if(bits.read_bit(7))
        return false;

    //now we try setting an out of range bit
    //set_bit must return false here
    if(bits.set_bit(8))
        return false;


	subtest = "To string";
    etk::StaticString<9> s = bits.to_string();
    cout << s.c_str() << endl;
    if(!s.compare("10000000", 8))
        return false;

    if(!bits.set_bit(0, false))
        return false;

	subtest = "to string again";
    s = bits.to_string();
    if(!s.compare("00000000", 8))
        return false;

    return true;
}


