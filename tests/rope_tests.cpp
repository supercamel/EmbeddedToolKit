#include "rope_tests.h"
#include "out.h"
#include <etk/etk.h>
#include <sstream>
#include <iomanip>
#include <limits>

#include <iostream>
using namespace std;

using namespace etk;

bool test_rope(std::string& subtest)
{
	subtest = "Constructor";
    char buf[20];
    etk::Rope rope(buf, 20, "Hello!");

    if(rope != "Hello!")
    {
        return false;
    }

    rope.clear();

    subtest = "Appending unsigned integer";
    rope.append(556u);
    if(rope.compare("556") == false) 
    {
        return false;
    }
    
    rope.clear();

    subtest = "Appending unsigned int with padding";
    rope.append(556u, 5);
    if(rope.compare("00556") == false) {
        return false;
    }
    
    rope.clear();
    
    subtest = "Appending min int";
    #undef min
    rope.append(numeric_limits<int32_t>::min());
    if(rope.compare("-2147483648") == false)
    	return false;
    rope.clear();

    subtest = "Appending positive signed integer";
    rope.append(565);
    if(rope.compare("565") == false) {
        return false;
    }

    rope.clear();
    
    subtest = "half max int32";
    auto test_val = 1073741823;
    rope.append(test_val);
    if(rope.compare("1073741823") == false) {
    	return false;
    }
    rope.clear();

    subtest = "Appending real_t";

    for(real_t i = -10; i < 10; i += 0.01)
    {
        if(etk::fabs(i) < 0.005)
            i = 0.0;
        std::stringstream ss;
        ss << setprecision(2) << fixed << i;

        rope.append(i);
        if(!rope.compare(ss.str().c_str()))
            return false;
        rope.clear();

    }
	
    rope.clear();
    rope.append(0.0f);
    if(rope.compare("0.00") == false) {
        return false;
    }
    
    rope.clear();

    rope.append(0.05f);
    if(rope.compare("0.05") == false) {
        return false;
    }
    
    rope.clear();
    
    subtest = "Appending huge double";
    rope.append(1200000000.0, 9);
    rope.clear();

    subtest = "Appending double";

    rope.append(-624.64);

    if(rope.compare("-624.64") == false) {
        return false;
    }

    subtest = "Comparison false positive";
    if(rope.compare("-624.646") == true) {
        return false;
    }
    rope.clear();

    subtest = "<< syntax appending double";
    rope << -624.6434;
    if(rope.compare("-624.64") == false) {
        return false;
    }

    if(rope.compare("-624.643")) {
        return false;
    }

    rope.clear();

    subtest = "appending double with precision";
    rope.append(12.5673, 6);
    if(rope.compare("12.567300") == false) {
        return false;
    }

    rope.clear();

    subtest = "Appending integer";
    rope << 52;
    if(rope.atoi() != 52) {
        return false;
    }
    rope.clear();

    subtest = "Appending a huge integer";
    rope << -156424634;
    if(rope.atoi() != -156424634) {
        return false;
    }

    subtest = "Rope length";
    if(rope.length() != 10) {
        return false;
    }

    subtest = "Assigning ropes";
    etk::Rope r2 = rope;
    if(r2.length() != 10) {
        return false;
    }

    subtest = "Assigning string of excessive length";
    rope.clear();
    rope << "hello world this is an overly long string";
    if(rope.compare("hello world this is") == false) {
        return false;
    }

    subtest = "Extracting sub strings";
    char b2[20];
    rope.sub_string(b2, 6, 5);
    etk::Rope rr(b2, 20);
    if(rr.compare("world") == false) {
        return false;
    }

    subtest = "Comparing sub strings";
    if(rope.compare("hello", 5) == false) {
        return false;
    }

    if(rope.compare(" world", 6, 1, 5) == false) {
        return false;
    }

    if(rope.compare(rr, 6, 0, 5) == false) {
        return false;
    }

    subtest = "atof";
    rope.clear();
    rope << 53.24;
    if(compare(rope.atof(), 53.24f, 0.000001f) == false) {
        return false;
    }


    rope.clear();
    rope << "2710.000";
    if(compare(rope.atof(), 2710.00, 0.0001f) == false) {
        return false;
    }


    subtest = "atof NAN";
    rope.clear();
    rope << NAN;
    if(etk::is_nan(rope.atof()) == false) {
        return false;
    }

    subtest = "atof INF";
    rope.clear();
    rope << INFINITY;
    if(etk::is_inf(rope.atof()) == false) {
        return false;
    }

    rope.clear();
    rope << 0.000000000003f;
    if(compare(rope.atof(), 0.0f, 0.0001f) == false) {
        return false;
    }

    subtest = "appending zero";
    rope.clear();
    rope << 0;
    if(rope.compare("0") == false) {
        return false;
    }

    subtest = "Atoi at a place";
    rope.clear();
    rope << "Seven 7 yay!";
    if(rope.atoi(6) != 7) {
        return false;
    }

    subtest = "set cursor";
    rope.set_cursor(6);
    rope << 8;
    if(rope.atoi(6) != 8) {
        return false;
    }

    subtest = "Appending characters";
    rope.clear();
    rope << 'c';
    if(rope != "c") {
        return false;
    }
        
    subtest = "parse hex";
    rope.clear();
    rope << "af";
    uint32 r = rope.parse_hex(0);
    
    rope.clear();
    rope.make_hex((uint8)0x6F);
    

	subtest = "making hex strings from uint16 type";
	uint16_t h = 0xF842;
	rope.clear();
	rope.make_hex(h);

	if(rope != "F842")
	{
		return false;
	}

	h = 0x3F;
	rope.clear();
	rope.make_hex(h);

	cout << buf << endl;
	if(rope != "3F")
	{
		return false;
	}

    return true;
}
