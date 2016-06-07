#ifndef POOL_H_INCLUDED
#define POOL_H_INCLUDED


#include "types.h"


/**
 * The Pool class is a memory pool - aka a disposable heap.
 *
 * Pros:
 *  - Faster than malloc/free
 *  - Eliminates the risk of heap fragmentation
 *  - Generally gets destroyed after use which almost eliminates the danger of fragmentation
 *
 * Computers and micro-controllers require dynamic memory allocation to solve complex problems in a non-deterministic way.
 * For example:
 *      A robot needs to find the shortest path around an obstacle course to get home.
 *      There are many randomly placed obstacles and potentially hundreds of thousands of routes through the course.
 *      The path finding algorithm will probably require a tree-like data structure to store possible pathways.
 *
 *      Due to the random nature of the obstacle course, it doesn't make sense to pre-allocate memory for the tree because
 *      it cannot know how many branches and sub-nodes the tree should have. If it did pre-allocate memory there would
 *      be substantial waste and the problem solving ability of the robot would be severely restricted.
 *
 * However, with great power comes great responsibility!
 * Normal dynamic memory allocation (using new/delete or malloc/free) comes with risks. Memory leaks
 * and heap fragmentation are a couple. malloc/new are also a little non-deterministic in that you can never
 * know exactly how long it will take for malloc or new to perform an allocation (memory pools don't eliminate
 * this, but they are generally faster). This is why the use of malloc and new is strongly discouraged or sometimes
 * prohibited in serious embedded applications.
 *
 * By far the most important pro of memory pools is that they can be completely destroyed when they are no longer required.
 * Using a memory pool, the robot can find the shortest path and copy it to a pre-allocated location. The entire pool could
 * then simply popped off the stack. All that memory would be freed in an instant, along with any fragmented pockets of memory.
 *
 */

namespace etk
{

template <uint32 SIZE> class Pool
{
public:
    /**
     * The constructor creates one big block at the start of the memory region and adds it to the free block list.
     */
    Pool()
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
        if(r)
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

    /**
     * free releases a block of memory back to the memory pool.
     *
     * The block is added to the free block list.
     * It is assumed that the size of the block hasn't been wrecked by some stray, memory trashing bug elsewhere in the application.
     * I guess everything is based on that assumption, aye? :-)
     */
    void free(void* ptr)
    {
        char* cptr = (char*)ptr;
        Block* pblock = (Block*)(cptr-sizeof(Block));
        add_to_list(free_head, pblock);
    }

    /**
     * coalesce_free_blocks scans through the list of free blocks and merges any adjacent blocks together.
     *
     * This helps minimize fragmentation. Remember the old windoze 95 hard drive defrag tool?
     */
    void coalesce_free_blocks()
    {
        Block* pblock = free_head.next;
        while(pblock)
        {
            char* pnext = (char*)pblock;
            pnext += pblock->size;

            if(block_is_free((Block*)pnext))
            {
                pblock->size += ((Block*)pnext)->size;
                remove_from_list((Block*)pnext);
            }
            pblock = pblock->next;
        }
    }

private:

    struct Block
    {
        uint32 size = 0;
        Block* next = nullptr;
        Block* prev = nullptr;
    };

    /**
     * Finds a free block that is at least as big as sz.
     */

    void* alloc_from_free_list(uint32 sz)
    {
        /*
         * alloc_from_free_list tries to find a free block that is big enough. If the free block is too big,
         * it will be split into two pieces to avoid wasting memory.
         *
         * Imagine if you needed only 24 bytes and there was a free block of 512 bytes. You wouldn't want
         * it to use the full 512 block! So, alloc_from_free_list will split free blocks if there's more
         * than split_size of wasteage.
         *
         */

        const uint32 split_size = 32;

        Block* pblock = free_head.next;
        while(pblock)
        {
            if(pblock->size >= sz)
            {
                if((pblock->size - sz) >= split_size)
                {
                    Block* sblock = (Block*)(((char*)pblock)+sz);
                    sblock->size = pblock->size-sz;
                    pblock->size = sz;
                    add_to_list(free_head, sblock);
                }

                remove_from_list(pblock);

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
     * It doesn't matter which list the block is on, since it's they are doubly linked lists.
     * All it does it re-arrange the next and prev pointers of the next and previous blocks.
     */
    void remove_from_list(Block* block)
    {
        if(block->prev)
            block->prev->next = block->next;
        if(block->next)
            block->next->prev = block->prev;
    }

    /**
     * Adds a block to either the free list of the allocated list of blocks.
     */
    void add_to_list(Block& head, Block* block)
    {
        Block* pblock = &head;
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


#endif

