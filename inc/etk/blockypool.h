#ifndef BLOCKYPOOL_H_INCLUDED
#define BLOCKYPOOL_H_INCLUDED

#include "pool.h"

#include <iostream>
using namespace std;


namespace etk
{
    namespace experimental
    {


        /**
         * The memory pool divides the memory into chunks of free memory and tracks them as a double linked list.
         *
         * Memory allocation works like this
         *      1. A free block with size > request is found.
         *      2. 
         *
         */
        template <uint32 SIZE> class BlockyPool : public Pool
        {
            public:
                /**
                 * The constructor creates one big block at the start of the memory region and adds it to the free block list.
                 */
                BlockyPool()
                {
                    begin();
                }

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
                 *
                 * First it tries to find a big enough free block. If that fails, it attempts to merge adjacent free blocks together.
                 * If it still can't get a big enough block, it returns a null pointer.
                 */
                void* alloc(uint32 sz)/*{{{*/
                {
                    //cout << "alloc: " << sz << endl;
                    //first see if there is a suitable block
                    void* r = alloc_from_free_list(sz);
                    //return on success
                    if(r != nullptr) {
                        return r;
                    }


                    //we're out of memory, so try joining together all the adjacent free blocks to see if they release a region large enough
                    coalesce_free_blocks();
                    //after merging free blocks, there might be a large enough block free
                    r = alloc_from_free_list(sz);
                    if(r != nullptr) {
                        return r;
                    }        

                    //can't allocate any more memory.
                    return nullptr;
                }/*}}}*/

                void* realloc(void* ptr, uint32 sz, uint32 oldsz)/*{{{*/
                {
                    if(ptr == nullptr)
                    {
                        return alloc(sz);
                    }

                    //cout << "realloc" << endl;

                    // calculate the number of chunks to allocate

                    uint32 n_chunks = sz/CHUNK_SIZE;
                    if((n_chunks == 0) || (sz%CHUNK_SIZE)) {
                        n_chunks += 1;
                    }

                    uint32 old_chunks = oldsz/CHUNK_SIZE;
                    if((old_chunks == 0) || (oldsz%CHUNK_SIZE)) {
                        old_chunks += 1;
                    }

                    //cout << "new size: " << sz << endl << endl;

                    //the allocation needs to be shrunk down
                    if(old_chunks > n_chunks)
                    {
                        //cout << "shrinking" << endl;
                        split_block((Block*)ptr, n_chunks);

                        print_free_list();
                        return ptr;
                    }
                    //the allocation needs to grow larger
                    else if(old_chunks < n_chunks)
                    {
                        //cout << "realloc growing" << endl;
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


                        free(ptr, oldsz);
                        //cout << "done realloc growing" << endl;
                        return n;
                    }
                    else 
                    {
                        return ptr;
                    }
                }
                /*}}}*/

                /* free releases a block of memory back to the memory pool.
                 *
                 * The block is added to the free block list.
                 * It is assumed that the size of the block hasn't been wrecked by some stray, memory trashing bug elsewhere in the application.
                 * I guess everything is based on that assumption, aye? :-)
                 */
                void free(void* ptr, uint32 sz)/*{{{*/
                {
                    // freeing a block adds it to the tail
                    Block* block = (Block*)ptr;

                    //cout << "freeing " << i << endl;

                    uint32 ssz = sz/CHUNK_SIZE;
                    if((ssz == 0) || (sz%CHUNK_SIZE)) {
                        ssz += 1;
                    }
                    //cout << "ssz: " << ssz << endl;
                    free_head->head.prev = block;
                    block->head.size = ssz;
                    block->head.next = free_head;
                    block->head.prev = nullptr;

                    free_head = block;
                    //cout << "free failed!" << endl;
                }/*}}}*/
                /**
                 * Joins free blocks together.
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
                            i++;
                        }
                    }
                    print_free_list();
                    //cout << "coalesce done" << endl;
                }/*}}}*/
                /**
                 * coalesce_free_blocks scans through the list of free blocks and merges any adjacent blocks together.
                 *
                 * This helps minimize fragmentation. Remember the old windoze 95 hard drive defrag tool?
                 */
                void coalesce_free_blocks()/*{{{*/
                {
                    coalesce();
                }/*}}}*/

                void dump()/*{{{*/
                {
                    /*
                       ofstream fout;
                       fout.open("memory_dump.bin", ios::binary | ios::out);
                       fout.write((char*) memory, SIZE);
                       fout.close();
                       */
                }
                /*}}}*/

                void print_free_list()/*{{{*/
                {
                    /*
                       cout << "print_free_list" << endl;
                       int count = 0;
                       int32 n = free_head;
                       while(n >= 0)
                       {
                       cout << "free: " << n << flush << " " << blocks[n].head.size << endl;
                       n = blocks[n].head.next;

                       count++;
                       if(count > 100) {
                       char b;
                       cin >> b;
                       }
                       }
                       */
                }
                /*}}}*/
            private:

                static const uint32 CHUNK_SIZE = 128;
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


                static_assert(sizeof(Block) == CHUNK_SIZE, "Block does not equal chunck size");

                void split_block(Block* n, uint32 split_pos) {
                    Block* sp = &n[split_pos];
                    free_head->head.prev = sp;
                    sp->head.size = n->head.size-split_pos;
                    sp->head.next = free_head;
                    sp->head.prev = nullptr;
                    //cout << "alloc_from_free_list free head = " << n+n_blocks << endl;
                    free_head = sp;
                }
                /**
                 * Finds and allocates a free block that is at least as big as sz.
                 */
                void* alloc_from_free_list(uint32 sz)
                {
                    // calculate the number of chunks to allocate
                    uint32 n_blocks = sz/CHUNK_SIZE;
                    if((n_blocks == 0) || (sz%CHUNK_SIZE)) {
                        n_blocks += 1;
                    }

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

                            //blocks[n].head.size = n_blocks;

                            if(n == free_head) {
                                free_head = (Block*)n->head.next;
                                n->head.prev = nullptr;
                            }

                            //remove the block from the free block list - cause it's not free no more
                            if(n->head.next != nullptr) {
                                reinterpret_cast<Block*>(n->head.next)->head.prev = n->head.prev;
                            }
                            if(n->head.prev != nullptr) {
                                reinterpret_cast<Block*>(n->head.prev)->head.next = n->head.next;
                            }

                            //return a pointer to the start of the allocated chunk
                            return (void*)n;
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

                Block* free_head;

                Block blocks[TOTAL_CHUNKS];
        };

    }
}


#endif


