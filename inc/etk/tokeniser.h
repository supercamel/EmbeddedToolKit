
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
        int count = 0;

        if(str[0] == 0)
            return false;

        while(count < len)
        {
            if(str[count] == '\0')
            {
                out[count] = '\0';
                str = &str[count];
                return true;
            }

            if(str[count] == token)
            {
                out[count] = '\0';
                count++;
                str = &str[count];
                return true;
            }

            out[count] = str[count];

            count++;
        }
        return false;
    }

private:
    T& str;
    int token;
};

template <typename T> Tokeniser<T> make_tokeniser(T& l, char t)
{
    return Tokeniser<T>(l, t);
}


}

#endif


