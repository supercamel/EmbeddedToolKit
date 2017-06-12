#include <iostream>
#include <etk/etk.h>

using namespace std;


class Sensor
{
public:
	void check_something()
	{
		something_detected_signal.emit(10.5);
	}
	
	etk::Signal1<void, float> something_detected_signal;
};

class Controller
{
public:
	Controller() : 
		something_measured_slot(this, &Controller::on_something_measured)
	{
	
	}
	
	void on_something_measured(float m)
	{
		cout << "measured " << m << endl;
	}
	
	etk::Slot1<Controller, void, float> something_measured_slot;
};


	
int main()
{
	Sensor sensor;
	Controller controller;
	
	sensor.something_detected_signal.connect(controller.something_measured_slot);
	
	sensor.check_something();
}


