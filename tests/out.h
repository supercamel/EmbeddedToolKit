#ifndef OUT_H_INCLUDED
#define OUT_H_INCLUDED

#include <etk/etk.h>
#include <iostream>

class Out : public etk::Stream<Out>
{
public:
	void put(char c)
	{
	    std::cout << c;
	}
private:

};


extern Out out;


#endif

