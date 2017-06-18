#ifndef ETK_POOL_POINTER
#define ETK_POOL_POINTER


/**
 * This is a smart pointer implementation for memory pools.
 *
 * Memory pool leaks are typically less dangerous than memory leaked from the heap
 * since memory pools are often placed on the stack anyway. Infact, sometimes it's
 * desirable to NOT explicitly free memory from a pool because it can be faster to
 * simply destroy the whole pool rather than each object individually.
 *
 * However, leaks can and do occur with memory pools.
 *
 * Memory pools simply allocate and free memory. They do not construct or destroy C++
 * objects. This must be done explicitly using placement new/delete.
 *
 * The pool_pointer class is a smart pointer that allocates memory and constructs the object.
 * It's a reference counting smart pointer, so it knows when the object is no longer required
 * and it automatically destructs the object and releases the memory back to the pool.
 * It's very convenient - but only use it if you need those features.
 *
 * Example:
        Pool<1024> pool;
        // pool_pointer<Pool<1024>, Object> p_obj = make_pool_ptr<1024, Object>(pool);
        //it's much cleaner to use auto where possible
        auto p_obj = make_pool_ptr<1024, Object>(pool);
        p_obj->foo();
 */

#ifndef __AVR__

#include <cstdlib>
#include <utility>

#include "pool.h"

namespace etk
{

namespace experimental
{

template <typename T> class pool_pointer
{
public:
    pool_pointer(Pool& pool) : pool(pool)
    {
        ref = static_cast<pool_pointer<T>::RefCounter*>(pool.alloc(sizeof(RefCounter)));
        ref->construct();
        if(ref)
            ref->reference();
    }

    pool_pointer(Pool& pool, T* pValue) : ptr(pValue), pool(pool)
    {
        ref = static_cast<pool_pointer<T>::RefCounter*>(pool.alloc(sizeof(RefCounter)));
        ref->construct();
        if(ref)
            ref->reference();
    }

    pool_pointer(const pool_pointer<T>& sp) : ptr(sp.ptr), ref(sp.ref), pool(sp.pool)
    {
        ref->reference();
    }

    ~pool_pointer()
    {
        if(ref->unreference() == 0)
        {
            ptr->~T(); //placement delete
            pool.free(ptr);
            pool.free(ref);
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

    pool_pointer<T>& operator = (const pool_pointer<T>& sp)
    {
        if (this != &sp)
        {
            if(ref->unreference() == 0)
            {
                ptr->~T(); //placement delete
                pool.free(ptr);
                pool.free(ref);
            }

            ptr = sp.ptr;
            ref = sp.ref;
            ref->reference();
        }
        return *this;
    }

    bool operator == (const pool_pointer<T>& sp)
    {
        return ptr == sp.ptr;
    }

    bool operator != (const pool_pointer<T>& sp)
    {
        return ptr != sp.ptr;
    }

    operator bool()
    {
        if((ptr) && (ref))
            return true;
        return false;
    }

    Pool& get_pool()
    {
        return pool;
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
        int32 count;
    };

    T* ptr = nullptr;
    RefCounter* ref = nullptr;
    Pool& pool;
};


template<typename T, class... U> pool_pointer<T> make_pool_ptr(Pool& pool, U&&... u)
{
    void* ptr = pool.alloc(sizeof(T));
    if(ptr)
        new(ptr)T(std::forward<U>(u)...);

    pool_pointer<T> sp(pool, static_cast<T*>(ptr));

    return sp;
}

}

}

#endif

#endif
