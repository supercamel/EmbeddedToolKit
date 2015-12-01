
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

template <uint32_t L> class StaticString
{
public:
    StaticString()
    {
		clear();
    }

    StaticString(const char* c)
    {
		clear();
		Rope r(list.raw_memory(), L);
		r.append(c);
    }

    StaticString(Rope r)
    {
		clear();
        r.copy(list.raw_memory());
    }

    StaticString& operator = (Rope r)
    {
        r.copy(list.raw_memory(), L);
        return *this;
    }

    StaticString& operator = (StaticString s)
    {
        for(uint32_t i = 0; i < L; i++)
        	list.raw_memory()[i] = s.c_str()[i];
        return *this;
    }

    StaticString& operator=(const char* c)
    {
		uint32_t l = Rope::c_strlen(c, L-1);
		uint32_t i = 0;
		for(i = 0; i < l; i++)
        	list.raw_memory()[i] = c[i];
		list.raw_memory()[i] = '\0';
        return *this;
    }

	StaticString& operator=(char* c)
	{
		for(uint32_t i = 0; i < Rope::c_strlen(c, L); i++)
        	list.raw_memory()[i] = c[i];
        return *this;
	}

    StaticString& operator + (Rope& s)
    {
		Rope r(list.raw_memory(), L);
		r.set_cursor(r.length());
        r = r + s;
        return *this;
    }

	template <uint32_t N> StaticString& operator += (StaticString<N> & s)
    {
        Rope r(list.raw_memory(), L);
		r.set_cursor(r.length());
        r += s.c_str();
        return *this;
    }

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

	StaticString& operator += (float f)
	{
		char temp[20];
		Rope r(temp, 20);
		r << f;
		*this += r.c_str();
		return *this;
	}

	StaticString& operator += (double f)
	{
		char temp[20];
		Rope r(temp, 20);
		r << f;
		*this += r.c_str();
		return *this;
	}

	StaticString& operator += (int32_t f)
	{
		char temp[20];
		Rope r(temp, 20);
		r << f;
		*this += r.c_str();
		return *this;
	}

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

    char& operator [](uint16_t p)
    {
        if(p >= L)
            return list.raw_memory()[L-1];
        return list.raw_memory()[p];
    }

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

	template <uint32_t N> bool compare(StaticString<N>& s, uint32_t max_len)
	{
		Rope rope(list.raw_memory(), L);
		return rope.compare(s.list.raw_memory(), min(max_len, L));
	}

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

	float atof()
	{
		Rope rope(list.raw_memory(), L);
		return rope.atof();
	}

	int atoi()
	{
		Rope rope(list.raw_memory(), L);
		return rope.atoi();
	}

	uint32_t length()
	{
		Rope rope(list.raw_memory(), L);
		return rope.length();
	}

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

    Rope& get_rope()
    {
        return *this;
    }

    void insert(char c, uint32_t pos)
    {
        list.set_list_end(Rope::c_strlen(list.raw_memory(), L));
        list.insert(c, pos);
    }

    void remove(uint32_t pos)
    {
        list.set_list_end(Rope::c_strlen(list.raw_memory(), L));
        list.remove(pos, '\0');
    }

    void erase(uint32_t pos, uint32_t len)
    {
        list.set_list_end(Rope::c_strlen(list.raw_memory(), L));
        list.erase(pos, len, '\0');
    }

    void to_upper()
    {
        for(uint32_t i = 0; i < Rope::c_strlen(list.raw_memory(), L); i++)
            list.raw_memory()[i] = etk::to_upper(list.raw_memory()[i]);
    }

    void to_lower()
    {
        for(uint32_t i = 0; i < Rope::c_strlen(list.raw_memory(), L); i++)
            list.raw_memory()[i] = etk::to_lower(list.raw_memory()[i]);
    }

	const char* c_str()
	{
		return list.buffer();
	}

	char* raw_memory()
	{
		return list.raw_memory();
	}

	void sub_string(char* buf, uint32_t start, uint32_t len)
	{
		Rope r(list.raw_memory(), L);
		r.sub_string(buf, start, len);
	}

    void sub_string(Rope& rope, uint32_t start, uint32_t len)
	{
		Rope r(list.raw_memory(), L);
		r.sub_string(rope, start, len);
	}

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


