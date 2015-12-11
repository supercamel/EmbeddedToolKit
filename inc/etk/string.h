#ifndef ETK_STRING_H_INCLUDED
#define ETK_STRING_H_INCLUDED

#include "pool.h"
#include "rope.h"

namespace etk
{

class String
{
public:
    String(MemPool& pool) : pool(pool)
    {
        buflen = 32;
        pstr = pool.malloc<char>(32);
    }

    String(MemPool& pool, const char* c) : pool(pool)
    {
        uint32_t len = 0;
        while(c[len] != 0)
            len++;
        pstr = pool.malloc<char>(len);
        for(uint32_t i = 0; i < len; i++)
            pstr[i] = c[i];
        buflen = len;
    }

    ~String()
    {
        pool.free(pstr);
    }

    String& operator=(String& s)
    {
        pool.free(pstr);
        pool = s.pool;
        pstr = s.pstr;
        buflen = s.buflen;
        return *this;
    }

    String& operator=(const char* c)
    {
        uint32_t len = 0;
        while(c[len] != 0)
            len++;
        if(len > buflen)
            pstr = pool.realloc<char>(pstr, len);
        for(uint32_t i = 0; i < len; i++)
            pstr[i] = c[i];
        buflen = len;

        return *this;
    }

    String& operator=(char* c)
    {
        uint32_t len = 0;
        while(c[len] != 0)
            len++;
        if(len > buflen)
            pstr = pool.realloc<char>(pstr, len);
        for(uint32_t i = 0; i < len; i++)
            pstr[i] = c[i];
        buflen = len;

        return *this;
    }

    String& operator += (String& s)
    {
        uint32_t len = length();
        uint32_t slen = s.length();
        if((len+slen) > buflen)
            pstr = pool.realloc<char>(pstr, len+slen);

        for(uint32_t i = 0; i < slen; i++)
            pstr[len+i] = s.pstr[i];
        return *this;
    }

    String& operator += (const char* s)
    {
        String s2(pool, s);
        *this += s2;
        return *this;
    }

    String& operator += (char* s)
    {
        *this += (const char*)s;
        return *this;
    }

    String& operator + (String& s)
    {
        uint32_t len = length();
        uint32_t slen = s.length();
        if((len+slen) > buflen)
            pstr = pool.realloc<char>(pstr, len+slen);

        for(uint32_t i = 0; i < slen; i++)
            pstr[len+i] = s.pstr[i];
        return *this;
    }

    String& operator + (const char* s)
    {
        String s2(pool, s);
        *this += s2;
        return *this;
    }

    String& operator + (char* s)
    {
        *this += (const char*)s;
        return *this;
    }

    uint32_t size()
    {
        return buflen;
    }

    uint32_t length()
    {
        for(uint32_t i = 0; i < buflen; i++)
        {
            if(pstr[i] == 0)
                return i;
        }
        return buflen;
    }

    char& operator [](uint16_t p)
    {
        return pstr[p];
    }

    const char* c_str()
    {
        return pstr;
    }

private:
    MemPool& pool;
    char* pstr;
    uint32_t buflen;
};

}


#endif

