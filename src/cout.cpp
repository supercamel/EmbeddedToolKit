

#include <etk/cout.h>


namespace etk
{


Cout::Cout()
{
     pos = 0;
     for(uint32_t i = 0; i < 24; i++)
        st[i] = '\0';
}

Cout::~Cout()
{

}

Cout& Cout::operator << (Rope rope)
{
	uint32_t l = rope.length();
	for(uint32_t i = 0; i < l; i++)
		put(rope.c_str()[i]);
	return *this;
}

Cout& Cout::operator << (const char* s)
{
	uint32_t l = Rope::c_strlen(s, 1024);
	for(uint32_t i = 0; i < l; i++)
		put(s[i]);
	return *this;
}

Cout& Cout::operator << (int s)
{
	char b[20];
	Rope rope(b, 20);
	rope << (int32_t)s;
	for(uint32_t i = 0; i < rope.length(); i++)
		put(b[i]);
	return *this;
}

Cout& Cout::operator << (long int s)
{
    char b[20];
	Rope rope(b, 20);
	rope << (int64_t)s;
	for(uint32_t i = 0; i < rope.length(); i++)
		put(b[i]);
	return *this;
}

Cout& Cout::operator << (float s)
{
	char b[20];
	Rope rope(b, 20);
	rope << s;
	for(uint32_t i = 0; i < rope.length(); i++)
		put(b[i]);
	return *this;
}

Cout& Cout::operator << (double s)
{
	char b[20];
	Rope rope(b, 20);
	rope << (float)s;
	for(uint32_t i = 0; i < rope.length(); i++)
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

	for(uint32_t i = 0; i < 24; i++)
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


