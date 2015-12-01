

#ifndef COUT_H_INCLUDED
#define COUT_H_INCLUDED

#include "rope.h"
#include "string.h"
#include "vector.h"
#include "matrix.h"

namespace etk
{

class Cout
{
public:
    Cout();
    virtual ~Cout();
    Cout& operator << (Rope rope);
    Cout& operator << (const char* s);
    Cout& operator << (int i);
    Cout& operator << (long int i);
    Cout& operator << (float i);
    Cout& operator << (double i);
    Cout& operator << (char i);
    Cout& operator << (Cout& s);

    template<uint32_t n> Cout& operator << (Vector<n> v)
    {
        for(uint32_t i = 0; i < n-1; i++)
            operator<<(v[i]), operator<<(", ");

        operator<<(v[n-1]);
        put('\n');
        return *this;
    }

    template<uint32_t max_x, uint32_t max_y> Cout& operator << (Matrix<max_x, max_y> v)
    {
        for(uint32_t x = 0; x < max_x; x++)
        {
            for(uint32_t y = 0; y < max_y-1; y++)
                operator<<(v(x, y)), operator<<("\t");
            operator<<(v(x, max_y-1));
            put('\n');
        }
        return *this;
    }

    void put(char c);
    void flush();

    virtual void write(const char* buf) = 0;

private:
    char st[24];
    int pos;
};


}


#endif

