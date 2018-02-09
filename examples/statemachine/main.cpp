#include <iostream>
#include <etk/etk.h>
#include "traffic.h"

using namespace std;

constexpr TrafficController::transition_table TrafficController::table[];

int main()
{
	TrafficController traffic;
	
	cout << "Starting in east/west green state" << endl;
	while(true)
	{
		//if the template parameter is true, it will run the event checks
		//if false, events will only be triggered using traffic.submit_event()
		traffic.iterate<true>();
		sleep_ms(100);
	}
}

