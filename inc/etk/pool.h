/*
   Copyright (C) 2022 Samuel Cowen samuel.cowen@camelsoftware.com

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
#include <string.h>
#include "types.h"
#include "math_util.h"


/**
 * A memory pool implementation.
 */

namespace etk
{
    class Pool
    {
        public:
            virtual void* alloc(uint32 sz) = 0;
            virtual void free(void* ptr) = 0;
            virtual void* realloc(void* ptr, uint32 sz) = 0;
            virtual uint32 coalesce() = 0;
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
            void* realloc(void* ptr, uint32 sz) {
                return ::realloc(ptr, sz);
            }
            uint32 coalesce() {
                return 0;
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

                memset(&blocks[0], 0, sizeof(Block)*TOTAL_CHUNKS);

                free_head = &blocks[0];
                blocks[0].head.size = TOTAL_CHUNKS;
                blocks[0].head.next = nullptr;
                blocks[0].head.prev = nullptr;
                blocks[0].head.ref = 0;
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

                return r;
            }

            /**
             * realloc will resize an allocation to make it either bigger or smaller
             */
            void* realloc(void* ptr, uint32 sz)/*{{{*/
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
                    // first attempt to join on any adjacent free blocks
                    uint32 blocknumber = (block - blocks);
                    join_adjacent(blocknumber);

