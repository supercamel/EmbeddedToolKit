#ifndef ETK_POOL_POINTER
#define ETK_POOL_POINTER


/**
 * This is a smart pointer implementation for memory pools.
 *
 * It's a reference counting smart pointer.
 *
 * Example:
 Pool<1024> pool;
// pool_pointer<Pool<1024>, Object> p_obj = make_pool_ptr<1024, Object>(pool);
//it's much cleaner to use auto where possible
auto p_obj = make_pool_ptr<1024, Object>(pool);
p_obj->foo();
*/


#ifndef __AVR__

#include <new>
#include <cstdlib>
#include <utility>

#include "pool.h"

namespace etk
{


    template <typename T> class pool_pointer
    {

        public:
            /**
             * Use the make function to construct a pool pointer. E.g.
             *
             * auto p = etk::pool_pointer<MyClass>::make(pool, <constructor params>);
             *
             */
            template<class... U> static pool_pointer<T> make(Pool& pool, U&&... u);


            /**
             * copy constructor
             */
            pool_pointer(const pool_pointer<T>& sp) 
            {
                pool.ref(o);
            }

            ~pool_pointer()
            {
                pool.free(o);
            }

            T& operator* ()
            {
                return o;
            }

            T* operator-> ()
            {
                return &o;
            }

            /**
             * assignment operator
             */
            pool_pointer<T>& operator = (const pool_pointer<T>& sp)
            {
                // if not assigning to itself
                if (this != &sp)
                {
                    // deref self
                    pool.unref(o);

                    //get other obj
                    o = sp.o;
                    //inc reference
                    pool.ref(o);
                }
                return *this;
            }

            /**
             * comparison operators
             */
            bool operator == (const pool_pointer<T>& sp)
            {
                return o == sp.o;
            }

            bool operator != (const pool_pointer<T>& sp)
            {
                return o != sp.o;
            }

            // boolean cast operator
            operator bool()
            {
                return o != nullptr;
            }

            // gets a reference to the pool
            Pool& get_pool()
            {
                return pool;
            }


        private:
            pool_pointer() { }

            pool_pointer(Pool& pool, T* o) 
                : pool(pool), o(o)
            { 
            }

            T* o = nullptr;
            Pool& pool;
    };

    template <typename T> template<class... U> pool_pointer<T> pool_pointer<T>::make(Pool& pool, U&&... u) {
        auto* ptr = (T*)pool.alloc(sizeof(T)); 

        new(&ptr->o) T(std::forward<U>(u)...);

        pool_pointer<T> sp(pool, ptr);
        return sp;
    }
}

#endif

#endif
