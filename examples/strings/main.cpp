#include <etk/etk.h>
#include <iostream>
using namespace std;


int main()
{
	etk::StaticString<128> s;
	s = "Hello world!";
	cout << s.c_str() << endl;

	//string to int and float
	s = "53.6";
	cout << "\nStrings can be used for int and float conversions" << endl;
	cout << "\t" << s.c_str() << " = " << s.atof() << endl;
	s = "58";
	cout << "\t" << s.c_str() << " = " << s.atoi() << endl;


	//int and float to string
	s.clear();
	s += 58;
	cout << "\t" << s.c_str() << endl;

	s.clear();
	s += -623.63;
	cout << "\t" << s.c_str() << endl;

	//appending test
	s.clear();
	s = "Appending ";
	s += "text!";
	cout << s.c_str() << endl;

	//strings don't use dynamic memory. it's quite possible for a string to 
	//exceed it's maximum size, so this must be handled safely.
	s.clear();
	s = "strings don't use dynamic memory. it's quite possible for a string to exceed it's maximum size"
			", so this must be handled safely. text that exceeds maximum length is simply ignored!";

	s.to_upper();
	cout << endl << s.c_str() << endl;
}


