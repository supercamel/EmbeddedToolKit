#ifndef ETK_STACK_H
#define ETK_STACK_H


#include "pool.h"

namespace etk
{

template <typename T> class Stack
{
public:
	Stack(MemPool& pool) : pool(pool)
	{
		pbottom = pool.malloc<T>(8);
		ptop = pbottom+8;
	}

	void push(T t)
	{
		pbottom[stack_pos++] = t;
		if(&pbottom[stack_pos] >= ptop)
		{
			pbottom = pool.realloc<T>(pbottom, stack_pos+8);
			ptop = pbottom+(stack_pos+8);
		}
	}

	T peek()
	{
		return pbottom[stack_pos];
	}

	T pop()
	{
		return pbottom[--stack_pos];
	}

	void popn(uint32_t n)
	{
		stack_pos -= n;
	}

private:
	T* pbottom;
	T* ptop;
	uint32_t stack_pos = 0;

	MemPool& pool;
};


};


#endif

