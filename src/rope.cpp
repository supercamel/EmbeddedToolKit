

#include <etk/rope.h>
#include <etk/math_util.h>
#include <cmath>


namespace etk
{


Rope::Rope(char* buf, uint16_t maxlen, const char* c)
{
    str = buf;
    pos = 0;
    N = maxlen;
    for(int i = 0; i < c_strlen(c, maxlen); i++)
        append(c[i]);
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
        len = N;
    for(int i = 0; i < len; i++)
    {
        append(s[i]);
        if(s[i] == '\0')
        {
            pos--;
            return;
        }
    }
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
	for(;i < 9; i++)
		append(buf[i]);

	terminate();
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
	for(;i < 20; i++)
		append(buf[i]);

	terminate();
}

void Rope::append(float j, uint8_t precision)
{
	if(std::isnan(j))
	{
		append("nan");
		return;
	}
	if(std::isinf(j))
	{
		append("inf");
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
    uint32_t i;

	for(i = 0; i < len; i++)
		append(sb.c_str()[i]);

    append('.');

    for(i = len; i < sb.length(); i++)
        append(sb.c_str()[i]);

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
    for(int i = 0; i < len; i++)
        append(sb.c_str()[i]);
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
    for(uint32_t i = 0; i < l; i++)
        append(s[i]);
	terminate();

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
            return false; //strings are different lengths, so they aren't the same
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

int Rope::atoi()
{
    int is[9];
    int i = 0;
    for(; i < 9; i++)
        is[i] = 0;
    i = 0;
    int result = 0;
    int negate = 1;
    if(str[0] == '-')
    {
        negate = -1;
        i++;
    }
    uint32_t count = 0;
	int len = length();
    for(; i < len; i++)
    {
        if((str[i] > '9') || (str[i] < '0'))
            break;
        is[count] = str[i]-'0';
        count++;
    }
    bool counting = false;
    int mul = 1;
    for(i = 8; i >= 0; i--)
    {
        int r = is[i];
        if(r != 0)
            counting = true;
        if(counting)
        {
            result += r*mul;
            mul *= 10;
        }
    }
    return negate*result;
}

float Rope::atof()
{
	if(compare("nan", 3))
		return NAN;
	if(compare("inf", 3))
		return INFINITY;

    float integer_part = 0.0f;
    float fraction_part = 0.0f;
    int is[9];
    int i = 0;
    for(; i < 9; i++)
        is[i] = 0;
    i = 0;
    int result = 0;
    int negate = 1;
    if(str[0] == '-')
    {
        negate = -1;
        i++;
    }
    int count = 0;
	int len = length();
    for(; i < len; i++)
    {
        if((str[i] > '9') || (str[i] < '0'))
            break;
        is[count] = str[i]-'0';
        count++;
    }
    bool counting = false;
    int mul = 1;
    for(int k = 8; k >= 0; k--)
    {
        int r = is[k];
        if(r != 0)
            counting = true;
        if(counting)
        {
            result += r*mul;
            mul *= 10;
        }
    }
    integer_part = result;

    if(str[i++] == '.')
    {
        float divisor = 10;
        for(; i < len; i++)
        {
            if((str[i] > '9') || (str[i] < '0'))
                break;
            fraction_part += (str[i]-'0')/divisor;
            divisor *= 10;
        }
    }
    return (integer_part + fraction_part) * negate;
}

void Rope::copy(char* c, uint32_t len) const
{
	if(len == 0)
		len = N;
	for(uint32_t i = 0; i < len; i++)
		c[i] = str[i];
}

char* Rope::raw_buffer()
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
    {
        append('\0');
        pos--;
    }
    else
    {
        str[N-1] = '\0';
    }
}


}

