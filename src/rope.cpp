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


#include <etk/rope.h>
#include <etk/math_util.h>
#include <math.h>

namespace etk
{


Rope::Rope(char* buf, uint32 maxlen, const char* c)
{
    str = buf;
    pos = 0;
    N = maxlen;
    uint32 i = 0;
    for(; i < maxlen; i++)
    {
        str[i] = c[i];
      	if(c[i] == '\0')
          break;
    }
    str[i] = '\0';
    pos = i;
}

Rope::Rope(char* c, uint32 maxlen)
{
    pos = 0;
    N = maxlen;
    str = c;
}

Rope::Rope(Rope& r)
{
    pos = r.pos;
    N = r.N;
    str = r.str;
}

Rope::Rope(const Rope& r)
{
    pos = r.pos;
    N = r.N;
    str = r.str;
}

void Rope::append(char c)
{
    if(pos < N-1)
        str[pos++] = c;
}

void Rope::append(const char* s, int len)
{
    if(len == 0)
        len = min(N-pos-1, Rope::c_strlen(s, N));
    else
        len = min(int(N-pos-1), len);
    for(int i = 0; i < len; i++)
        str[pos++] = s[i];
    str[pos] = '\0';
}

void Rope::append(int32 j, uint32 npad)
{
    if(j < 0)
    {
    	if(j == -2147483648) //special case for j == min int32
    	{
    		append("-2147483648");
    		return;
    	}
        append('-');
        j *= -1;
    }
    append((uint32)j, npad);
    terminate();
}

void Rope::append(int64 j, uint32 npad)
{
    if(j < 0)
    {
        if(j == (-9223372036854775807-1))
        {
            append("–922337203685477808");
            return;
        }
        append('-');
        j *= -1;
    }
    append((uint64)j, npad);
    terminate();
}

void Rope::append(uint32 j, uint32 npad)
{
    char buf[12];
    uint32 i = 0;
    for(i = 0; i < 12; i++)
    {
        int r = j%10;
        j /= 10;
        buf[11-i] = '0'+static_cast<char>(r);
    }
    i = 0;
    while(i < 12)
    {
        if(buf[i] != '0')
            break;
        i++;
    }
    i = min(i, 12-npad);
    append(&buf[i], 12-i);
    terminate();
}

void Rope::append(uint64 j, uint32 npad)
{
    char buf[60];
    uint32 i = 0;
    for(i = 0; i < 60; i++)
    {
        int r = j%10;
        j /= 10;
        buf[59-i] = '0'+static_cast<char>(r);
    }
    i = 0;
    while(i < 60)
    {
        if(buf[i] != '0')
            break;
        i++;
    }
    i = min(i, 60-npad);
    append(&buf[i], 60-i);
    terminate();
}

void Rope::append(float j, uint8 precision)
{
    if(isnan(j))
    {
        append("nan", 3);
        return;
    }
    if(isinf(j))
    {
        append("inf", 3);
        return;
    }

    precision &= 0x0F;

    uint64 mul = pow(10, precision);

    float r = j*mul;
    const int64 max_int64 = 9223372036854775807;
    if(r >= max_int64)
    {
    	append("ovr", 3);
    	return;
    }

    int64 t = static_cast<int64>(roundf(r));
    char b[20];
    Rope sb(b, 20);
    sb.clear();
    sb.append(t, static_cast<uint32>(precision)+1);
    uint64 len = max((int)sb.length()-precision, 0);
    append(sb.c_str(), len);
    append('.');
    append(&sb.c_str()[len], sb.length()-len);
    terminate();
}

void Rope::append(double d, uint8 precision)
{
    append(static_cast<float>(d), precision);
}

void Rope::append(Rope sb, uint16 len)
{
    if(len < 1)
        len = sb.length();
    append(sb.c_str(), len);
}

uint32 Rope::length()
{
    for(uint32 i = 0; i < N; i++)
    {
        if(str[i] == '\0')
            return i;
    }
    return N;
}

Rope& Rope::operator << (const char* s)
{
    append(s);
    return *this;
}

Rope& Rope::operator << (int32 i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (uint32 i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (int64 i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (uint64 i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (float i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (double d)
{
    append(d);
    return *this;
}

Rope& Rope::operator << (char i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (Rope& s)
{
    append(s);
    return *this;
}

Rope& Rope::operator + (Rope& s)
{
    append(s);
    return *this;
}

Rope& Rope::operator += (Rope& s)
{
    append(s);
    return *this;
}

Rope& Rope::operator + (char* s)
{
    append(s);
    return *this;
}

Rope& Rope::operator += (char* s)
{
    append(s);
    return *this;
}

Rope& Rope::operator + (const char* s)
{
    append(s);
    return *this;
}

Rope& Rope::operator += (const char* s)
{
    append(s);
    return *this;
}

Rope& Rope::operator = (const char* s)
{
    pos = 0;
    uint32 l = c_strlen(s, N);
    /*
    for(uint32 i = 0; i < l; i++)
        append(s[i]);
    terminate();
    */
    append(s, l);
    return *this;
}

Rope Rope::operator = (Rope r)
{
    pos = r.pos;
    N = r.N;
    str = r.str;
    return *this;
}

char& Rope::operator [](uint16 p)
{
    return str[p];
}

char Rope::get(uint16 p)
{
    return str[p];
}

bool Rope::operator == (Rope r) const
{
    return compare(r);
}

bool Rope::operator != (Rope r) const
{
    return (!compare(r));
}

bool Rope::operator == (const char* r) const
{
    return compare(r);
}

bool Rope::operator != (const char* r) const
{
    return (!compare(r));
}

bool Rope::compare(Rope r, uint32 len) const
{
    if(len == 0)
        len = N;

    for(uint32 i = 0; i < len; i++)
    {
        if(str[i] != r.str[i])
            return false;
        if(str[i] == '\0')
        	break;
    }
    return true;
}

bool Rope::compare(const char* c, uint32 len) const
{
    if(len == 0)
        len = N;

    for(uint32 i = 0; i < len; i++)
    {
        if(str[i] != c[i])
            return false;
        if(str[i] == '\0')
        	break;
    }
    return true;
}

bool Rope::compare(Rope r, uint32 start_this, uint32 start_that, uint32 len) const 
{
    for(uint32 i = 0; i < len; i++)
    {
        if(str[i+start_this] != r.str[start_that+i])
            return false;
    }
    return true;
}

bool Rope::compare(const char* c, uint32 start_this, uint32 start_that, uint32 len) const
{
    for(uint32 i = 0; i < len; i++)
    {
        if(str[i+start_this] != c[start_that+i])
            return false;
    }
    return true;
}

void Rope::sub_string(char* buf, uint32 start, uint32 len) const
{
    uint32 i = 0;
    for(; i < len; i++)
        buf[i] = str[i+start];
    buf[i] = '\0';
}

void Rope::sub_string(Rope& r, uint32 start, uint32 len) const
{
    sub_string(r.str, start, min(len+start, r.N));
}


uint32 Rope::parse_hex(uint32 start) const
{
	uint32 ret = 0;
	for(uint32 i = start; i < N; i++)
	{
		char c = to_upper(str[i]);
		uint32 val = 0;
		if((c >= '0') && (c <= '9'))
			val = c-'0';
		else if((c >= 'A') && (c <= 'F'))
			val = 10+(c-'A');
		else
			break;
			
		ret *= 16;
		ret += val;
	}
	
	return ret;
}

void Rope::make_hex(uint8 byte)
{
	uint8 nibble = (byte>>4);

	if(nibble < 10)
		append((char)('0'+nibble));
	else
	{
		nibble -= 10;
		append((char)('A'+nibble));
	}

	nibble = (byte&0x0F);

	if(nibble < 10)
		append((char)('0'+nibble));
	else
	{
		nibble-=10U;
		append((char)('A'+nibble));
	}
}


const char* Rope::c_str()
{
    return (const char*)str;
}

void Rope::clear()
{
    for(uint32 i = 0; i < N; i++) str[i] = '\0';
    pos = 0;
}

int Rope::atoi(uint32 p)
{
    int res = 0,n=1;
    char* pstr = &str[p];
    if(*pstr =='-')
    {
        n=-1;
        pstr++;
    }
    while (is_numeric(*pstr))
        res = res * 10 + *pstr++ - '0';
    return res*n;
    //return ::atoi(&str[p]);
}

float Rope::atof(uint32 ps)
{
    if(compare("nan", 3))
        return NAN;
    if(compare("inf", 3))
        return INFINITY;

    char* p = &str[ps];
    float sign, value;

    sign = 1.0;
    if (*p == '-')
    {
        sign = -1.0;
        p++;
    }
    else if (*p == '+')
        p++;

    for (value = 0.0; is_numeric(*p); p++)
        value = value * 10.0 + (*p - '0');

    // Get digits after decimal point, if any.
    if (*p == '.')
    {
        float pow10 = 10.0;
        p++;
        while ((*p >= '0') && (*p <= '9'))
        {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p++;
        }
    }
    // Return signed and scaled floating point result.
    return sign * value;
}

void Rope::copy(char* c, uint32 len) const
{
    if(len == 0)
        len = N;
    for(uint32 i = 0; i < len; i++)
        c[i] = str[i];
}

char* Rope::get_buffer()
{
    return str;
}

uint32 Rope::c_strlen(const char* c, uint32 maxlen)
{
    for(uint32 i = 0; i < maxlen; i++)
    {
        if(c[i] == '\0')
            return i;
    }
    return maxlen;
}

void Rope::terminate()
{
    if(pos < N)
        str[pos] = '\0';
    else
        str[N-1] = '\0';
}


}
