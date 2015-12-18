#ifndef OUT_H_INCLUDED
#define OUT_H_INCLUDED

#include <etk/etk.h>

class Out : public etk::Cout
{
public:
	Out() { }
	~Out() { flush(); }

	void write(const char* buf);
private:
	
};


extern Out out;


#endif

