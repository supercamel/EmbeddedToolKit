/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/


#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include "rope.h"
#include "staticstring.h"
#include "vector.h"
#include "matrix.h"

#define LF 10
#define CR 13
#define INPUT_BUFFER_MAX_SIZE 80

namespace etk
{

template <class derived> class Stream
{
public:
    void print(const char* cstr)
    {
        while(*cstr != '\0')
            static_cast<derived*>(this)->put(*cstr++);
    }

    void print(char* cstr)
    {
        print((const char*)(cstr));
    }

    template<uint32 L> void print(const StaticString<L> ss)
    {
        auto len = ss.length();
        for(uint32 i = 0; i < len; i++)
            static_cast<derived*>(this)->put(ss[i]);
    }

    template<uint32 L> void print(const Vector<L> v)
    {
        for(uint32 i = 0; i < L; i++)
            print(v[i], " ");
        print("\r\n");
    }

    template<typename T> void print(T v)
    {
        char buf[20];
        etk::Rope rope(buf, 20);
        rope << v;
        for(uint32 i = 0; i < rope.length(); i++)
            static_cast<derived*>(this)->put(buf[i]);
    }

    template<typename T, typename... Args> void print(T first, Args... args)
    {
        print(first);
        print(args...);
    }

    template<typename T> Stream& operator << (T& a)
    {
        print(a);
        return *this;
    }

    Stream& operator << (const char* cstr)
    {
        print(cstr);
        return *this;
    }

    Stream& operator << (char* cstr)
    {
        print(cstr);
        return *this;
    }

    template<uint32 L> Stream& operator << (StaticString<L> ss)
    {
        print(ss);
        return *this;
    }

	int getLine(){
		// This string contains all the printable/keyable characters used in the kernel
		char validChar[85]={"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ -+*/%&|^:=()[]$\",.;<>"};
		Rope ChkStr((char *)validChar,85);

		char c;
		c = static_cast<derived*>(this)->get();
		switch(c){
			case 0x0: return(-1);
			case 0x100 : return(-1); 	/* Nothing entered */
			case 27 : return(-2); 		/* Escape key      */
			case 1  : return(-3); 		/* <Ctrl A> key    */
			case 8  : 					/* <Backspace> */
				inputBuff.set_cursor(inputBuff.get_cursor()-1);
				return(0);
			default : /* <Enter>: line ready to be processed */
				if ((c == CR) | (c == LF)){
					inputBuff << '\0';
					return(1);
				}
				if(ChkStr.strchr(c)!= nullptr){
					inputBuff.append(c);
					return(0);
				}
				else{
					return (-1);
				}
		}
	}

	void putInput(){
		print(inputBuff);
	}
	
private:
	char inputBuffer[INPUT_BUFFER_MAX_SIZE];
	Rope inputBuff = Rope(inputBuffer,INPUT_BUFFER_MAX_SIZE);

};


}


#endif
