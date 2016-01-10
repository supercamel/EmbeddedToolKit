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

    template<uint32 n> Cout& operator << (Vector<n> v)
    {
        for(uint32 i = 0; i < n-1; i++)
            operator<<(v[i]), operator<<(", ");

        operator<<(v[n-1]);
        put('\n');
        return *this;
    }

    template<uint32 max_x, uint32 max_y> Cout& operator << (Matrix<max_x, max_y> v)
    {
        for(uint32 x = 0; x < max_x; x++)
        {
            for(uint32 y = 0; y < max_y-1; y++)
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