                    if(block->head.size > n_chunks)  // if the block size is now too big
                    {
                        split_block(block, n_chunks); // split it
                        return ptr; // return original pointer because nothing moved
                    }
                    else if(block->head.size == n_chunks) // if the block size is perfect
                    {
                        return ptr; 
                    }
                    else // worst case scenario
                    {
                        // free the memory
                        free(ptr);

                        // attempt allocation 
                        void* n = alloc(sz);
                        if(n == nullptr)
                        {
                            // un-free the block
                            if(block == free_head) {
                                free_head = (Block*)block->head.next;
                                if(free_head != nullptr) {
                                    free_head->head.prev = block->head.prev;
                                }
                            }
                            else {
                                if(block->head.next != nullptr) {
                                    reinterpret_cast<Block*>(block->head.next)->head.prev = block->head.prev;
                                }
                                if(block->head.prev != nullptr) {
                                    reinterpret_cast<Block*>(block->head.prev)->head.next = block->head.next;
                                }
                            }

                            block->head.ref = 1;

                            return nullptr;
                        }
                        // copy contents of the old chunks to the new location
                        memmove(n, ptr, old_chunks*CHUNK_SIZE);
                        return n;
                    }
                }
                // the allocation is fine
                else 
                {
                    return ptr;
                }
            }
            /*}}}*/

            /* 
             * free releases a block of memory back to the memory pool.
             */
            void free(void* ptr)/*{{{*/
            {
                if(ptr != nullptr) 
                {
                    // freeing a block adds it to the tail
                    uint8* bptr = (uint8*)ptr;
                    bptr -= sizeof(BlockHead);
                    Block* block = (Block*)bptr;

                    if(free_head != nullptr)
                    {
                        free_head->head.prev = block;
                    }
                    block->head.next = free_head;
                    block->head.prev = nullptr;
                    block->head.ref = 0;

                    free_head = block;
                }
            }/*}}}*/

            void ref(void* ptr)
            {
                if(ptr != nullptr) 
                {
                    // freeing a block adds it to the tail
                    uint8* bptr = (uint8*)ptr;
                    bptr -= sizeof(BlockHead);
                    Block* block = (Block*)bptr;
                    block->head.ref++;
                }
            }

            uint32_t unref(void* ptr)/*{{{*/
            {
                if(ptr != nullptr) 
                {
                    // freeing a block adds it to the tail
                    //

                    uint8* bptr = (uint8*)ptr;
                    bptr -= sizeof(BlockHead);
                    Block* block = (Block*)bptr;
                    if(block->head.ref > 0) {
                        block->head.ref--;

                        if(block->head.ref == 0) {
                            if(free_head != nullptr)
                            {
                                free_head->head.prev = block;
                            }
                            block->head.next = free_head;
                            block->head.prev = nullptr;
                            block->head.ref = 0;

                            free_head = block;
                        }
                        return block->head.ref;
                    }
                }
                return 0;
            }/*}}}*/


            /**
             * Joins adjacent free blocks together.
             */
            uint32 coalesce()
            {
                uint32 count = 0;
                uint32 i = 0;
                while((i < TOTAL_CHUNKS) && (count < TOTAL_CHUNKS))
                {
                    if(block_is_free(&blocks[i])) {
                        count++;
                        join_adjacent(i);
                    }
                    i += blocks[i].head.size;
                }
                return count;
            }

            void* get_memory() {
                return &blocks[0];
            }

            uint32_t get_free_head() {
                return free_head - blocks;
            }

        private:

            static const uint32 TOTAL_CHUNKS = SIZE/CHUNK_SIZE;

            struct BlockHead {
                uint32 size;
                uint32 ref;
                void* next;
                void* prev;
            };

            typedef union Block
            {
                BlockHead head;
                uint8 bytes[CHUNK_SIZE];
                // not sure if this is actually useful but it also can't hurt
                void* alignment; // not used, here to align with memory boundary
            }Block;


            static_assert(sizeof(Block) == CHUNK_SIZE, "Block does not equal chunk size");


            void join_adjacent(uint32 block_n) 
            {
                uint32 first = block_n;
                block_n += blocks[block_n].head.size;
                if(block_n >= TOTAL_CHUNKS) {
                    return;
                }

                uint32_t count = 0;
                while((block_is_free(&blocks[block_n])) && 
                        (count < TOTAL_CHUNKS)) {
                    blocks[first].head.size += blocks[block_n].head.size;

                    if(&blocks[block_n] == free_head) {
                        free_head = (Block*)blocks[block_n].head.next;
                        blocks[block_n].head.prev = nullptr;
                    }

                    //remove the block from the free block list - cause it's not free no more
                    if(blocks[block_n].head.next != nullptr) {
                        reinterpret_cast<Block*>(
                                blocks[block_n].head.next)->head.prev =
                            blocks[block_n].head.prev;
                    }
                    if(blocks[block_n].head.prev != nullptr) {
                        reinterpret_cast<Block*>(
                                blocks[block_n].head.prev)->head.next = 
                            blocks[block_n].head.next;
                    }

                    //memset(blocks[block_n].bytes, 0, sizeof(Block));
                    blocks[block_n].head.ref = 1;

                    block_n += blocks[block_n].head.size;
                }
            }

            /**
             * split_block splits a large chunk into two smaller chunks 
             * and adds the second chunk to the free list.
             */
            void split_block(Block* n, uint32 split_pos) {
                Block* sp = &n[split_pos];

                if(free_head != nullptr) 
                {
                    free_head->head.prev = sp;
                }
                sp->head.size = n->head.size-split_pos;
                sp->head.next = free_head;
                sp->head.prev = nullptr;
                sp->head.ref = 0;
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
                uint32_t count = 0;
                while((n != nullptr) && (count < TOTAL_CHUNKS))
                {
                    count++;
                    //if this block has sufficient space
                    if(n->head.size >= n_blocks)
                    {
                        //if the block is too big, split it
                        if(n->head.size > n_blocks)
                        {
                            //create second block at the end of this chunk of memory
                            split_block(n, n_blocks);
                        }

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

                        n->head.ref = 1;
                        //return a pointer to the start of the allocated chunk
                        return (void*)&n->bytes[sizeof(BlockHead)];
                    }
                    n = (Block*)n->head.next;
                }

                return nullptr;
            }


            /**
             * Returns true if the pointer is in the free block list
             */
            bool block_is_free(Block* block)
            {
                return block->head.ref == 0;
            }

            Block* free_head;
            Block blocks[TOTAL_CHUNKS];
    };
}

#endif
