#include <etk/etk.h>
#include <iostream>

using namespace std;
using namespace etk;
using CMD_PFUNC = int (*) (void);

typedef struct ct{
	StaticString<5> name;
	CMD_PFUNC f;     		//pointer to function for this item
}ctable;

int do_help(){
	cout << "This and that and this and that" << endl;
	return 0;
}

int do_runc(){
	printf("Running some code...\r\n");
	return 0;
}

int do_exit(){
	printf("EXIT\r\n");
	return 0;
}

int main(){
// Initialize list
	List<ctable, 3> list;

	ctable helpCmd;
	helpCmd.name = "help";
	helpCmd.f = do_help;

	ctable runCmd;
	runCmd.name = "runc";
	runCmd.f = do_runc;

	ctable exitCmd;
	exitCmd.name = "exit";
	exitCmd.f = do_exit;

	list.append(helpCmd);
	list.append(runCmd),
	list.append(exitCmd);
// <<-

	cout<< "Available commands are: " <<endl;
	for(auto i : list){
		cout << i.name.c_str() << endl;
	}
	cout << "> ";

	string userInput;
	int cnt=0;
	while(1){
		cnt=0;
		getline(cin, userInput);
		for(auto& i : list){
			if(i.name.compare(userInput.c_str())){
				i.f();
				break;
			}
			else if(cnt == list.size()-1){
				cout << "No such command!" << endl;
			}
			cnt++;
		}
		cout << "> ";
	}
}
/* [] END OF FILE */
