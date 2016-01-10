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


#include <etk/cout.h>


namespace etk
{


Cout::Cout()
{
     pos = 0;
     for(uint32 i = 0; i < 24; i++)
        st[i] = '\0';
}

Cout::~Cout()
{

}

Cout& Cout::operator << (Rope rope)
{
	uint32 l = rope.length();
	for(uint32 i = 0; i < l; i++)
		put(rope.c_str()[i]);
	return *this;
}

Cout& Cout::operator << (const char* s)
{
	uint32 l = Rope::c_strlen(s, 1024);
	for(uint32 i = 0; i < l; i++)
		put(s[i]);
	return *this;
}

Cout& Cout::operator << (int s)
{
	char b[20];
	Rope rope(b, 20);
	rope << (int32)s;
	for(uint32 i = 0; i < rope.length(); i++)
		put(b[i]);
	return *this;
}

Cout& Cout::operator << (long int s)
{
    char b[20];
	Rope rope(b, 20);
	rope << (int64)s;
	for(uint32 i = 0; i < rope.length(); i++)
		put(b[i]);
	return *this;
}

Cout& Cout::operator << (float s)
{
	char b[20];
	Rope rope(b, 20);
	rope << s;
	for(uint32 i = 0; i < rope.length(); i++)
		put(b[i]);
	return *this;
}

Cout& Cout::operator << (double s)
{
	char b[20];
	Rope rope(b, 20);
	rope << (float)s;
	for(uint32 i = 0; i < rope.length(); i++)
		put(b[i]);
	return *this;
}

Cout& Cout::operator << (char c)
{
	put(c);
	return *this;
}


Cout& Cout::operator << (Cout& s)
{
	for(int i = 0; i < s.pos; i++)
		st[i] = s.st[i];
	return *this;
}


void Cout::flush()
{
    write(st);

	for(uint32 i = 0; i < 24; i++)
		st[i] = '\0';
	pos = 0;
}

void Cout::put(char c)
{
	st[pos++] = c;
	if(pos == 2)
	{
		st[pos] = '\0';
		flush();
	}
}

}


