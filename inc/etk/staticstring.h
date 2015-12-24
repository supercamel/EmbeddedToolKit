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

#ifndef ETK_STATIC_STRING_H_INCLUDED
#define ETK_STATIC_STRING_H_INCLUDED


/*
 * The static string class uses non-dynamic memory.
 *
 * It's essentially a rope, but it doesn't need to be provided with a
 * pre-existing buffer.
 *
*/

#include <stdint.h>
#include <etk/list.h>
#include <etk/rope.h>
#include <etk/vector.h>


namespace etk
{

/**
 * \class StaticString
 *
 * \brief StaticStrings are a safer alternative to C-strings as StaticStrings are immune to buffer overruns.
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


template <uint32_t L> class StaticString
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
        Rope r(list.raw_memory(), L);
        r.append(c);
    }

	/**
     * \brief Initialises the string from a Rope.
     */
    StaticString(Rope r)
    {
        clear();
        r.copy(list.raw_memory());
    }

	/**
	 * \brief Assigns the contents of a Rope to the string.
	 */
    StaticString& operator = (Rope r)
    {
        r.copy(list.raw_memory(), L);
        return *this;
    }

	/**
	 * \brief Copies another string to this.
	 */
    StaticString& operator = (StaticString s)
    {
        for(uint32_t i = 0; i < L; i++)
            list.raw_memory()[i] = s.c_str()[i];
        return *this;
    }

	/**
	 * \brief Copies a const C-string to this.
	 */
    StaticString& operator=(const char* c)
    {
        uint32_t l = Rope::c_strlen(c, L-1);
        uint32_t i = 0;
        for(i = 0; i < l; i++)
            list.raw_memory()[i] = c[i];
        list.raw_memory()[i] = '\0';
        return *this;
    }

	/**
	 * \brief Copies a C-string to this.
	 */
    StaticString& operator=(char* c)
    {
        uint32_t i = 0;
        for(i = 0; i < Rope::c_strlen(c, L); i++)
            list.raw_memory()[i] = c[i];
        list.raw_memory()[i] = '\0';
        return *this;
    }

	/**
	 * \brief Appends the contents of a Rope to this.
	 */
    StaticString& operator + (Rope& s)
    {
        Rope r(list.raw_memory(), L);
        r.set_cursor(r.length());
        r = r + s;
        return *this;
    }

	/**
	 * \brief Appends the contents of another string to this.
	 */
    template <uint32_t N> StaticString& operator += (StaticString<N> & s)
    {
        Rope r(list.raw_memory(), L);
        r.set_cursor(r.length());
        r += s.c_str();
        return *this;
    }

	/**
	 * \brief Appends and nicely formats an etk::Vector to this.
	 */
    template <uint32_t N> StaticString& operator += (Vector<N>& v)
    {
        Rope r(list.raw_memory(), L);
        r.set_cursor(r.length());
        for(uint32_t i = 0; i < N-1; i++)
        {
            r << v[i] << ", ";
        }
        r << v[N-1];
        return *this;
    }

