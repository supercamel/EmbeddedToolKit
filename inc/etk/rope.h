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

#ifndef ROPE_H_INCLUDED
#define ROPE_H_INCLUDED

#include "types.h"

namespace etk
{

/**
 * \class Rope
 *
 * \brief WHY ROPE? It's like a string, only more robust.
 *
 * Rope is a C-string manipulation class.
 *
 * Many C-string functions are prohibited by JSF-AV standard.
 * Rope is designed to replace standard C string functions
 * such as strncmp, strcpy and sprintf
 *
 * It uses pre-allocated buffers instead of dynamic memory like
 * std::string, so it's relatively safe to use in embedded systems
 * with tight memory constraints.
 *
 * http://www.camelsoftware.com/blog/2015/12/11/ditching-c-libraries-concatenation-of-c-strings-without-stdio/
 */

class Rope
{
public:
    Rope(char* buf, uint32 maxlen, const char* c);
    Rope(char* c, uint32 maxlen);
    Rope(Rope& r);
    Rope(const Rope& r);


    void append(char c);
    void append(const char* s, int len = 0);
    //void append(int i, uint32 npad = 1);
    //void append(unsigned int i, uint32 npad = 1);
    void append(int32 j, uint32 npad = 1);
    void append(uint32 j, uint32 npad = 1);
    void append(int64 j, uint32 npad = 1);
    void append(uint64 j, uint32 npad = 1);
    void append(float j, uint8 precision = 2); //precision cannot be more than 15
    void append(double d, uint8 precision = 2); //precision cannot be more than 15
    void append(Rope sb, uint16 len = 0);

    uint32 length();

    Rope& operator << (const char* s);
    Rope& operator << (int32 i);
    Rope& operator << (uint32 i);
    Rope& operator << (int64 i);
    Rope& operator << (uint64 i);
    Rope& operator << (float i);
    Rope& operator << (double d);
    Rope& operator << (char i);
    Rope& operator << (Rope& s);

    Rope& operator + (Rope& s);
    Rope& operator += (Rope& s);

    Rope& operator + (char* s);
    Rope& operator += (char* s);

    Rope& operator + (const char* s);
    Rope& operator += (const char* s);

    Rope& operator = (const char*);
    Rope operator = (Rope r);

    char& operator [](uint16 p);
    char get(uint16 p);

    bool operator == (Rope r) const;
    bool operator != (Rope r) const;
    bool operator == (const char* r) const;
    bool operator != (const char* r) const;

    bool compare(Rope r, uint32 len = 0) const;
    bool compare(const char* c, uint32 len = 0) const;
    bool compare(Rope r, uint32 start_this, uint32 start_that, uint32 len) const;
    bool compare(const char* c, uint32 start_this, uint32 start_that, uint32 len) const;

    void sub_string(char* buf, uint32 start, uint32 len) const;
    void sub_string(Rope& r, uint32 start, uint32 len) const;

    uint32 parse_hex(uint32 start) const;
    void make_hex(uint8 byte);

    const char* c_str();

    void clear();

    int atoi(uint32 p=0);
    float atof(uint32 p=0);

    void set_cursor(uint32 p) {
        pos = p;
    }
    uint32 get_cursor() {
        return pos;
    }

    char* get_buffer();
    void set_buffer(char* b) {
        str = b;
    }

    void copy(char* b, uint32 len=0) const;

    static uint32 c_strlen(const char* c, uint32 maxlen);

private:
    void terminate(); //appends a null terminator
    uint32 pos;
    uint32 N;
    char* str;
};

}


#endif // ROPE_H_INCLUDED
