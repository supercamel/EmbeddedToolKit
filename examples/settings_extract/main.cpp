#include <etk/etk.h>
#include <iostream>
#include <string>

using namespace std;
using namespace etk;

int main()
{
	/*
	 * I've using an std::string to store the settings in. 
	 * I did this to prove that make_tokeniser will work with many different string containers.
	 * This includes char*, const char*, etk::StaticString, etk::List, std::string . . .
	 */
	 
	string settings = "gain 45\n"
						"max_travel 85\n"
						"max_temp 75";
	int gain, max_travel, max_temp;
	
	//make a string tokeniser 'tok' using the settings string. tokens will be separated by '\n' character.
	//'tok' will split the settings into lines
	auto tok = make_tokeniser(settings, '\n');
	
	
	StaticString<40> line;
	while(tok.next(line, 40)) //while there are tokens (lines)
	{
		auto line_tok = make_tokeniser(line, ' '); //make another tokeniser, this time splitting by spaces
		StaticString<20> token;
		while(line_tok.next(token, 20)) //while there are tokens
		{
			if(token == "gain") //if the token is 'gain'
			{
				line_tok.next(token, 20); //the next token must be the gain setting
				gain = token.atoi(); //convert to integer
				continue;
			}
			if(token == "max_travel")
			{
				line_tok.next(token, 20);
				max_travel = token.atoi();
				continue;
			}
			if(token == "max_temp")
			{
				line_tok.next(token, 20);
				max_temp = token.atoi();
				continue;
			}
		}
	}
	
	cout << "gain " << gain << endl;
	cout << "max_travel " << max_travel << endl;
	cout << "max_temp " << max_temp << endl;
}

