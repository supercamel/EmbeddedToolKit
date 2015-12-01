

#include <etk/tokeniser.h>

namespace etk
{

Tokeniser::Tokeniser(char* _str, char _token)
{
    str = _str;
    token = _token;
}


bool Tokeniser::next(char* out, int len)
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

}

