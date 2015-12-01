#include "string_test.h"
#include <etk/string.h>
#include <iostream>
using namespace std;

using namespace etk;

const uint32_t blob_sz = 1024*1024;
uint8_t blob[blob_sz];

MemPool pool(blob, blob_sz);


bool tst()
{

	String str(pool);
	str = "Hello world!";

	String s2(pool, " I'm a super extremely very ultra longer than 32 byte string. Probably a lot longer than 32 bytes I'd say.");

	String str3(pool);

	str3 += str;
	str3 += s2;

	return true;
}

bool string_test(std::string& subtest)
{
	subtest = "string construction";
	tst();

	return true;
}


