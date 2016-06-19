/*
    Embedded Tool Kit
    Copyright (C) 2016 Samuel Cowen

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
#include <etk/rope.h>
#include <etk/vector.h>
#include <type_traits>


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


template <uint32 L> class StaticString
{
public:
    StaticString()
    {
        clear();
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
        clear();
        Rope r(buf, L);
        r.append(c);
    }

	/**
     * \brief Initialises the string from a Rope.
     */
    StaticString(Rope r)
    {
        clear();
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
        uint32 l = Rope::c_strlen(c, L-1);
        uint32 i = 0;
        for(; i < l; i++)
            buf[i] = c[i];
        buf[i] = '\0';
        return *this;
    }

	/**
	 * \brief Copies a C-string to this.
	 */
    StaticString& operator=(char* c)
    {
        uint32 i = 0;
        for(; i < Rope::c_strlen(c, L); i++)
            buf[i] = c[i];
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
    template <uint32 N> StaticString& operator += (Vector<N>& v)
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
    StaticString& operator += (float f)
    {
        char temp[20];
        Rope r(temp, 20);
        r << f;
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
        r << f;
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
        uint32 len = etk::min<uint32>(L, Rope::c_strlen(c, L));

        if(len != Rope::c_strlen(buf, L))
            return false;

        for(uint32 i = 0; i < len; i++)
        {
            if(c[i] != buf[i])
                return false;
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
        uint32 l = min<uint32>(Rope::c_strlen(s.c_str(), N), length());
        Rope rope(buf, L);
        return rope.compare(s.buf, l);
    }

    bool compare(const char* s)
    {
        Rope rope(buf, L);
        return rope.compare(s, min<uint32>(Rope::c_strlen(s, L), L));
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

	/**
	 * \brief Returns the number of characters in the string.
	 */
    uint32 length()
    {
        Rope rope(buf, L);
        return rope.length();
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
            for(uint32 i = length(); i != pos; i--)
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
        for(uint32 i = 0; i < Rope::c_strlen(buf, L); i++)
            buf[i] = etk::to_upper(buf[i]);
    }

	/**
	 * \brief Converts the string to lower case.
	 */
    void to_lower()
    {
        for(uint32 i = 0; i < Rope::c_strlen(buf, L); i++)
            buf[i] = etk::to_lower(buf[i]);
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
    template<typename... Args> void scan(Args&... args)
    {
        _scan(buf, args...);
    }
    

private:
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
    
    char buf[L];
};


}


#endif


