#ifndef ETK_SMART_POINTER
#define ETK_SMART_POINTER

/**

    Why do we need another smart pointer??

    1. Many micro-controllers don't have the STL infrastructure to deal with std::shared_ptr and std::unique_ptr
    2. Even if they did, shared_ptr and unique_ptr are known to blow out code size massively due to dependancies.
    3. The standard smart pointers throw exceptions and a lot of people don't like that on a microcontroller.
    4. This is small, fast, easy and relatively safe provided you check the pointer after EVERY creation.

    Also, beware heap fragmentation!

    Useage:
        auto ptr = make_smart_ptr<Obj>();


        if the object takes constructor parameters

        auto ptr = make_smart_ptr<Obj>(param1, param2);


*/

#ifndef __AVR__

#include <stdlib.h>
#include <utility>

namespace etk
{


template <typename T> class smart_pointer
{
public:
    smart_pointer()
    {
        ref = static_cast<smart_pointer<T>::RefCounter*>(malloc(sizeof(RefCounter)));
        ref->construct();
        if(ref)
            ref->reference();
    }

    smart_pointer(T* pValue) : ptr(pValue)
    {
        ref = static_cast<smart_pointer<T>::RefCounter*>(malloc(sizeof(RefCounter)));
        ref->construct();
        if(ref)
            ref->reference();
    }

    smart_pointer(const smart_pointer<T>& sp) : ptr(sp.ptr), ref(sp.ref)
    {
        ref->reference();
    }

    ~smart_pointer()
    {
        if(ref->unreference() == 0)
        {
            ptr->~T(); //placement delete
            free(ptr);
            free(ref);
        }
    }

    T& operator* ()
    {
        return *ptr;
    }

    T* operator-> ()
    {
        return ptr;
    }

    smart_pointer<T>& operator = (const smart_pointer<T>& sp)
    {
        if (this != &sp)
        {
            if(ref->unreference() == 0)
            {
                ptr->~T(); //placement delete
                free(ptr);
                free(ref);
            }

            ptr = sp.ptr;
            ref = sp.ref;
            ref->reference();
        }
        return *this;
    }

    bool operator == (const smart_pointer<T>& sp)
    {
        return ptr == sp.ptr;
    }

    bool operator != (const smart_pointer<T>& sp)
    {
        return ptr != sp.ptr;
    }

    operator bool()
    {
        if((ptr) && (ref))
            return true;
        return false;
    }

private:

    class RefCounter
    {
    public:
        void construct() {
            count = 0;
        }

        void reference()
        {
            count++;
        }

        int unreference()
        {
            return --count;
        }

    private:
        int count;
    };

    T* ptr = nullptr;
    RefCounter* ref = nullptr;
};


template<typename T, class... U> smart_pointer<T> make_smart_ptr(U&&... u)
{
    void* ptr = malloc(sizeof(T));
    if(ptr)
        new(ptr)T(std::forward<U>(u)...);

    smart_pointer<T> sp(static_cast<T*>(ptr));
    return sp;
}

}

#endif

#endif
