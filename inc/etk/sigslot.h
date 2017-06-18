#ifndef SIGSLOT_H_INCLUDED
#define SIGSLOT_H_INCLUDED

#include "math_util.h"

namespace etk
{

/**
 \class SlotBase0
 \brief The base class for all Slot0 instantiations.
 */
template<typename R> class SlotBase0
{
public:
    virtual R callback() {
        return R();
    }
};

/**
 \class Slot0

 \brief A slot with zero parameters.
  If a Signal0 is connected to a slot, then the slot callback function is called whenever the signal is emitted.
@tparam BASE The base class type.
@tparam R The return type of the signal.

@code

class Controller
{
public:
	Controller() : 
		something_measured_slot(this, &Controller::on_something_measured)
	{ }
	
	void on_something_measured() {
		cout << "something measured " << endl;
	}
	
	etk::Slot0<Controller, void> something_measured_slot;
};

@endcode
 */
 
template<typename BASE, typename R> class Slot0 : public SlotBase0<R>
{
public:
	/**
	 * \brief Slot0 constructor.
	 * @arg base A pointer to the base class.
	 * @arg on_callback A pointer to the callback function.
	 */
    Slot0(BASE* base, R (BASE::*on_callback)()) : base(base), on_callback(on_callback)
    { }

	/**
	 * \brief Runs the callback function. If the base or callback are invalid, this will crash.
	 */
    R callback()
    {
        return (base->*on_callback)();
    }

private:
    BASE* base = nullptr;
    R (BASE::*on_callback)() = nullptr;
};


/**
 \class Signal0

 \brief A signal with zero parameters.
 Signals and slots are used in event-driven programming to signal when an event has occured. 
 Signals connect to slots.
 When the signal is emitted, it is receive by the slot. 
 This means the emitter and receiver can be completely de-coupled. The emitter doesn't need a pointer or reference to the receiver and vise-versa. 
@tparam R Return type of the signal.

@code
class TempSensor
{
public:
	void check_something() { something_detected_signal.emit(); }
	
	etk::Signal0<void> something_detected_signal;
};

class Controller
{
public:
	Controller() : 
		something_measured_slot(this, &Controller::on_something_measured)
	{ }
	
	void on_something_measured() {
		cout << "something measured " << endl;
	}
	
	etk::Slot0<Controller, void> something_measured_slot;
};

int main()
{
	Sensor sensor;
	Controller controller;
	
	sensor.something_detected_signal.connect(controller.something_measured_slot);
	sensor.check_something();
}

@endcode

 Note that TempSensor has absolutely no idea about Controller, and Controller doesn't know about TempSensor either. Using signals and slots means they are totally de-coupled.
 That's a good thing! 
 */

template<typename R> class Signal0
{
public:
	/**
	 * \brief constructor
	 */
    Signal0() { }
    
    /**
     * \brief Constructs and connects the signal to a slot.
     */
    Signal0(SlotBase0<R>& slot) : slot(&slot) { }

	/**
	 * \brief Connects a signal to a slot.
	 * @arg a a slot. 
	 */
    void connect(SlotBase0<R>& s) {
        slot = &s;
    }

	/**
	 * \brief Emits the signal. 
	 * @return The result of the slot callback function.
	 */
    R emit()
    {
        if(slot)
            return slot->callback();
        return R();
    }

private:
    SlotBase0<R>* slot = nullptr;
};



/**
 \class SlotBase1
 \brief The base class for all Slot1 instantiations.
 */
template<typename R, typename ARG1> class SlotBase1
{
public:
    virtual R callback(ARG1 a1) {
        unused(a1);
        return R();
    }
};


/**
 \class Slot1

 \brief A slot with one parameter.
  If a Signal1 is connected to a slot, then the slot callback function is called whenever the signal is emitted.
  
@tparam BASE The base class type.
@tparam R The return type of the signal.
@tparam ARG1 The type of the parameter.

@code

class Controller
{
public:
	Controller() : 
		something_measured_slot(this, &Controller::on_something_measured)
	{ }
	
	void on_something_measured(float m) {
		cout << "something measured " << m << endl;
	}
	
	etk::Slot1<Controller, void, float> something_measured_slot;
};

@endcode
 */
 
template<typename BASE, typename R, typename ARG1> class Slot1 : public SlotBase1<R, ARG1>
{
public:
    Slot1(BASE* base, R (BASE::*on_callback)(ARG1)) : base(base), on_callback(on_callback)
    { }

    R callback(ARG1 arg1)
    {
        return (base->*on_callback)(arg1);
    }

private:
    BASE* base = nullptr;
    R (BASE::*on_callback)(ARG1 arg1) = nullptr;
};


/**
 \class Signal1

 \brief A signal with one parameter. 
 The parameter is passed to the slot when the signal is emitted. 

@tparam R return type of the signal
@tparam ARG1 The parameter type of the signal. 
 */
template<typename R, typename ARG1> class Signal1
{
public:
	/**
	 * \brief constructor
	 */
    Signal1() { }
    
    /**
     * \brief Constructs and connects the signal.
     * @arg slot the slot to connect to.
     */
    Signal1(SlotBase1<R, ARG1>& slot) : slot(&slot) { }

	/**
     * \brief Connects the signal.
     * @arg s the slot to connect to.
     */
    void connect(SlotBase1<R, ARG1>& s) {
        slot = &s;
    }

	/**
	 * \brief Emits the signal.
	 * @arg arg1 This parameter is sent to the slot.
	 * @return Returns the result of the slot.
	 */
    R emit(ARG1 arg1)
    {
        if(slot)
            return slot->callback(arg1);
        return R();
    }

private:
    SlotBase1<R, ARG1>* slot = nullptr;
};



/**
 \class SlotBase2
 \brief The base class for all Slot2 instantiations.
 */
template<typename R, typename ARG1, typename ARG2> class SlotBase2
{
public:
    virtual R callback(ARG1 a1, ARG2 a2) {
        unused(a1);
        unused(a2);
        return R();
    }
};


/**
 \class Slot2

 \brief A slot with two parameters.
  If a Signal2 is connected to a slot, then the slot callback function is called whenever the signal is emitted.
  
@tparam BASE The base class type.
@tparam R The return type of the signal.
@tparam ARG1 The type of the parameter.
@tparam ARG2 The type of the second parameter.
 */
 
template<typename BASE, typename R, typename ARG1, typename ARG2> class Slot2 : public SlotBase2<R, ARG1, ARG2>
{
public:
    Slot2(BASE* base, R (BASE::*on_callback)(ARG1, ARG2)) : base(base), on_callback(on_callback)
    { }

    R callback(ARG1 arg1, ARG2 arg2)
    {
        return (base->*on_callback)(arg1, arg2);
    }

private:
    BASE* base = nullptr;
    R (BASE::*on_callback)(ARG1 arg1, ARG2 arg2) = nullptr;
};


/**
 \class Signal2

 \brief A signal with two parameters. 
 The parameters are passed to the slot when the signal is emitted. 

@tparam R return type of the signal
@tparam ARG1 Type of first parameter. 
@tparam ARG2 The second parameter type.
 */
template<typename R, typename ARG1, typename ARG2> class Signal2
{
public:
    Signal2() { }
    
    /**
     * \brief Constructs and connects the signal to a Slot2
     * @arg slot the slot to connect to.
     */
    Signal2(SlotBase2<R, ARG1, ARG2>& slot) : slot(&slot) { }

	/**
     * \brief Connects the signal.
     * @arg s the slot to connect to.
     */
    void connect(SlotBase2<R, ARG1, ARG2>& s) {
        slot = &s;
    }

	/**
	 * \brief Emits the signal.
	 * @arg the first parameter to send to the slot.
	 * @arg the second parameter to send to the slot.
	 * @return the return value of the slot.
	 */
    R emit(ARG1 arg1, ARG2 arg2)
    {
        if(slot)
            return slot->callback(arg1, arg2);
        return R();
    }

private:
    SlotBase2<R, ARG1, ARG2>* slot = nullptr;
};

}

#endif

