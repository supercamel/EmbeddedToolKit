#include "static_string_tests.h"
#include <etk/staticstring.h>
#include <etk/tokeniser.h>
#include <etk/loop_range.h>
#include <iostream>
using namespace std;

using namespace etk;

bool static_string_test(std::string& subtest)
{
	etk::StaticString<128> string;
	subtest = "Testing assignment";

	string = "Test";
	if(string.length() != 4)
		return false;

	subtest = "Sub string";
	string = "Hello world!";

	subtest = "Sub strings";
	etk::StaticString<64> ts;
	string.sub_string(ts, 6, 5);

	if(!ts.compare("world"))
		return false;

	subtest = "Appending and comparing";
	etk::StaticString<128> st = "Hello ";
	st += ts.c_str();
	st += "!";

	if(!st.compare(string))
		return false;

	subtest = "Inserting";
	st.insert('a', 0);
	if(!st.compare("aHello", 6))
		return false;

	subtest = "Removing";
	st.remove(0);
	if(!st.compare("Hello", 5))
		return false;

	st.remove(1);
	if(!st.compare("Hllo", 4))
		return false;

	subtest = "Erasing";
	st = "Hello";
	st.erase(2, 2);
	if(!st.compare("Heo", 3))
		return false;

	subtest = "Assigning string to string";
	string = "Woohoo!";
	st = string;
	if(!st.compare(string))
		return false;

	subtest = "Atoi";
	string = "565";
	if(string.atoi() != 565)
		return false;

	subtest = "Atof";
	string = "5.65";
	if(!compare<float>(string.atof(), 5.65, 0.001))
		return false;

	subtest = "subscript access";
	string[0] = '6';
	if(!compare<float>(string.atof(), 6.65, 0.001))
		return false;

	subtest = "out of bounds subscript access";
	for(int i = -55; i < 10000; i++)
		string[i] = '4';

	subtest = "comparison operators";
	string = "hello";
	if(string != "hello")
		return false;

	if(string == "heloo")
		return false;

	if((string == "hello") == false)
		return false;

	subtest = "Constructors";
	char b[20];
	etk::Rope rope(b, 20);
	rope << "G'day g'day";
	etk::StaticString<20> gday(rope);

	if(gday != "G'day g'day")
		return false;

	subtest = "Rope operators";
	string.clear();
	string += 6.65;
	if(!compare<float>(string.atof(), 6.65, 0.001))
		return false;

	subtest = "to upper";
	string = "hi";
	string.to_upper();
	if(string != "HI")
		return false;

	subtest = "to lower";
	string.to_lower();
	if(string != "hi")
		return false;

	subtest = "clearing";
	string.clear();
	string += "Hello world";

    subtest = "Tokeniser";
	std::string tokens = "Tokens are awesome";
	auto tok = make_tokeniser(tokens, ' ');

    tok.next(gday, 20);
    if(gday != "Tokens")
        return false;
    tok.next(gday, 20);
    if(gday != "are")
        return false;
    tok.next(gday, 20);

    subtest = "Tokenising c-strings";
    char trr[256] = "Tokens are awesome";
    char tbuf[20];
    auto cstok = make_tokeniser(trr, ' ');

    cstok.next(tbuf, 20);

    rope.clear();
    rope << tbuf;

    if(!rope.compare("Tokens", 6))
        return false;
    cstok.next(tbuf, 20);
    cstok.next(tbuf, 20);

    StaticString<100> ss = "Hello world";
    ss.remove(0);
    ss.insert('Y', 0);
    ss.insert('w', 5);
    if(ss != "Yellow world")
        return false;

    ss = "Giggle";
    ss.fill('0', 1, 2);
    if(ss != "G00gle")
        return false;

	return true;
}


