#include <etk/etk.h>
#include <iostream>

using namespace std;
using namespace etk;
using CMD_PFUNC = int (*) (void);

struct ctable{
	ctable(StaticString<5> name, CMD_PFUNC f) : name(name), f(f) { }
	StaticString<5> name;
	CMD_PFUNC f;
};

List<ctable, 3> list;

int do_help(){
	cout<< "Available commands are: " <<endl;
	for(auto i : list){
		cout <<i.name.c_str() << endl;
	}
	return 0;
}

int do_runc(){
	printf("Running some code...\r\n");
	return 0;
}

int do_exit(){
	printf("Exiting...\r\n");
	exit(0);
	return 0;
}

void loadCommandList(){
// Initialize list with some dummy functions
	list.append({"help", do_help});
	list.append({"runc", do_runc});
	list.append({"exit", do_exit});
}

int main(){

	string userInput;

	loadCommandList();

	cout << "Command Line interface example\r\n";
	cout << "Type help for a list of commands\r\n";
	cout << "================================\r\n";
	cout << "> ";

	while(1){
		int cnt=0;
		getline(cin, userInput);
		for(auto& i : list){
			if(i.name.compare(userInput.c_str())){
				i.f();
				break;
			}
			else if(cnt == list.size()-1){
				cout << "No such command! Type help for a list of commands." << endl;
			}
			cnt++;
		}
		cout << "> ";
	}
}
/* [] END OF FILE */
