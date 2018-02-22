#ifndef OUT_H_INCLUDED
#define OUT_H_INCLUDED

#include <etk/etk.h>

#include <iostream>

/*
 * etk::Stream sucks up strings, floats and integers and turns them into a stream of characters.
 * How these characters are displayed/transmitted/stored is implemented by <i>void put(char c);</i>
 *
 * etk::Stream uses two nifty C++ techniques to turn stuff into a character stream. The first is chomp operator overloading.
 * This means you can use your streamable class similar to how you could use std::cout
 * 
 *     stream << "A string " << 58 << " " << 123.45 << "\n";
 *
 * It also uses a variadic template function called 'print'. You can call 'print' with string, int and float arguments of any length and in any order.
 *
 * 	   stream.print("A string", 58, " ", 123.45, "\n");
 *
 *
 * To make a class streamable, you can inherit the etk::Stream class using CRTP
 *
 * https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Curiously_Recurring_Template_Pattern
 *
 * All you need to do is implement a 'put' function which determines how characters are dealt with.
 * In this case, we're simply displaying character using stdout.
 * In an embedded environment, 'put' could transmit via UART or display onto an LCD screen.
 *
 */
 
class Stream : public etk::Stream<Stream>
{
public:
	
	void put(char c)
    {
        std::cout << c;
    }

	uint32 available()
	{
		int32 r = buf_len - buf_pos;
		if(r > 0)
			return r;
		return 0;
	}

	char get()
	{
		if(buf_pos < buf_len)
			return in_buf[buf_pos++];
		return 0;
	}

	void reset()
	{
		buf_pos = 0;
	}

private:
	char* in_buf = "A line\nA number: 86\naaa23.5";
	int buf_len = etk::Rope::c_strlen(in_buf, 256);
	int buf_pos = 0;

};


extern Stream stream;


#endif

