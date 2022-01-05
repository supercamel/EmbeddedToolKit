#ifndef ETK_OBJECT_POOL_H
#define ETK_OBJECT_POOL_H

#include "types.h"
#include "array.h"
#include "list.h"

namespace etk
{

    class RefCounter {
        public:
            virtual void inc_references(void* pdata) = 0;
            virtual void dec_references(void* pdata) = 0;
    };

    template <typename T> class PoolPtr
    {
        public:
            PoolPtr()
            {
            }

            PoolPtr(RefCounter* pool) 
                : pData(0), pool(pool)
            {
            }

            PoolPtr(RefCounter* pool, T* pValue) 
                : pData(pValue), pool(pool)
            {
                if(pool != nullptr) {
                    pool->inc_references(pData);
                }
            }

            PoolPtr(const PoolPtr<T>& sp) 
                : pData(sp.pData), pool(sp.pool)
            {
                if(pool != nullptr) {
                    pool->inc_references(pData);
                }
            }

            ~PoolPtr()
            {
                if(pool) 
                {
                    pool->dec_references(pData);
                }
            }

            T& operator* ()
            {
                return *pData;
            }

            T* operator-> ()
            {
                return pData;
            }

            void unref()
            {
                if(pool)
                {
                    pool->dec_references(pData);
                }
            }

            operator bool()
            {
                return pData != nullptr;
            }

            PoolPtr<T>& operator = (const PoolPtr<T>& sp)
            {
                if (this != &sp)
                {
                    if(pool)
                    {
                        pool->dec_references(pData);
                    }
                    pData = sp.pData;
                    pool = sp.pool;
                    if(pool)
                    {
                        pool->inc_references(pData);
                    }
                }

                return *this;
            }

            bool operator == (const PoolPtr& sp)
            {
                return &pData == &sp.pData;
            }

            bool operator != (const PoolPtr& sp)
            {
                return &pData != &sp.pData;
            }

            bool operator == (const std::nullptr_t n) {
                return ((pData == nullptr) || (pool == nullptr));
            }

        private:
            T* pData = 0;
            RefCounter* pool = 0;
    };


    template <typename T, uint32 N> class ObjPool : private RefCounter
    {
        public:
            ObjPool()
            {
                begin();
            }

            void begin()
            {
                free_head = &blocks[0];

                blocks[0].head.prev = nullptr;
                for(int i = 0; i < N-1; i++) {
                    blocks[i].head.references = 0;
                    blocks[i].head.next = (void*)&blocks[i+1];
                }
                blocks[N-1].head.next = nullptr;
                navailable = N;
            }

            bool available()
            {
                return free_head != nullptr;
            }

            uint32 n_available() 
            {
                return navailable;
            }

            PoolPtr<T> alloc_ptr()
            {
                T* r = alloc();
                if(r == nullptr) {
                    return PoolPtr<T>(this, nullptr);
                }
                else {
                    new((void*)r) T();
                    return PoolPtr<T>(this, r);
                }
            }

            T* alloc()
            {
                if(free_head != nullptr) {
                    Block* n = free_head;
                    free_head = (Block*)free_head->head.next;
                    n->head.references = 0;
                    navailable--;
                    return (T*)n->t;
                }
                else {
                    return nullptr;
                }
            }

            void free(T* ptr)
            {
                uint8* bptr = (uint8*)ptr;
                bptr -= sizeof(Head);
                Block* block = (Block*)bptr;

                if(free_head != nullptr) 
                {
                    free_head->head.prev = block;
                }
                block->head.next = free_head;
                block->head.prev = nullptr;

                free_head = block;
                navailable++;
            }

        private:
            friend PoolPtr<T>;

            struct Head {
                unsigned int references;
                void* next;
                void* prev;
            };

            struct Block {
                Head head;
                uint8 t[sizeof(T)];
            };

            Block* free_head;
            Block blocks[N];

            uint32 navailable;


            void inc_references(void* ptr)
            {
                inc_references((T*)ptr);
            }

            void inc_references(T* ptr)
            {
                if(ptr != nullptr) 
                {
                    uint8* bptr = (uint8*)ptr;
                    bptr -= sizeof(Head);
                    Block* block = (Block*)bptr;

                    block->head.references++;
                }
            }

            void dec_references(void* ptr)
            {
                dec_references((T*)ptr);
            }

            void dec_references(T* ptr)
            {
                if(ptr != nullptr) 
                {
                    uint8* bptr = (uint8*)ptr;
                    bptr -= sizeof(Head);
                    Block* block = (Block*)bptr;

                    block->head.references--;

                    if(block->head.references == 0) {
                        ptr->~T();
                        if(free_head != nullptr) {
                            free_head->head.prev = block;
                        }
                        block->head.next = free_head;
                        block->head.prev = nullptr;

                        free_head = block;
                        navailable++;
                    }
                }
            }
    };

}


#endif // ETK_OBJECT_POOL_H
