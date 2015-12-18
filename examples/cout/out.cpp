#include "out.h"
//#include <etk/rope.h>
#include <iostream>

/*
 * The purpose of etk::Cout is to provide a simple, C++-like method of printing text to SOMETHING.
 *
 * This example uses std::cout to print text to stdout.
 * In an embedded environment, you may use this to write to a UART or some other output instead. 
 */
 
void Out::write(const char* buf)
{
	std::cout << buf << std::flush;
	
	//Usart1.write(buf, Rope::c_strlen(buf));
}


Out out;

