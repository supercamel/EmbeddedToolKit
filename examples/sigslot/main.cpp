#include <iostream>
#include <etk/etk.h>

using namespace std;

/**
 Signals and slots are very useful in event-driven programming because they 
 provide an easy way of de-coupling event sources from event handlers.
 
 Here's an example
 
 You have a temperature controller beer brewer. You have a temperature sensor 
 and a heater element that needs to turn off at the right temperature. 
 */


class TempSensor
{
public:
	void check_temperature()
	{
		something_detected_signal.emit(10.5);
	}
	
	etk::Signal1<void, float> hot_enough_signal;
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


