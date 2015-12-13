
#ifndef TOKENISER_H_INCLUDED
#define TOKENISER_H_INCLUDED


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

class Tokeniser
{
public:
    Tokeniser(char* str, char token);

    bool next(char* out, int len);

private:
    char* str;
    int token;
};

}

#endif


