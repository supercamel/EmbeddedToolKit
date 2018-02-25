#include "stream.h"


int main()
{
	stream << "Hello world!\n";
	
	stream.print("Hello world!\n", 56, " ", 58.354, "\n");

	char buf[128];
	stream.getline(buf, 128);
	stream << buf << "\n";

	stream.reset();
	etk::StaticString<128> ss;
	stream.read(ss);
	stream << ss;
}

