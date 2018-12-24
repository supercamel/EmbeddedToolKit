#include "dynamic_list_test.h"


#include <etk/pool.h>


#include <iostream>
using namespace std;

namespace etk
{

namespace experimental
{

template <typename T, uint32 REALLOC_SZ = 5> class DynamicList
{
public:
    DynamicList(Pool& pool) : pool(pool)
    {
    }

    void append(const T& t)
    {
        if(sz >= alloc_sz)
        {
            list = static_cast<T*>(pool.realloc(static_cast<void*>(list), (alloc_sz+REALLOC_SZ)*sizeof(T)));
            alloc_sz += REALLOC_SZ;
        }

        list[sz] = t;
        sz++;
    }

    void remove(uint32 index)
    {
        if((index < sz) && (index != 0))
        {
            (&list[index])->~T();
            for(uint32 i = index; i < sz-1; i++)
                list[i] = list[i+1];
            sz--;

            if(alloc_sz-sz > REALLOC_SZ)
            {
                list = static_cast<T*>(pool.realloc(static_cast<void*>(list), sz*sizeof(T)));
            }
        }
    }

    T pop_back()
    {
        T t = list[sz-1];
        sz--;
        return t;
    }

    void push_back(const T& t)
    {
        append(t);
    }


    T& operator[](const uint32 pos) const
    {
        return list[pos];
    }

    const T* buffer() const
    {
        return list;
    }

    T* raw_memory() const
    {
        return list;
    }


private:
    T* list = nullptr;
    Pool& pool;
    uint32 sz = 0; //number of items in dynamic list
    uint32 alloc_sz = 0; //memory has been allocated for this many items
};


}

}



#include <iostream>
using namespace std;

using namespace etk;
using namespace etk::experimental;

bool dynamic_list_test(std::string& subtest)
{
    MemPool<2048> pool;
    DynamicList<int> list(pool);

    for(int i = 0; i < 20; i++)
    {
        list.append(i);
    }

    list.remove(14);

    /*
    for(auto i : range(20)) {
        cout << list[i] << endl;
    }
    */

    return true;
}


