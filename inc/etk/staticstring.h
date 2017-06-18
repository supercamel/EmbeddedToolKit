/*
    Embedded Tool Kit
    Copyright (C) 2016 Samuel Cowen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/

#ifndef ETK_STATIC_STRING_H_INCLUDED
#define ETK_STATIC_STRING_H_INCLUDED


/*
 * The static string class uses non-dynamic memory.
 *
 * It's essentially a rope, but it doesn't need to be provided with a
 * pre-existing buf.
 *
*/

#include "types.h"
#include "rope.h"
#include "vector.h"

#ifndef __AVR__
#include <type_traits>
#endif

namespace etk
{

/**
 * \class StaticString
 *
 * \brief StaticStrings are a safer alternative to C-strings as StaticStrings are immune to buf overruns.
 * StaticStrings also provide a lot of the convenience of std::strings, although they do differ from std::strings in a number of ways.
 *
 * @code
 #include <etk/etk.h>
#include <iostream>

using namespace std;


int main()
{
	etk::StaticString<100> ss;
	ss = "Hello world!"; //basic string assignment

	ss.clear(); //clear string and start again
	//assign a bunch of stuff.
	ss += 56;
	ss += " + ";
	ss += 5.4;
	ss += " = ";
	ss += 56+5.4;

	//that's a big clunky. here's another option.
	ss.clear();
	ss.get_rope() << 56 << " + " << 5.4  << " = " << 56+5.4;
	cout << ss.c_str() << endl;

	etk::StaticString<20> sub;
	ss.sub_string(sub, 5, 4);
	//sub now contains "5.40"
	float f = sub.atof();
	cout << f << endl;
}
@endcode
 * @tparam L the maximum length of the string in bytes.
 */


template <uint32 L, uint8 default_float_precision=2> class StaticString
{
public:
    StaticString()
    {
        buf[0] = '\0';
    }

    /**
     * \brief Initialises the string with some text.
     * @code
     StaticString<20> ss("Hello world!");
     //ss now contains 'Hello world!'
     @endcode
     */
    StaticString(const char* c)
    {
        Rope r(buf, L, c);
    }

    /**
     * \brief Initialises the string from a Rope.
     */
    StaticString(Rope r)
    {
        r.copy(buf);
    }

    /**
     * \brief Assigns the contents of a Rope to the string.
     */
    StaticString& operator = (Rope r)
    {
        r.copy(buf, L);
        return *this;
    }

    /**
     * \brief Copies another string to this.
     */
    StaticString& operator = (StaticString& s)
    {
        for(uint32 i = 0; i < L; i++)
            buf[i] = s.c_str()[i];
        return *this;
    }

    template <uint32 nn> StaticString& operator = (StaticString<nn>& s)
    {
        for(uint32 i = 0; i < min(L,nn); i++)
            buf[i] = s.c_str()[i];
        return *this;
    }

    /**
     * \brief Copies a const C-string to this.
     */
    StaticString& operator=(const char* c)
    {
        uint32 i = 0;
        for(; i < L-1; i++)
        {
            buf[i] = c[i];
            if(buf[i] == '\0')
                return *this;
        }
        buf[i] = '\0';
        return *this;
    }

    /**
     * \brief Copies a C-string to this.
     */
    StaticString& operator=(char* c)
    {
        uint32 i = 0;
        for(; i < L-1; i++)
        {
            buf[i] = c[i];
            if(buf[i] == '\0')
                return *this;
        }
        buf[i] = '\0';
        return *this;
    }

    template<uint32 nn> StaticString& operator=(StaticString<nn> ss)
    {
        *this = ss.c_str();
        return *this;
    }

    template<typename T> StaticString& operator=(T i)
    {
        buf[0] = '\0';
        Rope r(buf, L);
        r << i;
        return *this;
    }

    /**
     * \brief Appends the contents of a Rope to this.
     */
    StaticString& operator + (Rope& s)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r = r + s;
        return *this;
    }

