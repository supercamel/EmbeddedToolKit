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
#include <iostream>


#define LF 10
#define CR 13
#define NULC  (char)(0)
// This string contains all the printable/keyable characters used in the kernel
const char ChkStr[] ={
	"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ -+*/%&|^:=()[]$\",.;<>"
};

namespace etk
{

template <class derived> class Stream
{
public:
	//StaticString<84> ChkStr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ -+*/%&|^:=()[]$\",.;<>";
	//etk::StaticString<128> line;
	char linebuf[100];  // Line buffer used for command line input
	char *cline = linebuf;

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

	char *strchr(char *s, char c){
		int i=0;
		std::cout << "Enter----->: " << s[i] << std::endl;
		for ( i=0; s[i]!='\0'; i++){
			std::cout << s[i] << std::endl;
			if(s[i]==c){
				s = s+i;
				std::cout << "Returning" << std::endl;
				return s;
			}
		}
		return 0;
	}

	void putChar( char c){
		print(c);
	}

	unsigned int getChar(){
		//platform Oriented getChar()
		//for desktop version include <iostream> and use getchar()
		return getchar();
	}

	int getLine(){
		unsigned int c;
		c = getChar();
		switch(c){
			case 0x0: return(-1);
			case 0x100 : return(-1); /* Nothing entered */
			case 27 : return(-2); /* Escape key      */
			case 1  : return(-3); /* <Ctrl A> key    */
			case 8  : /* <Backspace>     */
				*cline-- = (char)0;
				*cline = (char)0;
				//printBackspace();
				return(0);
			default : /* <Enter>: line ready to be processed */
				if ((c == CR) | (c == LF)){
					*cline = NULC;
					cline = linebuf;
					return(1);
				}
				/* If valid character, print and put character in line buffer */
				std::cout << "Debug" << std::endl;
				if(strchr((char *)ChkStr,(char)c)!= 0 /*NULC*/){
					*cline++ = (char)c;
					*cline = NULC;
					std::cout << "Debug" << std::endl;
					putChar(c);

					return(0);
				}
				else{
					return (-1);
				}
		}
	}

};


}


#endif
