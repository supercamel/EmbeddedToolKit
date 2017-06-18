#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

namespace etk
{


/**
 *
 *
 *
 */

template <class T, typename _STATE, typename _EVENT>
class StateMachine
{
public:
    struct transition_table
    {
        _STATE last_state;
        _STATE state;
        _EVENT event;
        _STATE next_state;
    };

    typedef bool (T::*iteration_foo)(void);
    typedef bool (T::*event_check_foo)(void);
    typedef void (T::*state_entry_foo)(void);
    typedef void (T::*state_exit_foo)(void);

    StateMachine(T* t, _STATE s) : t(t)
    {
        state = s;
        for(uint32_t i = 0; i < static_cast<uint32_t>(_STATE::END_STATE); i++)
        {
            state_callbacks[i] = nullptr;
            entry_callbacks[i] = nullptr;
            exit_callbacks[i] = nullptr;
        }
        for(auto& f : event_checks)
            f = nullptr;
    }

    template <bool CHECK_EVENTS> bool iterate()
    {
        if(CHECK_EVENTS)
        {
            for(const auto& row : T::table)
            {
                if((row.last_state == last_state) || (row.last_state == _STATE::END_STATE))
                {
                    if(row.state == state)
                    {
                        int n = static_cast<int>(row.event);
                        auto f = event_checks[n];
                        if(f != nullptr)
                        {
                            if((t->*f)())
                            {
                                on_state_changed(last_state, state, row.next_state);
                                uint32_t n = static_cast<uint32_t>(state);
                                if(exit_callbacks[n] != nullptr)
                                    (t->*exit_callbacks[n])();
                                last_state = state;
                                state = row.next_state;

                                n = static_cast<uint32_t>(state);
                                if(entry_callbacks[n] != nullptr)
                                    (t->*entry_callbacks[n])();
                            }
                        }
                    }
                }
            }
        }

        int n = static_cast<int>(state);
        if(state_callbacks[n] != nullptr)
        {
            auto f = state_callbacks[n];
            return (t->*f)();
        }

        return false;
    }

    bool submit_event(_EVENT e)
    {
        for(const auto& row : T::table)
        {
            if((row.state == state) && (row.event == e))
            {
                on_state_changed(last_state, state, row.next_state);

                uint32_t n = static_cast<uint32_t>(state);
                if(exit_callbacks[n] != nullptr)
                    (t->*exit_callbacks[n])();
                last_state = state;
                state = row.next_state;

                n = static_cast<uint32_t>(state);
                if(entry_callbacks[n] != nullptr)
                    (t->*entry_callbacks[n])();
                return true;
            }
        }
        return false;
    }

    void add_state_func(_STATE s, iteration_foo iter_foo)
    {
        uint32_t n = static_cast<uint32_t>(s);
        state_callbacks[n] = iter_foo;
    }

    void add_event_check(_EVENT e, event_check_foo event_foo)
    {
        uint32_t n = static_cast<uint32_t>(e);
        event_checks[n] = event_foo;
    }

    void add_entry_callback(_STATE s, state_entry_foo f)
    {
        uint32_t n = static_cast<uint32_t>(s);
        entry_callbacks[n] = f;
    }

    void add_exit_callback(_STATE s, state_exit_foo f)
    {
        uint32_t n = static_cast<uint32_t>(s);
        exit_callbacks[n] = f;
    }

    _STATE get_state()
    {
        return state;
    }

private:
    void on_state_changed(_STATE last, _STATE from, _STATE to)
    {
        static_cast<T*>(this)->on_state_changed(last, from, to);
    }

    _STATE last_state;
    _STATE state;


    iteration_foo state_callbacks[static_cast<uint32_t>(_STATE::END_STATE)];
    event_check_foo event_checks[static_cast<uint32_t>(_EVENT::END_EVENT)];
    state_entry_foo entry_callbacks[static_cast<uint32_t>(_STATE::END_STATE)];
    state_exit_foo exit_callbacks[static_cast<uint32_t>(_STATE::END_STATE)];

    T* t;
};

}

#endif

