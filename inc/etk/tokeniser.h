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

#ifndef TOKENISER_H_INCLUDED
#define TOKENISER_H_INCLUDED

#include "staticstring.h"

namespace etk
{

/**
 \class Tokeniser



 \brief The tokeniser moves along a string and breaks it into tokens.

	http://www.camelsoftware.com/blog/2015/12/11/splitting-strings-a-c-string-tokeniser/

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
        etk::Tokeniser tok(nmea, ',');
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
    Tokeniser(T& _str, char tok) : str(_str)
    {
        token = tok;
    }

    bool next(auto& out, int len)
    {
        int counter = 0;
        if(str[count] == 0)
            return false;

        while(counter < len)
        {
            if(str[count] == '\0')
            {
                out[counter] = '\0';
                return true;
            }

            if(str[count] == token)
            {
                out[counter] = '\0';
                count++;
                return true;
            }

            out[counter] = str[count];

            count++;
            counter++;
        }
        return false;
    }

private:
    T& str;
    int token;
    int count = 0;
};



template <typename T> Tokeniser<T> make_tokeniser(T& l, char t)
{
    return Tokeniser<T>(l, t);
}

}

#endif


