#ifndef SIGSLOT_H_INCLUDED
#define SIGSLOT_H_INCLUDED

#include "math_util.h"

namespace etk
{

template<typename R> class SlotBase0
{
public:
    virtual R callback() {
        return R();
    }
};


template<typename BASE, typename R> class Slot0 : public SlotBase0<R>
{
public:
    Slot0(BASE* base, R (BASE::*on_callback)()) : base(base), on_callback(on_callback)
    { }

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
 When an event occurs, the signal is emitted. If a slot is connected, it will receive the signal. 
 What this means is that the emitter and receiver can be completely de-coupled. The emitter doesn't need a pointer or reference to the receiver and vise-versa. 
 
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
		cout << "something measured " << m << endl;
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
    Signal0() { }
    Signal0(SlotBase0<R>& slot) : slot(&slot) { }

    void connect(SlotBase0<R>& s) {
        slot = &s;
    }

    R emit()
    {
        if(slot)
            return slot->callback();
        return R();
    }

private:
    SlotBase0<R>* slot = nullptr;
};




template<typename R, typename ARG1> class SlotBase1
{
public:
    virtual R callback(ARG1 a1) {
        unused(a1);
        return R();
    }
};


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


template<typename R, typename ARG1> class Signal1
{
public:
    Signal1() { }
    Signal1(SlotBase1<R, ARG1>& slot) : slot(&slot) { }

    void connect(SlotBase1<R, ARG1>& s) {
        slot = &s;
    }

    R emit(ARG1 arg1)
    {
        if(slot)
            return slot->callback(arg1);
        return R();
    }

private:
    SlotBase1<R, ARG1>* slot = nullptr;
};


template<typename R, typename ARG1, typename ARG2> class SlotBase2
{
public:
    virtual R callback(ARG1 a1, ARG2 a2) {
        unused(a1);
        unused(a2);
        return R();
    }
};


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


template<typename R, typename ARG1, typename ARG2> class Signal2
{
public:
    Signal2() { }
    Signal2(SlotBase2<R, ARG1, ARG2>& slot) : slot(&slot) { }

    void connect(SlotBase2<R, ARG1, ARG2>& s) {
        slot = &s;
    }

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

