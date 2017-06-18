#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <etk/etk.h>
#include <iostream>

/**
 * State machines are everywhere. They are implemented in microcontrollers and
 * in software all the time. State machines are abstract machines that can be in one of
 * many different states, and can transition to a different state in response
 * to an event. A simple state machine is very easy to implement but once there are
 * many different states with complex rules for transitioning, things get messy
 * very quickly. 
 *
 * ETK provides a StateMachine template that strives to make moderately complex
 * state machines more maintainable and easier to implement. 
 * There are so many different potential usecases for FSMs that it's quite 
 * challenging to implement a FSM template class that is general purpose, 
 * efficient and flexible. 
 * 
 * I think this implementation is OK but it needs a fair bit of boilerplate code.
 *
 * This example is of a FSM controlling traffic lights at an intersection. 
 * There are two roads going east/west and north/south. 
 *
 * StateMachine uses a transition table to determine which events can change the
 * state and what the new state will be. 
 * The format for each row is
 * { last state, current state, event, next state }
 *
 * { STATE::EW_YELLOW, STATE::BOTH_RED, EVENT::BOTH_RED_TIMEOUT, STATE::NS_GREEN },
 *
 * This line means if the last state was east/west light is yellow and the current 
 * state is both red, and there is a timeout event, then the new state must be 
 * north/south light green. 
 * This example is quite trivial. Imagine if turning lanes, fault conditions and
 * pedestrian crossings needed to be implemented. 
 * 
 * Any suggestions on how to make this StateMachine template better would be 
 * greatly appreciated!!
 *
 */

/**
 * This enum class defines the states.
 */
enum class STATE
{
	EW_GREEN, //east/west light is green. north/south is red
	EW_YELLOW, //east/west is yellow. ns is red.
	BOTH_RED, //both ways are red for a short moment
	NS_GREEN, //ns is green, ew is red
	NS_YELLOW, //ns is yellow, ew is red
	END_STATE //END_STATE is mandatory. (boilerplate code)
};

enum class EVENT
{
	EW_GREEN_TIMER, //this event occurs when the ew green light has been on for too long
	EW_GREEN_TRAFFIC, //occurs when there is no traffic going through
	EW_YELLOW_TIMER, //timer based event, so yellow light turns to red
	BOTH_RED_TIMEOUT, //both red timeout event
	NS_GREEN_TIMER, 
	NS_GREEN_TRAFFIC,
	NS_YELLOW_TIMER,
	END_EVENT //END_EVENT is mandatory boilerplate code
};


//a typedef, for readability
typedef etk::StateMachine<class TrafficController, STATE, EVENT> StateMachineType;

		
class TrafficController : public StateMachineType
{
public:
	TrafficController() : StateMachineType(this, STATE::EW_GREEN)
	{
		//the event check functions are called every iteration.
		//only the relevant event checks for the current state are called.
		//'external' events can be triggered by calling the 'submit_event()' function
		add_event_check(EVENT::EW_GREEN_TIMER, &TrafficController::green_timer_check);
		add_event_check(EVENT::EW_YELLOW_TIMER, &TrafficController::yellow_timer_check);
		add_event_check(EVENT::BOTH_RED_TIMEOUT, &TrafficController::both_red_check);
		add_event_check(EVENT::NS_GREEN_TIMER, &TrafficController::green_timer_check);
		add_event_check(EVENT::NS_YELLOW_TIMER, &TrafficController::yellow_timer_check);
		
		//this callbacks are called when a new state is entered.
		//there are also exit callbacks for when a state is exited (use add_exit_callback() instead). 
		add_entry_callback(STATE::EW_GREEN, &TrafficController::on_ew_green_entry);
		add_entry_callback(STATE::EW_YELLOW, &TrafficController::on_ew_yellow_entry);
		add_entry_callback(STATE::BOTH_RED, &TrafficController::on_both_red_entry);
		add_entry_callback(STATE::NS_GREEN, &TrafficController::on_ns_green_entry);
		add_entry_callback(STATE::NS_YELLOW, &TrafficController::on_ns_yellow_entry);
		
		//set start time for timing
		start = etk::now();
	}
	
	void reset_time()
	{
		start = etk::now();
	}
	

private:
	friend StateMachineType; //boilerplate
	
	void on_ew_green_entry()
	{ std::cout << "Entered East/West green light state" << std::endl; }
	
	void on_ew_yellow_entry()
	{ std::cout << "Entered East/West yellow light state" << std::endl; }
	
	void on_both_red_entry()
	{ std::cout << "Entered both red state" << std::endl; }
	
	void on_ns_green_entry()
	{ std::cout << "Entered North/South green state" << std::endl; }
	
	void on_ns_yellow_entry()
	{ std::cout << "Entered North/South yellow state" << std::endl; }
	
	bool green_timer_check()
	{
		return(etk::now().diff_time(start) > 5.0);
	}
	
	bool yellow_timer_check()
	{
		return(etk::now().diff_time(start) > 3.0);
	}
	
	bool both_red_check()
	{
		return(etk::now().diff_time(start) > 2.0);
	}
	
	//this is a boilerplate function. it must be implemented.
	//it's used here to reset the start time after a state change. 
	void on_state_changed(STATE last, STATE from, STATE to)
	{
		etk::unused(last);
		etk::unused(from);
		etk::unused(to);
		start = etk::now();
	}
	
	//this is also boilerplate code. 
	//it's the transition table. it must be called 'table' but of course you 
	//should populate the table with your own states and events.
	static constexpr transition_table table[] = {
		//ignore last state, if state is east/west green and the green timer goes off, turn lights yellow
		{ STATE::END_STATE, STATE::EW_GREEN, EVENT::EW_GREEN_TIMER, STATE::EW_YELLOW },
		//ignore last sate, if state is e/w green and there is no traffic, turn lights yellow
		{ STATE::END_STATE, STATE::EW_GREEN, EVENT::EW_GREEN_TRAFFIC, STATE::EW_YELLOW },
		//ignore last state, if east/west lights are yellow, and the timer goes off, turn all lights red
		{ STATE::END_STATE, STATE::EW_YELLOW, EVENT::EW_YELLOW_TIMER, STATE::BOTH_RED },
		//if last state was east/west yellow and state is both red and there is a timeout event, turn north south lights green
		{ STATE::EW_YELLOW, STATE::BOTH_RED, EVENT::BOTH_RED_TIMEOUT, STATE::NS_GREEN },
		//if last state was north/south yellow and state is both red and timeout occurs, turn east/west lights green
		{ STATE::NS_YELLOW, STATE::BOTH_RED, EVENT::BOTH_RED_TIMEOUT, STATE::EW_GREEN },
		// and so on . . .
		{ STATE::END_STATE, STATE::NS_GREEN, EVENT::NS_GREEN_TIMER, STATE::NS_YELLOW },
		{ STATE::END_STATE, STATE::NS_GREEN, EVENT::NS_GREEN_TRAFFIC, STATE::NS_YELLOW },
		{ STATE::END_STATE, STATE::NS_YELLOW, EVENT::NS_YELLOW_TIMER, STATE::BOTH_RED }
	};
	
	etk::Time start;
};


#endif

