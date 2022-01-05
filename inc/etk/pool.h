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
#include "bitfield.h"
#include "math_util.h"

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
 * ProTip: If you're doing a lot of allocating and freeing with a MemPool, call coalesce() after free().
 *     This will join together free blocks that are adjacent to each other. It helps minimize fragmentation and
 *         could help speed up the next call to alloc().
 */

#include <iostream>
using namespace std;


namespace etk
{

    namespace experimental
    {

        class Pool
        {
            public:
                virtual void* alloc(uint32 sz) = 0;
                virtual void free(void* ptr, uint32 sz) = 0;
                virtual void* realloc(void* ptr, uint32 sz, uint32 oldsz) = 0;
                virtual void coalesce() = 0;
        };

        class Heap : public Pool
        {
            public:
                void* alloc(uint32 sz) {
                    return malloc(sz);
                }
                void free(void* ptr, uint32 sz = 0) {
                    unused(sz);
                    ::free(ptr);
                }
                void* realloc(void* ptr, uint32 sz, uint32 oldsz = 0) {
                    return ::realloc(ptr, sz);
                }
                void coalesce() {
                }
        };


        /**
         * This is a memory pool implementation that uses a free list to track
         * unused chunks.
         *
         * SIZE = number of bytes to reserve for the pool
         * CHUNK_SIZE = the size of the blocks. bigger blocks usually perform 
         * better than small blocks, but will reduce the number of available 
         * blocks to allocate. 
         *
         */
        template <uint32 SIZE, uint32 CHUNK_SIZE = 64> class MemPool : public Pool
        {
            public:
                MemPool()
                {
                    begin();
                }

                /**
                 * Some systems use startup code designed for C.
                 * In this case, C++ constructors for global objects are not
                 * executed when the program begins. Here we provide an 
                 * alternative way to initialise the object.
                 */
                void begin()
                {
                    static_assert(SIZE%CHUNK_SIZE == 0, "The memory pool size must be a multiple of the chunk size.");
                    static_assert(CHUNK_SIZE > sizeof(BlockHead), "CHUNK_SIZE is too small.");

                    free_head = &blocks[0];
                    blocks[0].head.size = TOTAL_CHUNKS;
                    blocks[0].head.next = nullptr;
                    blocks[0].head.prev = nullptr;
                }

                /**
                 * alloc allocates a block of memory of at least sz bytes.
                 */
                void* alloc(uint32 sz)
                {
                    void* r = nullptr;
                    //first see if there is a suitable block
                    r = alloc_from_free_list(sz);
                    if(r == nullptr) {
                        //we're out of memory, so try joining together all the adjacent free blocks to see if they release a region large enough
                        coalesce();
                        //after merging free blocks, there might be a large enough block free
                        r = alloc_from_free_list(sz);
                    }        

                    if(r == nullptr) {
                        cout << "alloc failed" << endl;
                    }
                    return r;
                }

                /**
                 * realloc will resize an allocation to make it either bigger or smaller
                 */
                void* realloc(void* ptr, uint32 sz, uint32 oldsz = 0)/*{{{*/
                {
                    if(ptr == nullptr)
                    {
                        return alloc(sz);
                    }

                    uint8* bptr = (uint8*)ptr;
                    bptr -= sizeof(BlockHead);
                    Block* block = (Block*)bptr;
                    uint32 old_chunks = block->head.size;


                    // calculate the number of chunks to allocate
                    uint32 N = sz+sizeof(BlockHead);
                    uint32 n_chunks = N / CHUNK_SIZE + ((N % CHUNK_SIZE != 0) * 1) ;

                    //the allocation needs to be shrunk down
                    if(old_chunks > n_chunks)
                    {
                        split_block(block, n_chunks);
                        return ptr;
                    }
                    //the allocation needs to grow larger
                    else if(old_chunks < n_chunks)
                    {
                        // TODO try to coalesce adjacent blocks
                        // before calling free and alloc
                        free(ptr);
                        void* n = alloc(sz);
                        if(n == nullptr)
                        {
                            return nullptr;
                        }

                        char* pptr = (char*)ptr;
                        char* nptr = (char*)n;
                        if(pptr != nptr) {
                            for(uint32 i = 0; i < sz; i++) {
                                nptr[i] = pptr[i];
                            }
                        }

                        return n;
                    }
                    else 
                    {
                        return ptr;
                    }
                }
                /*}}}*/

                /* 
                 * free releases a block of memory back to the memory pool.
                 */
                void free(void* ptr, uint32 sz = 0)/*{{{*/
                {
                    unused(sz);
                    // freeing a block adds it to the tail
                    uint8* bptr = (uint8*)ptr;
                    bptr -= sizeof(BlockHead);
                    Block* block = (Block*)bptr;


                    free_head->head.prev = block;
                    //block->head.size = ssz;
                    block->head.next = free_head;
                    block->head.prev = nullptr;

                    free_head = block;
                }/*}}}*/

                /**
                 * Joins adjacent free blocks together.
                 */
                void coalesce()/*{{{*/
                {
                    int32 i = 0;
                    while(i < TOTAL_CHUNKS)
                    {
                        if(block_is_free(&blocks[i])) {
                            int32 first = i;
                            i += blocks[i].head.size;
                            while(block_is_free(&blocks[i])) {
                                blocks[first].head.size += blocks[i].head.size;

                                if(&blocks[i] == free_head) {
                                    free_head = (Block*)blocks[i].head.next;
                                    blocks[i].head.prev = nullptr;
                                }

                                //remove the block from the free block list - cause it's not free no more
                                if(blocks[i].head.next != nullptr) {
                                    reinterpret_cast<Block*>(
                                            blocks[i].head.next)->head.prev =
                                        blocks[i].head.prev;
                                }
                                if(blocks[i].head.prev != nullptr) {
                                    reinterpret_cast<Block*>(
                                            blocks[i].head.prev)->head.next = 
                                        blocks[i].head.next;
                                }

                                i += blocks[i].head.size;
                            }
                        }
                        else {
                            i += blocks[i].head.size;
                        }
                    }
                }
                /*
                 * version that uses bit field to look up if a block is allocated
                 * doesn't appear to be faster
                 void coalesce()
                 {
                 construct_abf();
                 int32 i = 0;
                 while(i < TOTAL_CHUNKS)
                 {
                 if(abf.get(i)) {
                 int32 first = i;
                 i += blocks[i].head.size;
                 while(abf.get(i)) {
                 blocks[first].head.size += blocks[i].head.size;

                 if(&blocks[i] == free_head) {
                 free_head = (Block*)blocks[i].head.next;
                 if(free_head != nullptr) {
                 free_head->head.prev = nullptr;
                 }
                 }
                 else {
                //remove the block from the free block list - cause it's not free no more
                if(blocks[i].head.next != nullptr) {
                reinterpret_cast<Block*>(blocks[i].head.next)->head.prev 
                = blocks[i].head.prev;
                }
                if(blocks[i].head.prev != nullptr) {
                reinterpret_cast<Block*>(blocks[i].head.prev)->head.next 
                = blocks[i].head.next;
                }
                }


                i += blocks[i].head.size;
                }
                }
                else {
                i++;
                }
                }

                construct_abf();
                }*/

            private:

                static const uint32 TOTAL_CHUNKS = SIZE/CHUNK_SIZE;

                struct BlockHead {
                    uint16 size;
                    void* next;
                    void* prev;
                };

                typedef union Block
                {
                    BlockHead head;
                    uint8 bytes[CHUNK_SIZE];
                    void* alignment; // not used, here to align with memory boundary
                }Block;


                static_assert(sizeof(Block) == CHUNK_SIZE, "Block does not equal chunk size");

                /**
                 * split_block splits a large chunk into two smaller chunks 
                 * and adds the second chunk to the free list.
                 */
                void split_block(Block* n, uint32 split_pos) {
                    Block* sp = &n[split_pos];
                    free_head->head.prev = sp;
                    sp->head.size = n->head.size-split_pos;
                    sp->head.next = free_head;
                    sp->head.prev = nullptr;
                    free_head = sp;

                    n->head.size = split_pos;
                }
                /**
                 * Finds and allocates a free chunk that is at least as big as sz.
                 */
                void* alloc_from_free_list(uint32 sz)
                {
                    // calculate the number of blocks to allocate
                    uint32 N = sz+sizeof(BlockHead);
                    uint32 n_blocks = N / CHUNK_SIZE + ((N % CHUNK_SIZE != 0) * 1) ;

                    // iterate over the free blocks
                    Block* n = free_head;
                    while(n != nullptr)
                    {
                        //if this block has sufficient space
                        if(n->head.size >= n_blocks)
                        {
                            //if the block is too big, split it
                            if(n->head.size > n_blocks)
                            {
                                //create second block at the end of this chunk of memory
                                split_block(n, n_blocks);
                            }

                            // TODO this line may be unecessary??
                            //n->head.size = n_blocks;

                            // if the free head is being allocated
                            if(n == free_head) {
                                // move the free head to the next free chunk
                                free_head = (Block*)n->head.next;
                                // link the next chunk to the prior chunk
                                if(free_head != nullptr) {
                                    free_head->head.prev = n->head.prev;
                                }
                            }
                            else {
                                //remove the block from the free block list - cause it's not free no more
                                if(n->head.next != nullptr) {
                                    reinterpret_cast<Block*>(n->head.next)->head.prev = n->head.prev;
                                }
                                if(n->head.prev != nullptr) {
                                    reinterpret_cast<Block*>(n->head.prev)->head.next = n->head.next;
                                }
                            }

                            //return a pointer to the start of the allocated chunk
                            return (void*)&n->bytes[sizeof(BlockHead)];
                        }
                        n = (Block*)n->head.next;
                    }

                    return nullptr;
                }


                /**
                 * Returns true if the pointer is in the free block list
                 * TODO maybe implement a bit field to store if a chunk is 
                 * allocated or not
                 * or maybe not?? 
                 */
                bool block_is_free(Block* block)
                {
                    Block* n = free_head;
                    while(n != nullptr)
                    {
                        if(n == block) 
                        {
                            return true;
                        }
                        n = (Block*)n->head.next;
                    }
                    return false;
                }

                /*
                   void construct_abf() 
                   {
                   abf.zero(); // turn all bits off
                   Block* n = free_head;
                   while(n != nullptr)
                   {
                // find the block number from pointer address

                uint32 blocknumber = (n - blocks);
                abf.set(blocknumber);
                n = (Block*)n->head.next;
                }


                int i = 0; 
                while(i < TOTAL_CHUNKS) {
                if(abf.get(i)) {
                while(abf.get(i++)) { };
                }
                else {
                i++;
                }
                }
                }
                */

                Block* free_head;
                Block blocks[TOTAL_CHUNKS];

                // allocation bit field
                //BitField<TOTAL_CHUNKS> abf;
        };


        }

        }

#endif
