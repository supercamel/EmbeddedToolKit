#ifndef ETK_OBJECT_POOL_H
#define ETK_OBJECT_POOL_H

#include <stdint.h>
#include <etk/array.h>

namespace etk
{

template <typename P, typename T> class PoolPtr
{
public:
    PoolPtr(P* pool) : pData(0), pool(pool)
    {
        pool->inc_references(pData);
    }

    PoolPtr(P* pool, T* pValue) : pData(pValue), pool(pool)
    {
        pool->inc_references(pData);
    }

    PoolPtr(const PoolPtr<P,T>& sp) : pData(sp.pData), pool(sp.pool)
    {
        pool->inc_references(pData);
    }

    ~PoolPtr()
    {
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

    PoolPtr<P,T>& operator = (const PoolPtr<P,T>& sp)
    {
        if (this != &sp)
            pData = sp.pData;

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
    T* pData;
    P* pool;
};


template <typename T, uint32_t N> class ObjPool
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

    auto alloc()
    {
        for(auto& o : pool)
        {
            if(o.references == 0)
            {
                PoolPtr<ObjPool, T> ptr(this, &o.obj);
                return ptr;
            }
        }

        return PoolPtr<ObjPool, T>(this);
    }

    T* raw_alloc()
    {
        for(auto& o : pool)
        {
            if(o.references == 0)
            {
                o.references = 1;
                return &o.obj;
            }
        }
    }

    void free(T* ptr)
    {
        for(auto& o : pool)
        {
            if((&o.obj) == ptr)
            {
                o.references = 0;
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
            if(&o.obj == ptr)
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
            if(&o.obj == ptr)
            {
                if(o.references > 0)
                    o.references--;
                return;
            }
        }
    }

    class tbl
    {
    public:
        uint32_t references = 0;
        T obj;
    };

    Array<tbl, N> pool;
};


}


#endif // ETK_OBJECT_POOL_H
