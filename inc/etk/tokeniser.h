
#ifndef TOKENISER_H_INCLUDED
#define TOKENISER_H_INCLUDED


/*

	The tokeniser moves along a string and breaks it into tokens.

	These are all comma separated tokens

	  v    v  v    v    v
	$POW0,12,135,1238,8234*F5


	Example:

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

    Result:
        $POW0 12 135*F5

*/


namespace etk
{

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


