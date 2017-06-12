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

};


}


#endif
