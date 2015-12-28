#include <etk/etk.h>
#include <iostream>
#include <string>

using namespace std;
using namespace etk;


enum foo_codes
{
	RED,
	GREEN,
	BLUE,
	ON,
	OFF
};
	
int main()
{
	StaticString<80> foo_str = "$LED,170,50,0,100,5000";
	
	int codes[5];

	if(foo_str.compare("$LED,",5))
	{
		auto tok = make_tokeniser(foo_str, ',');
		int token_n = 0;
		StaticString<20> token;
		
		tok.next(token, 20); //the first token is '$LED', so call next() to get that out of the way
		while(tok.next(token, 20) && (token_n < 5))
			codes[token_n++] = token.atoi();
	}
	
	for(int i = 0; i < 5; i++)
		cout << codes[i] << " ";
	cout << endl;
}

