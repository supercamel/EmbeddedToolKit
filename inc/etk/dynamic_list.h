/*
   Embedded Tool Kit
   Copyright (C) 2015 Samuel Cowen

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   */

#ifndef ETK_DYNAMIC_LIST_H_INCLUDED
#define ETK_DYNAMIC_LIST_H_INCLUDED

#ifdef __AVR__
inline void *operator new(size_t, void *buf) {
    return buf;
}
#else
#include <new>
#endif

#include "pool.h"
#include "types.h"
#include "math_util.h"


namespace etk
{

    /**
     * \class DynamicList
     *
     * \brief The dynamic list class is an iterable container object.
     * DynamicList has a very similar feel to List but uses a memory pool to provide memory. 
     *
     * When the dynamic list needs to resize its memory allocation, it will
     * increase the number of objects it can hold by RESIZE_STEP. 
     * e.g. RESIZE_STEP = 4. If size() == 12 and append is called, it will resize to hold 
     * 16 objects. 
     * @tparam T The type of object that the list contains.
     */


    template <typename T, uint32 RESIZE_STEP = 1> class DynamicList
    {
        public:
            DynamicList(Pool* _pool) 
            {
                pool = _pool;
                //space = pool->alloc(sizeof(T)*2);
                reserved = 0;
                list_end = -1;
                space = nullptr;
            }

            ~DynamicList()
            {
                T* pt = (T*)space;
                for(uint32 i = 0; i < size(); i++)
                    (&pt[i])->~T();
                if(space != nullptr)
                {
                    pool->free(space);
                }
            }

            DynamicList(DynamicList& list)
            {
                pool = list.pool;
                void* tmp_space = space;
                space = pool->realloc(space, sizeof(T)*list.reserved);
                if(space == nullptr)
                {
                    space = tmp_space;
                    return;
                }
                reserved = list.reserved;

                memcpy(space, list.space, sizeof(T)*list.reserved);
                list_end = list.list_end;
            }

            DynamicList(const DynamicList& list)
            {
                pool = list.pool;
                void* tmp_space = space;
                space = pool->realloc(space, sizeof(T)*list.reserved);
                if(space == nullptr)
                {
                    space = tmp_space;
                    return;
                }
                reserved = list.reserved;

                memcpy(space, list.space, sizeof(T)*list.reserved);
                list_end = list.list_end;
            }

            // assignment operator
            DynamicList& operator = (const DynamicList& sp)
            {
                // if not assigning to self
                if (space != sp.space)
                {
                    pool = sp.pool;
                    void* tmp_space = space;
                    space = pool->realloc(space, sizeof(T)*sp.reserved);
                    if(space == nullptr)
                    {
                        space = tmp_space;
                        return *this;
                    }
                    reserved = sp.reserved;

                    memcpy(space, sp.space, sizeof(T)*sp.reserved);
                    list_end = sp.list_end;
                }
                return *this;
            }


            /**
             * \class Iterator
             *
             * \brief The list iterator makes a list iterable using C++11 ranged for loop syntax.
             * @code
             etk::List<int, 10> list;
             ...
            //print out the entire list
            for(auto i : list)
            {
            cout << i << " ";
            }
            @endcode
             *
             */
            class Iterator
            {
                friend class DynamicList;
                public:
                Iterator(DynamicList& list) : list(&list)
                {
                    pos = 0;
                }

                T& operator*()
                {
                    return (*list)[pos];
                }

                Iterator& operator++ ()
                {
                    pos++;
                    return *this;
                }

                Iterator operator++(int)
                {
                    Iterator iter(*this);
                    ++(*this);
                    return iter;
                }

                bool operator==(Iterator iter)
                {
                    return (pos == iter.pos);
                }

                bool operator!=(Iterator iter)
                {
                    return (pos != iter.pos);
                }

                private:
                DynamicList* list;
                uint32 pos;
            };


            /**
             * \brief Returns an iterator to the first item in the list.
             */
            Iterator begin()
            {
                Iterator iter(*this);
                iter.pos = 0;
                return iter;
            }

            /**
             * \brief Returns an iterator to the end of the list.
             */
            Iterator end()
            {
                Iterator iter(*this);
                iter.pos = list_end+1;
                return iter;
            }

            /**
             * \brief Adds an item to the end of the list and increments the size of the list by 1.
             * \Returns true on success or false if it has failed to allocate more memory.
             */
            bool append(T& t)
            {
                list_end++;
                if(resize() == false) 
                {
                    list_end--;
                    return false;
                }
                else 
                {
                    T* pt = (T*)space;
                    pt += list_end;
                    new(pt)T(t);
                    return true;
                }
            }

            /**
             * \brief Inserts an item into the list.
             * @arg T the item to be inserted.
             * @arg pos where the item will be inserted. If pos is zero, then it will be inserted at the start of the list. If it's 1, it will become the second item in the list.
             */
            bool insert(T& t, uint32 pos)
            {
                if(pos <= size())
                {
                    list_end++;
                    if(resize() == false) {
                        list_end--;
                        return false;
                    }

                    T* pt = (T*)space;
                    for(uint32 i = size(); i >= (pos+1); i--)
                        pt[i] = pt[i-1];

                    new(&pt[pos])T(t);
                    list_end++;

                    return true;
                }
                return false;
            }

            /**
             * \brief Removes an item from the list.
             * @arg pos The position of the item to remove.
             * @arg padding A value / default object to fill in the space at the end of the list.
             */
            void remove(uint32 pos)
            {
                T* pt = (T*)space;
                (&pt[pos])->~T();
                for(uint32 i = pos; i < size()-1; i++)
                    pt[i] = pt[i+1];
                list_end--;

                resize();
            }


            void remove_item(T& item)
            {
                for(uint32 i = 0; i < size(); i++)
                {
                    T* pt = (T*)space;
                    if(*pt[i] == item)
                    {
                        remove(i);
                    }
                }
            }


            /**
             * \brief Removes len number of items from the list, starting as pos.
             * @arg pos The position of the first item to erase.
             * @arg len The number of items to remove.
             */
            void erase(uint32 pos, uint32 len)
            {
                for(uint32 i = 0; i < len; i++)
                    remove(pos);
            }

            /**
             * \brief Destroys every item in the list.
             */
            void clear()
            {
                T* pt = (T*)space;
                for(uint32 i = 0; i < size(); i++) 
                {
                    pt->~T();
                    pt++;
                }

                list_end = -1;
                resize();
            }

            /**
             * \brief Returns the number of a particular item in the list.
             * @arg t The item to count.
             * @return The number of these items in the list.
             */
            uint32 count(T& t)
            {
                uint32 c = 0;
                T* pt = (T*)space;
                auto sz = size();
                for(uint32 i = 0; i < sz; i++)
                {
                    if(t == pt[i])
                        c++;
                }
                return c;
            }

            /**
             * \brief Sets a section of the list to a given value.
             * @arg start The starting position.
             * @arg end The end position.
             * @arg f The item to fill with.
             */
            void fill(uint32 start, uint32 end, T& f)
            {
                T* pt = (T*)space;
                end = min(end, size());
                start = min(start, end);
                for(uint32 i = start; i < end; i++)
                    pt[i] = f;
            }

            /**
             * \brief Returns the last item on the list and reduces the list length by 1.
             * @return The item at the end of the list.
             */
            T pop_back()
            {
                T* pt = (T*)space;
                if(list_end > 0)
                {
                    T ret = pt[list_end];
                    (&pt[list_end])->~T();
                    list_end--;
                    resize();
                    return ret;
                }
                return T();
            }

            /**
             * \brief Same as append();
             */
            void push_back(T& t)
            {
                append(t);
            }

            /**
             * \brief This operator allows you to access elements of the list just like a normal array.
             */
            T& operator[](uint32 pos)
            {
                T* pt = (T*)space;
                return pt[pos];
            }

            T& get(uint32 pos)
            {
                return (*this)[pos];
            }

            T at(uint32 pos)
            {
                return (*this)[pos];
            }

            /**
             * \brief Returns the number of items in the list.
             */
            uint32 size()
            {
                return list_end+1;
            }

            /**
             * \brief Returns a const pointer to the array.
             */
            const T* buffer() const
            {
                return (T*)space;
            }

            /**
             * \brief Returns a pointer to the raw memory used by List.
             */
            T* raw_memory()
            {
                return (T*)space;
            }

            void print_pool_ptr()
            {

                //cout << "list pool addr: " <<  (uint64)pool << endl;
            }
        private:

            bool resize() 
            {
                if(reserved <= list_end) 
                {
                    void* tmp_space = space;
                    reserved += RESIZE_STEP;

                    print_pool_ptr();
                    space = pool->realloc(space, sizeof(T)*reserved);
                    if(space == nullptr) 
                    {
                        space = tmp_space;
                        reserved -= RESIZE_STEP;
                        return false;
                    }
                    return true;
                }
                else if(reserved > (list_end+RESIZE_STEP))
                {
                    if(reserved > RESIZE_STEP) {
                        reserved -= RESIZE_STEP;
                        space = pool->realloc(space, sizeof(T)*reserved);
                    }
                    return true;
                }
                else
                {
                    return true;
                }
            }

            Pool* pool;
            void* space;
            int32 reserved = 0;
            int32 list_end = 0;
    };

}
#endif
