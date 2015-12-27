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


#include <etk/rope.h>
#include <etk/math_util.h>
#include <math.h>

namespace etk
{


Rope::Rope(char* buf, uint16_t maxlen, const char* c)
{
    str = buf;
    pos = 0;
    N = maxlen;
    for(int i = 0; i < c_strlen(c, maxlen); i++)
        append(c[i]);
    Rope::terminate();
}

Rope::Rope(char* c, uint16_t maxlen)
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
    {
        str[pos++] = c;
    }
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

void Rope::append(int32_t j, uint32_t npad)
{
    if(j < 0)
    {
        append('-');
        j *= -1;
    }
    append((uint32_t)j, npad);
}

void Rope::append(int64_t j, uint32_t npad)
{
    if(j < 0)
    {
        append('-');
        j *= -1;
    }
    append((uint64_t)j, npad);
}

void Rope::append(uint32_t j, uint32_t npad)
{
    char buf[9];
    uint32_t i = 0;
    for(i = 0; i < 9; i++)
    {
        int r = j%10;
        j /= 10;
        buf[8-i] = '0'+static_cast<char>(r);
    }
    i = 0;
    while(i < 9)
    {
        if(buf[i] != '0')
            break;
        i++;
    }
    i = min(i, 9-npad);
    append(&buf[i], 9-i);
}

void Rope::append(uint64_t j, uint32_t npad)
{
    char buf[20];
    uint32_t i = 0;
    for(i = 0; i < 20; i++)
    {
        int r = j%10;
        j /= 10;
        buf[19-i] = '0'+static_cast<char>(r);
    }
    i = 0;
    while(i < 20)
    {
        if(buf[i] != '0')
            break;
        i++;
    }
    i = min(i, 20-npad);
    append(&buf[i], 20-i);
    terminate();
}

void Rope::append(float j, uint8_t precision)
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
    uint32_t mul = 1;
    for(int i = 0; i < precision; i++)
        mul *= 10;
    int32_t t = static_cast<int32_t>(roundf(j*mul));
    char b[20];
    Rope sb(b, 20);
    sb.clear();
    sb.append(t, 3);
    uint32_t len = max((int)sb.length()-precision, 0);
    append(sb.c_str(), len);
    append('.');
    append(&sb.c_str()[len], sb.length()-len);
    terminate();
}

void Rope::append(double d, uint8_t precision)
{
    append(static_cast<float>(d), precision);
}

void Rope::append(Rope sb, uint16_t len)
{
    if(len < 1)
        len = sb.length();
    append(sb.c_str(), len);
}

uint32_t Rope::length()
{
    for(uint32_t i = 0; i < N; i++)
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

Rope& Rope::operator << (int32_t i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (uint32_t i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (int64_t i)
{
    append(i);
    return *this;
}

Rope& Rope::operator << (uint64_t i)
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
    uint32_t l = c_strlen(s, N);
    /*
    for(uint32_t i = 0; i < l; i++)
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

char& Rope::operator [](uint16_t p)
{
    return str[p];
}

bool Rope::operator == (Rope r)
{
    return compare(r);
}

bool Rope::operator != (Rope r)
{
    return (!compare(r));
}

bool Rope::operator == (const char* r)
{
    return compare(r);
}

bool Rope::operator != (const char* r)
{
    return (!compare(r));
}

bool Rope::compare(Rope r, uint32_t len)
{
    if(len == 0)
    {
        if(r.length() != length())
            return false;
        len = length();
    }
    for(uint32_t i = 0; i < len; i++)
    {
        if(str[i] != r.str[i])
            return false;
    }
    return true;
}

bool Rope::compare(const char* c, uint32_t len)
{
    if(len == 0)
    {
        len = static_cast<int>(c_strlen(c, static_cast<uint16_t>(length()+2)));
        if(len != length())
        {
            return false; //strings are different lengths, so they aren't the same
        }
    }
    for(uint32_t i = 0; i < len; i++)
    {
        if(str[i] != c[i])
            return false;
    }
    return true;
}

bool Rope::compare(Rope r, uint32_t start_this, uint32_t start_that, uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        if(str[i+start_this] != r.str[start_that+i])
            return false;
    }
    return true;
}

bool Rope::compare(const char* c, uint32_t start_this, uint32_t start_that, uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        if(str[i+start_this] != c[start_that+i])
            return false;
    }
    return true;
}

void Rope::sub_string(char* buf, uint32_t start, uint32_t len)
{
    uint32_t i = 0;
    for(; i < len; i++)
        buf[i] = str[i+start];
    buf[i] = '\0';
}

void Rope::sub_string(Rope& r, uint32_t start, uint32_t len)
{
    sub_string(r.str, start, len);
}

const char* Rope::c_str()
{
    return (const char*)str;
}

void Rope::clear()
{
    for(int i = 0; i < N; i++) str[i] = '\0';
    pos = 0;
}

int Rope::atoi(uint16_t p)
{
    int res = 0,n=1;
    char* pstr = &str[p];
    if(*pstr =='-')
    {
        n=-1;
        pstr++;
    }
    while (*pstr >= '0' && *pstr <= '9')
        res = res * 10 + *pstr++ - '0';
    return res*n;
    //return ::atoi(&str[p]);
}

float Rope::atof(uint16_t ps)
{
    if(compare("nan", 3))
        return NAN;
    if(compare("inf", 3))
        return INFINITY;

    char* p = &str[ps];
    int frac;
    double sign, value;

    sign = 1.0;
    if (*p == '-')
    {
        sign = -1.0;
        p += 1;
    }
    else if (*p == '+')
        p += 1;

    for (value = 0.0; (*p >= '0') && (*p <= '9'); p++)
        value = value * 10.0 + (*p - '0');

    // Get digits after decimal point, if any.
    if (*p == '.')
    {
        double pow10 = 10.0;
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

void Rope::copy(char* c, uint32_t len) const
{
    if(len == 0)
        len = N;
    for(uint32_t i = 0; i < len; i++)
        c[i] = str[i];
}

char* Rope::get_buffer()
{
    return str;
}

uint16_t Rope::c_strlen(const char* c, uint16_t maxlen)
{
    for(uint16_t i = 0; i < maxlen; i++)
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

