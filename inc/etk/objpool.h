#ifndef ETK_OBJECT_POOL_H
#define ETK_OBJECT_POOL_H

#include "types.h"
#include "array.h"
#include "list.h"

/*
 * This code is still very much under development.
 */

namespace etk
{

namespace experimental
{

template <typename P, typename T> class PoolPtr
{
public:
    PoolPtr()
    {
    }

    PoolPtr(P* pool) : pData(0), pool(pool)
    {
        if(pool)
            pool->inc_references(pData);
    }

    PoolPtr(P* pool, T* pValue) : pData(pValue), pool(pool)
    {
        if(pool)
        {
            pool->inc_references(pData);
        }
    }

    PoolPtr(const PoolPtr<P,T>& sp) : pData(sp.pData), pool(sp.pool)
    {
        if(pool)
            pool->inc_references(pData);
    }

    ~PoolPtr()
    {
        if(pool)
            pool->dec_references(pData);
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
            pool->dec_references(pData);
    }

    operator bool()
    {
        if(pData == nullptr)
            return false;
        return true;
    }

    PoolPtr<P,T>& operator = (const PoolPtr<P,T>& sp)
    {
        if (this != &sp)
        {
            if(pool)
                pool->dec_references(pData);
            pData = sp.pData;
            pool = sp.pool;
            if(pool)
                pool->inc_references(pData);
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

private:
    T* pData = 0;
    P* pool = 0;
};


template <typename T, uint32 N> class ObjPool
{
public:
    ObjPool()
    {

    }

    bool available()
    {
        for(auto& o : pool)
        {
            if(o.references == 0)
                return true;
        }
        return false;
    }

    int n_available()
    {
        int count = 0;
        for(auto& o : pool)
        {
            if(o.references == 0)
                count++;
        }
        return count;
    }

    PoolPtr<ObjPool, T> null_ptr()
    {
        return PoolPtr<ObjPool, T>(this);
    }

    PoolPtr<ObjPool, T> alloc()
    {
        for(auto& o : pool)
        {
            if(o.references == 0)
            {
                T* r = new((void*)o.data) T();
                return PoolPtr<ObjPool, T>(this, r);
            }
        }
        return PoolPtr<ObjPool, T>(this, nullptr);
    }

    void* raw_alloc()
    {
        for(auto& o : pool)
        {
            if(o.references == 0)
            {
                o.references = 1;
                return o.data;
            }
        }
        return nullptr;
    }

    void free(T*& ptr)
    {
        for(auto& o : pool)
        {
            if((&o.obj) == ptr)
            {
                o.references = 0;
                ptr = nullptr;
                return;
            }
        }
    }

private:
    friend PoolPtr<ObjPool, T>;

    void inc_references(T* ptr)
    {
        for(auto& o : pool)
        {
            if(&o.data == (void*)ptr)
            {
                o.references++;
                return;
            }
        }
    }

    void dec_references(T* ptr)
    {
        for(auto& o : pool)
        {
            if(&o.data == (void*)ptr)
            {
                if(o.references > 0)
                {
                    o.references--;
                    if(o.references == 0)
                        ptr->~T();
                }
                return;
            }
        }
    }

    class tbl
    {
    public:
        uint32 references = 0;
        uint8 data[sizeof(T)];
    };

    Array<tbl, N> pool;
};

}

}


#endif // ETK_OBJECT_POOL_H
