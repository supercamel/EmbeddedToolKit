/*
    Copyright (C) 2016 Samuel Cowen samuel.cowen@camelsoftware.com

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/

#ifndef POOL_H_INCLUDED
#define POOL_H_INCLUDED

#include <stdlib.h>
#include "types.h"

/*
#include <iostream>
using namespace std;
*/

/**
 * The Pool class is a memory pool - aka a disposable heap.
 *
 * Pros:
 *  - Generally faster than malloc/free
 *  - Eliminates the risk of heap fragmentation
 *  - Generally gets destroyed after use which almost eliminates the danger of fragmentation
 *
 * Computers and micro-controllers require dynamic memory allocation to solve complex problems.
 * Such problems include:
 *  - Path finding & navigation
 *  - Language processing (compilers, interpreters, NLP, etc)
 *  - Searching algorithms
 *
 *
 * However, with great power comes great responsibility!
 * Normal dynamic memory allocation (using new/delete or malloc/free) comes with risks. Memory leaks
 * and heap fragmentation are a couple. malloc/new are also a little non-deterministic in that you can never
 * know exactly how long it will take for malloc or new to perform an allocation (memory pools don't eliminate
 * this, but they are generally faster). This is why the use of malloc and new is strongly discouraged or sometimes
 * prohibited in serious embedded applications.
 *
 * By far the most important pro of memory pools is that they can be completely destroyed when they are no longer required.
 * The entire pool can simply popped off the stack. All that memory would be freed in an instant, along with any
 * fragmented pockets of memory.
 *
 * ProTip: Put MemPools on the stack whenever possible - and don't call free() unless you actually need to.
 *
 * ProTip: If you're doing a lot of allocating and freeing with a MemPool, call coalesce_free_blocks() after free().
 * 	   This will join together free blocks that are adjacent to each other. It helps minimize fragmentation and
 *         could help speed up the next call to alloc().
 */

