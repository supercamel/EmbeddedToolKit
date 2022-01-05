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
                : refobj(sp.refobj), pool(sp.pool)
            {
                refobj->count++;
            }

            ~pool_pointer()
            {
                refobj->count--;
                if(refobj->count == 0)
                {
                    refobj->obj.~T(); //placement delete
                    pool.free((void*)refobj);
                }
            }

            T& operator* ()
            {
                return refobj->obj;
            }

            T* operator-> ()
            {
                return &refobj->obj;
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
                    refobj->count -= 1;
                    if(refobj->count == 0)
                    {
                        refobj->obj.~T(); //destroy and free, if necessary
                        pool.free((void*)refobj);
                    }

                    //get other obj
                    refobj = sp.refobj;
                    //inc reference
                    refobj->count++;
                }
                return *this;
            }

            /**
             * comparison operators
             */
            bool operator == (const pool_pointer<T>& sp)
            {
                return refobj == sp.refobj;
            }

            bool operator != (const pool_pointer<T>& sp)
            {
                return refobj != sp.refobj;
            }

            // boolean cast operator
            operator bool()
            {
                return refobj != nullptr;
            }

            // gets a reference to the pool
            Pool& get_pool()
            {
                return pool;
            }


        private:
            pool_pointer() { }

            struct RefObj {
                uint32 count = 0;
                T obj;
            };

            pool_pointer(Pool& pool, RefObj* refobj) 
                : pool(pool), refobj(refobj)
            { 
            }

            RefObj* refobj = nullptr;
            Pool& pool;
    };

    template <typename T> template<class... U> pool_pointer<T> pool_pointer<T>::make(Pool& pool, U&&... u) {
        auto* ptr = (pool_pointer<T>::RefObj*)
            pool.alloc(sizeof(pool_pointer<T>::RefObj)); 

        new(&ptr->obj) T(std::forward<U>(u)...);

        ptr->count = 1;
        pool_pointer<T> sp(pool, ptr);
        return sp;
    }
}

#endif

#endif
