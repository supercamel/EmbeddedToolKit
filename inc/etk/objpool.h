#ifndef ETK_OBJECT_POOL_H
#define ETK_OBJECT_POOL_H

#include <new> 
#include "types.h"
#include "pool.h"

namespace etk
{
    // an interface for a class that can allocate only a single type of object
    template<typename T> class ObjectAllocator
    {
        public:
            // allocates space for a new object. note the object is not constructed
            virtual T* alloc() = 0;
            // deallocates space for an object. note the object is not destructed
            virtual bool free(T* obj) = 0;
    };


    /*
    A ObjectArrayAllocator reserves a block of memory and allocates objects from it.
    The memory is created as an array, and the objects are allocated from the array.
    It has no memory overhead for the objects, but it has a fixed size.
    */
    template<typename T, uint32 N_OBJECTS> class ObjectArrayAllocator : public ObjectAllocator<T>
    {
        public:
            ObjectArrayAllocator()
            {
                free_head = blocks;

                uint32 count = 0;
                while (count < N_OBJECTS)
                {
                    blocks[count].next = &blocks[count + 1];
                    count++;
                }
                blocks[N_OBJECTS - 1].next = nullptr;
            }

            T* alloc()
            {
                Block *n = free_head;
                if(n == nullptr) {
                    return nullptr;
                }

                free_head = free_head->next;
                T* t = (T*)n;
                return t;
            }

            bool free(T *obj_ptr)
            {
                void* ptr = (void*)obj_ptr;
                if ((ptr >= &blocks[0]) && (ptr <= &blocks[N_OBJECTS - 1]))
                {
                    Block *n = (Block*)ptr;
                    n->next = free_head;
                    free_head = n;
                    return true;
                }
                return false;
            }

            int available() {
                int count = 0;
                Block *n = free_head;
                while (n != nullptr)
                {
                    count++;
                    n = n->next;
                }
                return count;
            }

        private:
            union Block
            {
                Block *next;
                uint8 space[sizeof(T)];
            };

            Block blocks[N_OBJECTS];

            Block *free_head;
    };

    template<typename T> class ObjectPoolAllocator : public ObjectAllocator<T>
    {
        public:
            ObjectPoolAllocator(Pool* pool)
            {
                this->pool = pool;
            }

            T* alloc()
            {
                return (T*)pool->alloc(sizeof(T));
            }

            bool free(T *obj_ptr)
            {
                return pool->free(obj_ptr);
            }

        private:
            Pool *pool;
    };
}


#endif // ETK_OBJECT_POOL_H