namespace etk
{

namespace experimental
{

class Pool
{
public:
    virtual void* alloc(uint32 sz) = 0;
    virtual void free(void* ptr) = 0;
};

class Heap : public Pool
{
public:
    void* alloc(uint32 sz) {
        return malloc(sz);
    }
    void free(void* ptr) {
        ::free(ptr);
    }
};

template <uint32 SIZE> class MemPool : public Pool
{
public:
    /**
     * The constructor creates one big block at the start of the memory region and adds it to the free block list.
     */
    MemPool()
    {
        Block* pblock = (Block*)&memory[0];
        pblock->size = SIZE;
        pblock->next = nullptr;
        pblock->prev = &free_head;

        free_head.next = pblock;
        free_head.prev = nullptr;
    }

    /**
     * alloc allocates a block of memory of at least sz bytes.
     *
     * First it tries to find a big enough free block. If that fails, it attempts to merge adjacent free blocks together.
     * If it still can't get a big enough block, it returns a null pointer.
     */
    void* alloc(uint32 sz)
    {
        sz += sizeof(Block);

        //first see if there is a suitable block
        void* r = alloc_from_free_list(sz);
        //return on success
        if(r != nullptr)
            return r;


        //we're out of memory, so try joining together all the adjacent free blocks to see if they release a region large enough
        coalesce_free_blocks();
        //after merging free blocks, there might be a large enough block free
        r = alloc_from_free_list(sz);
        if(r)
            return r;

        //can't allocate any more memory.
        return nullptr;
    }

    void* realloc(void* ptr, uint32 sz)
    {
        void* n = alloc(sz);
        if(n == nullptr)
            return nullptr;

        char* pptr = (char*)ptr;
        char* nptr = (char*)n;
        for(uint32 i = 0; i < sz; i++)
            nptr[i] = pptr[i];

        if(ptr != nullptr)
            free(ptr);
        return n;
    }

    /**
     * free releases a block of memory back to the memory pool.
     *
     * The block is added to the free block list.
     * It is assumed that the size of the block hasn't been wrecked by some stray, memory trashing bug elsewhere in the application.
     * I guess everything is based on that assumption, aye? :-)
     */
    void free(void* ptr)
    {
        uint8* cptr = (uint8*)ptr;
        Block* pblock = reinterpret_cast<Block*>(cptr-sizeof(Block));
        add_to_list(pblock);
    }

    /**
     * coalesce_free_blocks scans through the list of free blocks and merges any adjacent blocks together.
     *
     * This helps minimize fragmentation. Remember the old windoze 95 hard drive defrag tool?
     */
    void coalesce_free_blocks()
    {
        bool changes = true;
        while(changes)
        {
            changes = false;
            Block* pblock = free_head.next;
            while(pblock)
            {
                uint8* pnext = (uint8*)pblock;
                pnext = &pnext[pblock->size];

                if(block_is_free(reinterpret_cast<Block*>(pnext)))
                {
                    pblock->size += (reinterpret_cast<Block*>(pnext))->size;
                    remove_from_list(reinterpret_cast<Block*>(pnext));
                    changes = true;
                }
                pblock = pblock->next;
            }
        }
    }

    /*
        void print_free_list()
        {
            Block* pblock = free_head.next;
            while(pblock)
            {
                cout << (uintptr_t)pblock << " " << pblock->size << endl;
                pblock = pblock->next;
            }
        }
    */

private:

    struct Block
    {
        uint32 size = 0;
        Block* next = nullptr;
        Block* prev = nullptr;
    };

    /**
     * Finds and allocates a free block that is at least as big as sz.
     */
    void* alloc_from_free_list(uint32 sz)
    {
        /*
         * alloc_from_free_list tries to find a free block that is big enough. If the free block is too big,
         * it will be split into two pieces to avoid wasting memory.
         *
         * Imagine if you needed only 24 bytes and there was a free block of 512 bytes. You wouldn't want
         * it to use the full 512 block! So, alloc_from_free_list will split free blocks if there's more
         * than chunk_size of wasteage.
         *
         */

        const uint32 chunk_size = 64;

        Block* pblock = free_head.next;
        while(pblock)
        {
            //if this block has sufficient space
            if(pblock->size >= sz)
            {
                //if the block is too big, split it
                if((pblock->size - sz) >= chunk_size)
                {
                    //create second block at the end of this chunk of memory
                    uint32 inc = ((sz/chunk_size)*chunk_size);
                    if(sz%chunk_size)
                        inc += chunk_size;

                    Block* sblock = (Block*)(((char*)pblock)+inc);
                    sblock->size = pblock->size-inc;
                    pblock->size = inc;

                    //add the second block to the free block list
                    add_to_list(sblock);
                }

                //remove the block from the free block list - cause it's not free no more
                remove_from_list(pblock);

                //return a pointer to the start of the allocated chunk
                char* ptr = (char*)pblock;
                return (void*)(ptr+sizeof(Block));
            }
            pblock = pblock->next;
        }

        return nullptr;
    }


    /**
     * Returns true if the pointer is in the free block list
     */
    bool block_is_free(Block* block)
    {
        Block* pblock = free_head.next;
        while(pblock)
        {
            if(pblock == block)
                return true;
            pblock = pblock->next;
        }
        return false;
    }

    /**
     * Removes a block from a list. Assumes the pointer is valid!
     *
     * All it does it re-arrange the next and prev pointers of the next and previous blocks.
     */
    void remove_from_list(Block* block)
    {
        if(block->next)
            block->next->prev = block->prev;
        if(block->prev)
            block->prev->next = block->next;
    }

    /**
     * Adds a block to the free list
     */
    void add_to_list(Block* block)
    {
        Block* pblock = &free_head;
        Block* last_block = nullptr;
        while(pblock)
        {
            last_block = pblock;
            pblock = pblock->next;
        }

        pblock = last_block;
        pblock->next = block;
        block->prev = pblock;
        block->next = nullptr;
    }


    Block free_head;
    uint8 memory[SIZE];
};

}

}

#endif