    StaticString& operator += (Rope& s)
    {
        Rope r(list.raw_memory(), L);
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
	 * \brief Appends an int32_t to this.
	 */
    StaticString& operator += (int32_t f)
    {
        char temp[20];
        Rope r(temp, 20);
        r << f;
        *this += r.c_str();
        return *this;
    }

	/**
	 * \brief Appends a uint32_t to this.
	 */
    StaticString& operator += (uint32_t f)
    {
        char temp[20];
        Rope r(temp, 20);
        r << f;
        *this += r.c_str();
        return *this;
    }

    StaticString& operator + (char* s)
    {
        Rope r(list.raw_memory(), L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    StaticString& operator += (char* s)
    {
        Rope r(list.raw_memory(), L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    StaticString& operator + (const char* s)
    {
        Rope r(list.raw_memory(), L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

    StaticString& operator += (const char* s)
    {
        Rope r(list.raw_memory(), L);
        r.set_cursor(r.length());
        r += s;
        return *this;
    }

	/**
	 * \brief This operator overload allows you to access and modify individual characters in the string.
	 */
    char& operator [](uint16_t p)
    {
        if(p >= L)
            return list.raw_memory()[L-1];
        return list.raw_memory()[p];
    }

	/**
	 * \brief Compares this to a const C-string
	 */
    bool operator == (const char* c)
    {
        uint32_t len = etk::min<uint32_t>(L, Rope::c_strlen(c, L));
        for(uint32_t i = 0; i < len; i++)
        {
            if(c[i] != list[i])
                return false;
        }
        return true;
    }

	/**
	 * \brief Compares this to another StaticString up to max_len characters.
	 */
    template <uint32_t N> bool compare(StaticString<N>& s, uint32_t max_len)
    {
        Rope rope(list.raw_memory(), L);
        return rope.compare(s.list.raw_memory(), min(max_len, L));
    }

	/**
	 * \brief Compares this to const C-string up to max_len characters.
	 */
    bool compare(const char* s, uint32_t max_len)
    {
        Rope rope(list.raw_memory(), L);
        return rope.compare(s, min(max_len, L));
    }

    template <uint32_t N> bool compare(StaticString<N>& s)
    {
        uint32_t l = min<uint32_t>(Rope::c_strlen(s.c_str(), N), length());
        Rope rope(list.raw_memory(), L);
        return rope.compare(s.list.raw_memory(), l);
    }

    bool compare(const char* s)
    {
        Rope rope(list.raw_memory(), L);
        return rope.compare(s, min<uint32_t>(Rope::c_strlen(s, L), L));
    }

	/**
	 * \brief Converts the string to a floating point number.
	 * @code
	 etk::StaticString<20> ss("20.65");
	 float f = ss.atof();
	 //f == 20.65f
	 @endcode
	 */
    float atof(uint16_t p=0)
    {
        Rope rope(list.raw_memory(), L);
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
    int atoi(uint16_t p=0)
    {
        Rope rope(list.raw_memory(), L);
        return rope.atoi(p);
    }

	/**
	 * \brief Returns the number of characters in the string.
	 */
    uint32_t length()
    {
        Rope rope(list.raw_memory(), L);
        return rope.length();
    }

	/**
	 * \brief Sets all characters to null.
	 */
    void clear()
    {
        Rope rope(list.raw_memory(), L);
        rope.clear();
    }

    bool operator == (Rope r)
    {
        Rope r1(list.raw_memory(), L);
        return (r1 == r);
    }

    bool operator != (const char* c)
    {
        return !(operator==(c));
    }

    bool operator != (Rope r)
    {
        Rope r1(list.raw_memory(), L);
        return (r1 != r);
    }

    Rope get_rope()
    {
    	Rope rope(list.raw_memory(), L);
        return rope;
    }

	/**
	 * \brief Inserts a character in a position.
	 */
    void insert(char c, uint32_t pos)
    {
        list.set_list_end(Rope::c_strlen(list.raw_memory(), L));
        list.insert(c, pos);
    }

	/**
	 * \brief Removes a character rom position pos
	 */
    void remove(uint32_t pos)
    {
        list.set_list_end(Rope::c_strlen(list.raw_memory(), L));
        list.remove(pos, '\0');
    }

	/**
	 * \brief Erases a number of characters starting at position pos.
	 */
    void erase(uint32_t pos, uint32_t len)
    {
        list.set_list_end(Rope::c_strlen(list.raw_memory(), L));
        list.erase(pos, len, '\0');
    }

    void fill(char c, uint32_t pos, uint32_t len)
    {
        list.set_list_end(Rope::c_strlen(list.raw_memory(), L));
        list.fill(pos, pos+len, c);
    }


	/**
	 * \brief Converts the string to upper case.
	 */
    void to_upper()
    {
        for(uint32_t i = 0; i < Rope::c_strlen(list.raw_memory(), L); i++)
            list.raw_memory()[i] = etk::to_upper(list.raw_memory()[i]);
    }

	/**
	 * \brief Converts the string to lower case.
	 */
    void to_lower()
    {
        for(uint32_t i = 0; i < Rope::c_strlen(list.raw_memory(), L); i++)
            list.raw_memory()[i] = etk::to_lower(list.raw_memory()[i]);
    }

	/**
	 * \brief Returns a pointer to the C-string buffer.
	 */
    const char* c_str()
    {
        return list.buffer();
    }

	/**
	 * \bried Returns a pointer to the raw memory used by StaticString
	 */
    char* raw_memory()
    {
        return list.raw_memory();
    }

	/**
	 * \bried Extracts a section of text from the string and assigns it to buf.
	 */
    void sub_string(char* buf, uint32_t start, uint32_t len)
    {
        Rope r(list.raw_memory(), L);
        r.sub_string(buf, start, len);
    }

	/**
	 * \bried Extracts a section of text from the string and assigns it to rope.
	 */
    void sub_string(Rope& rope, uint32_t start, uint32_t len)
    {
        Rope r(list.raw_memory(), L);
        r.sub_string(rope, start, len);
    }

	/**
	 * \bried Extracts a section of text from the string and assigns it to string.
	 */
    template <uint32_t N> void sub_string(StaticString<N>& string, uint32_t start, uint32_t len)
    {
        Rope r(list.raw_memory(), L);
        r.sub_string(string.raw_memory(), start, len);
    }

private:
    List<char, L> list;
};


}


#endif


