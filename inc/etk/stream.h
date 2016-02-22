/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include "rope.h"
#include "staticstring.h"
#include "vector.h"
#include "matrix.h"

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
	
    template<uint32 L> void print(StaticString<L> ss)
    {
        for(uint32 i = 0; i < ss.length(); i++)
            static_cast<derived*>(this)->put(ss[i]);
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

    Stream& operator << (auto& a)
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

};


}


#endif

