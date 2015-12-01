#ifndef OUT_H_INCLUDED
#define OUT_H_INCLUDED

#include <etk/etk.h>
#include <iostream>

class Out : public etk::Cout
{
public:
	Out() { }
	~Out() { flush(); }

	void write(const char* buf)
	{
		std::cout << buf << std::flush;
	}
private:

};


extern Out out;


#endif

