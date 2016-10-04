/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/

#ifndef TOKENISER_H_INCLUDED
#define TOKENISER_H_INCLUDED

namespace etk
{

/**
 \class Tokeniser



 \brief The tokeniser moves along a string and breaks it into tokens.

	http://www.camelsoftware.com/2015/12/24/embedded-string-programming/

	These are all comma separated tokens
<pre>
	  v    v  v    v    v
	$POW0,12,135,1238,8234*F5
</pre>

	Example:
@code
    include <iostream>
    using namespace std;

    int main()
    {

        char nmea[] = "$POW0,12,135*F5";

        char token[20]; //buffer to store token
        auto tok = etk::make_tokeniser(nmea, ',');
        while(tok.next(token, 20))
            cout << token << " ";
        cout << endl;
    }
@endcode
    Result:
        $POW0 12 135*F5

*/

template <typename T> class Tokeniser
{
public:
    Tokeniser(const T& _str, const char tok) : str(_str)
    {
        token = tok;
    }

    template <typename Tt> bool next(Tt& out, const uint32 len)
    {
        uint32 out_counter = 0;
        if(str[count] == 0)
            return false;

        while(out_counter < len)
        {
            if(str[count] == '\0')
            {
                out[out_counter] = '\0';
                return true;
            }

            if(str[count] == token)
            {
                out[out_counter] = '\0';
                count++;
                return true;
            }

            out[out_counter] = str[count];

            count++;
            out_counter++;
        }
        return false;
    }

private:
    const T& str;
    char token;
    uint32 count = 0;
};



template <typename T> Tokeniser<T> make_tokeniser(const T& l, const char t)
{
    return Tokeniser<T>(l, t);
}

}

#endif
