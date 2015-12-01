#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stdint.h>


namespace etk
{

class MemPool
{
public:
    MemPool(uint8_t* r, uint32_t ps) : pool_sz(ps)
    {
        root = (Allocation*)r;
		root->next = 0;
		root->size = 0;
    }

    template <typename T> T* malloc(uint32_t sz)
    {
		Allocation* node = root;
		while(node->next)
		{
			uint32_t uinode = (uint32_t)node;
			uint32_t uinext= (uint32_t)node->next;
			uint32_t space = (uinext-uinode);
			space -= sizeof(Allocation);
			space -= node->size;
			if(space > sz)
			{
				Allocation* nnode = (Allocation*)((uint8_t*)(node)+sizeof(Allocation)+(node->size));
				nnode->next = node->next;
				node->next = nnode;
				nnode->size = sz;
				return (T*)(&nnode[1]);
			}
			node = node->next;
		}
		uint32_t uinode = (uint32_t)node;
		uint32_t uinext= (uint32_t)(root+pool_sz);
		uint32_t space = (uinext-uinode);
		space -= sizeof(Allocation);
		if(space > sz)
		{
			Allocation* nnode = (Allocation*)((uint8_t*)(node)+sizeof(Allocation)+(node->size));
			nnode->next = nullptr;
			node->next = nnode;
			nnode->size = sz;
			return (T*)(&nnode[1]);
		}
		return nullptr;
    }

    template <typename T> T* realloc(T* orig, uint32_t sz)
    {
		free(orig);
        T* nptr = malloc<T>(sz);
        if(orig == 0)
            return nptr;
        if((nptr) && (orig))
        {
            if(nptr != orig)
            {
                for(uint32_t i = 0; i < sz; i++)
                    nptr[i] = orig[i];
            }
            return nptr;
        }
        return 0;
    }

    template <typename T> void free(T* p)
    {
		Allocation* node = root;
		while(node->next)
		{

			if((T*)(node->next+1) == p)
			{
				node->next = node->next->next;
				return;
			}
			node = node->next;
		}
    }

private:
    struct Allocation
    {
        Allocation* next;
        uint32_t size;
    };

	Allocation* root;
    uint32_t pool_sz;
};

}


#endif

