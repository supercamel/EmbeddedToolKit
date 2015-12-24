#include "tokeniser_test.h"
#include <etk/etk.h>
#include <iostream>

using namespace etk;
using namespace std;


bool tokeniser_test(std::string& subtest)
{
    subtest = "Tokeniser test";

    {
        StaticString<100> ss = "A bunch of words";
        StaticString<20> token;
        auto tok = make_tokeniser(ss, ' ');
        int count = 0;
        while(tok.next(token, 20))
        {
            switch(count)
            {
            case 0:
            {
                if(token != "A")
                    return false;
            }
            break;
            case 1:
            {
                if(token != "bunch")
                    return false;
            }
            break;
            case 2:
            {
                if(token != "of")
                    return false;
            }
            break;
            case 3:
            {
                if(token != "words")
                    return false;
            }
            break;
            }
            count++;
        }
    }

    {
        subtest = "Tokenising a C string to StaticString";
        const char* s = "A bunch of words";
        StaticString<20> token;
        auto tok = make_tokeniser(s, ' ');
        int count = 0;
        while(tok.next(token, 20))
        {
            switch(count)
            {
            case 0:
            {
                if(token != "A")
                    return false;
            }
            break;
            case 1:
            {
                if(token != "bunch")
                    return false;
            }
            break;
            case 2:
            {
                if(token != "of")
                    return false;
            }
            break;
            case 3:
            {
                if(token != "words")
                    return false;
            }
            break;
            }
            count++;
        }
    }

    {
        subtest = "Tokenising a C string to StaticString";
        const char* s = "A bunch of words";
        char ctoken[20];
        auto tok = make_tokeniser(s, ' ');
        int count = 0;
        while(tok.next(ctoken, 20))
        {
            Rope token(ctoken, 20);
            switch(count)
            {
            case 0:
            {
                if(token != "A")
                    return false;
            }
            break;
            case 1:
            {
                if(token != "bunch")
                    return false;
            }
            break;
            case 2:
            {
                if(token != "of")
                    return false;
            }
            break;
            case 3:
            {
                if(token != "words")
                    return false;
            }
            break;
            }
            count++;
        }
    }
    return true;
}