    /**
     * \brief Appends the contents of another string to this.
     */
    template <uint32 N> StaticString& operator += (StaticString<N> & s)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r += s.c_str();
        return *this;
    }

    /**
     * \brief Appends and nicely formats an etk::Vector to this.
     */
    template <uint32 N> StaticString& operator += (Vector<N> v)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        for(uint32 i = 0; i < N-1; i++)
        {
            r << v[i] << ", ";
        }
        r << v[N-1];
        return *this;
    }

    StaticString& operator += (Rope& s)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    /**
     * \brief Appends a float to this.
     */
    void append(float f, uint8 precision=default_float_precision)
    {
        char temp[20];
        Rope r(temp, 20);
        r.append(f, precision);
        *this += temp;
    }

    /**
     * \brief Appends a double to this.
     */
    void append(double f, uint8 precision=default_float_precision)
    {
        char temp[20];
        Rope r(temp, 20);
        r.append(f, precision);
        *this += temp;
    }
    /**
     * \brief Appends a float to this.
     */
    StaticString& operator += (float f)
    {
        char temp[20];
        Rope r(temp, 20);
        r.append(f, default_float_precision);
        *this += r.c_str();
        return *this;
    }

    /**
     * \brief Appends a double to this.
     */
    StaticString& operator += (double f)
    {
        char temp[20];
        Rope r(temp, 20);
        r.append(f, default_float_precision);
        *this += r.c_str();
        return *this;
    }

    /**
     * \brief Appends an int32 to this.
     */
    StaticString& operator += (int32 f)
    {
        char temp[20];
        Rope r(temp, 20);
        r << f;
        *this += r.c_str();
        return *this;
    }

    /**
     * \brief Appends a uint32 to this.
     */
    StaticString& operator += (uint32 f)
    {
        char temp[20];
        Rope r(temp, 20);
        r << f;
        *this += r.c_str();
        return *this;
    }

    StaticString& operator + (char* s)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    StaticString& operator += (char c)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r << c;
        return *this;
    }

    StaticString& operator += (char* s)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    StaticString& operator + (const char* s)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    StaticString& operator += (const char* s)
    {
        Rope r(buf, L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    /**
     * \brief This operator overload allows you to access and modify individual characters in the string.
     */
    char& operator [](uint32 p)
    {
        if(p >= L)
            return buf[L-1];
        return buf[p];
    }

    char operator [](uint32 p) const
    {
        if(p >= L)
            return buf[L-1];
        return buf[p];
    }

    char& at(uint32 p)
    {
        return (*this)[p];
    }

    char get_char(uint32 p) const
    {
        if(p >= L)
            return buf[L-1];
        return buf[p];
    }

    /**
     * \brief A simple get function for accessing const string characters.
     */
    char get(uint32 p) const
    {
        if(p >= L)
            return buf[L-1];
        return buf[p];
    }

    /**
     * \brief Compares this to a const C-string
     */
    bool operator == (const char* c)
    {
        for(uint32 i = 0; i < L-1; i++)
        {
            if(c[i] != buf[i])
                return false;
            if(c[i] == '\0')
                return true;
        }
        return true;
    }

    /**
     * \brief Compares this to another StaticString up to max_len characters.
     */
    template <uint32 N> bool compare(StaticString<N>& s, uint32 max_len)
    {
        Rope rope(buf, L);
        return rope.compare(s.buf, min(max_len, L));
    }

    /**
     * \brief Compares this to const C-string up to max_len characters.
     */
    bool compare(const char* s, uint32 max_len)
    {
        Rope rope(buf, L);
        return rope.compare(s, min(max_len, L));
    }

    template <uint32 N> bool compare(StaticString<N>& s)
    {
        Rope rope(buf, L);
        return rope.compare(s.buf, L);
    }

    bool compare(const char* s)
    {
        Rope rope(buf, L);
        return rope.compare(s, L);
    }

    /**
     * \brief Converts the string to a floating point number.
     * @code
     etk::StaticString<20> ss("20.65");
     float f = ss.atof();
     //f == 20.65f
     @endcode
     */
    float atof(uint16 p=0)
    {
        Rope rope(buf, L);
        return rope.atof(p);
    }

    /**
     * \brief Converts the string to an integer.
     * @code
     etk::StaticString<20> ss("20.65");
     int f = ss.atof();
     //f == 20
     @endcode
     */
    int atoi(uint16 p=0)
    {
        Rope rope(buf, L);
        return rope.atoi(p);
    }

    uint32 parse_hex(uint16 p=0)
    {
        Rope rope(buf, L);
        return rope.parse_hex(p);
    }

    /**
     * \brief Returns the number of characters in the string.
     */
    uint32 length() const
    {
        uint32 i = 0;
        for(; i < L; i++)
        {
            if(buf[i] == '\0')
                break;
        }
        return i;
    }

    /**
     * \brief Sets all characters to null.
     */
    void clear()
    {
        Rope rope(buf, L);
        rope.clear();
    }

    bool operator == (Rope r)
    {
        Rope r1(buf, L);
        return (r1 == r);
    }

    bool operator != (const char* c)
    {
        return !(operator==(c));
    }

    bool operator != (Rope r)
    {
        Rope r1(buf, L);
        return (r1 != r);
    }

    Rope get_rope()
    {
        Rope rope(buf, L);
        return rope;
    }

    /**
     * \brief Inserts a character in a position.
     */
    void insert(char c, uint32 pos)
    {
        if(pos < L-1)
        {
            for(uint32 i = length()+1; i != pos; i--)
                buf[i] = buf[i-1];
            buf[pos] = c;
        }
    }

    /**
     * \brief Removes a character rom position pos
     */
    void remove(uint32 pos)
    {
        if(pos < L)
        {
            for(uint32 i = pos; i < L-1; i++)
                buf[i] = buf[i+1];
        }
    }

    /**
     * \brief Erases a number of characters starting at position pos.
     */
    void erase(uint32 pos, uint32 len)
    {
        if((pos+len) < L)
        {
            for(uint32 i = pos; i < L-1; i++)
                buf[i] = buf[min(L-1, i+len)];
        }
    }

    void fill(char c, uint32 pos, uint32 len)
    {
        if((pos+len) < L)
        {
            for(uint32 i = pos; i < (pos+len); i++)
                buf[i] = c;
        }
    }


    /**
     * \brief Converts the string to upper case.
     */
    void to_upper()
    {
        for(uint32 i = 0; i < L-1; i++)
        {
            if(buf[i] == '\0')
                break;
            buf[i] = etk::to_upper(buf[i]);
        }
    }

    /**
     * \brief Converts the string to lower case.
     */
    void to_lower()
    {
        for(uint32 i = 0; i < L-1; i++)
        {
            if(buf[i] == '\0')
                break;
            buf[i] = etk::to_lower(buf[i]);
        }
    }

    template<uint32 nn> operator StaticString<nn>() const
    {
        StaticString<nn> ss;
        ss = this->c_str();
        return ss;
    }

    /**
     * \brief Returns a pointer to the C-string buf.
     */
    const char* c_str() const
    {
        return buf;
    }

    /**
     * \brief Returns a pointer to the raw memory used by StaticString
     */
    char* raw_memory()
    {
        return buf;
    }

    /**
     * \brief Extracts a section of text from the string and assigns it to buf.
     */
    void sub_string(char* buf, uint32 start, uint32 len) const
    {
        Rope r(buf, L);
        r.sub_string(buf, start, len);
    }

    /**
     * \brief Extracts a section of text from the string and assigns it to rope.
     */
    void sub_string(Rope& rope, uint32 start, uint32 len) const
    {
        Rope r(buf, L);
        r.sub_string(rope, start, len);
    }

    /**
     * \brief Extracts a section of text from the string and assigns it to string.
     */
    template <uint32 N> void sub_string(StaticString<N>& string, uint32 start, uint32 len) const
    {
        uint32 i = 0;
        for(; i < len; i++)
            string[i] = (*this).get(i+start);
        string[i] = '\0';
        /*
                Rope r(buf, L);
                r.sub_string(string.raw_memory(), start, len);
        */
    }

    /**
     * \brief Reverses the contents of the string. E.g. "Hello" would become "olleH".
     */
    void reverse()
    {
        uint32 hlen = length()/2;
        uint32 len = length()-1;
        for(uint32 i = 0; i < hlen; i++)
            swap(buf[i], buf[len-i]);
    }

    /**
     * \brief Extracts a list numbers from a string of text. It's a little bit like scanf, except it doesn't check the format of the string.
     * For safety critical applications, you should use the tokeniser. The scan function doesn't perform any error checking and will fail silently.
     @code
    StaticString<100> st = "MSG05, 34, -9.5";
    int msgid, inum;
    float fnum;
    st.scan(msgid, inum, fnum);
    cout << msgid << " " << inum << " " << fnum << endl;
    //output would be 05 34 -9.5
    @endcode
     */
     
#ifndef __AVR__
    template<typename... Args> void scan(Args&... args)
    {
        _scan(buf, args...);
    }
#endif

private:

#ifndef __AVR__
    template<typename T> void _scan(const char* bbuf, T& t)
    {
        uint32 count = 0;
        while(count < L)
        {
            char c = bbuf[count++];
            if(c == '\0')
                break;
            else if(is_numeric(c) || (c == '-'))
            {
                char cbuf[20];
                Rope rope(cbuf, 20, &bbuf[count-1]);
                if(std::is_integral<T>::value)
                    t = rope.atoi();
                if(std::is_floating_point<T>::value)
                    t = rope.atof();
                while(is_numeric(c) || (c == '-') || (c == '.'))
                {
                    c = bbuf[count++];
                    if(count > L)
                        break;
                }
                break;
            }
        }
    }

    template<typename T, typename... Args> void _scan(const char* bbuf, T& t, Args&... args)
    {
        uint32 count = 0;
        while(count < L)
        {
            char c = bbuf[count++];
            if(c == '\0')
                break;
            else if(is_numeric(c) || (c == '-'))
            {
                char cbuf[20];
                Rope rope(cbuf, 20, &bbuf[count-1]);
                if(std::is_integral<T>::value)
                    t = rope.atoi();
                if(std::is_floating_point<T>::value)
                    t = rope.atof();
                while(is_numeric(c) || (c == '-') || (c == '.'))
                {
                    c = bbuf[count++];
                    if(count > L)
                        break;
                }
                break;
            }
        }
        _scan(&bbuf[count], args...);
    }
#endif

    char buf[L];
};


}


#endif
