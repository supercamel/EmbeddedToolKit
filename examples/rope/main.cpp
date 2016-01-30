#include <etk/etk.h>
#include <iostream>
using namespace std;


int main()
{
	char buffer[60]; //create a char array
	etk::Rope rope(buffer, 60); //create an etk::Rope using buffer
	
	rope << "Writing text to buffer\n"; //rope will put this text into the buffer
	cout << buffer; //print buffer to prove it works
	
	rope.clear(); //empty buffer
	rope << 100 << "\n"; //write an integer plus newline to buffer
	cout << buffer; //print the evidence
	
	rope.clear();
	rope << 100.01 << "\n"; //floats and doubles also work
	cout << buffer << endl;
}


