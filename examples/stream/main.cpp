#include "stream.h"


int main()
{
	stream << "Hello world!\n";

	stream.print("Hello world!\n", 56, " ", 58.354, "\n");
	
	while(!stream.getLine()){};
	stream << "input: ";
	stream.putInput();
	stream << "\r\n";
}
