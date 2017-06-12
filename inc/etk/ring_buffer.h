/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
*/

#ifndef RING_BUF_H
#define RING_BUF_H

#include "types.h"

namespace etk
{

/**
 * \class RingBuffer
 *
 * \brief RingBuffers are a great way to buffer information from data streams such as UARTs.
 * RingBuffer does not allocate it's own memory, instead it requires a pointer to an array or memory region where it can store data.
 * <br><br>
 * Example
 * @code
 char usart_buffer[128];
 etk::RingBuffer<char> ringbuf(usart_buffer, 128);

 void usart_isr_hander()
 {
     ringbuf.put(USART_READ_BYTE());
 }

 void foo()
 {
     while(ringbuf.available())
         Usart.put(ringbuf.get());
 }
 @endcode
 * @tparam T type of object that is being buffered. For a UART this would typically be a char or uint8.
 * @tparam overwrite If overriden to true, the ring buffer will write over data when it becomes full.
 */


template <class T, bool overwrite = false> class RingBuffer
{
public:
    /**
     * \brief The constructor.
     * @arg buffer Pointer to a writeable memory location.
     * @arg sz Maximum number of items that can be stored before the buffer is full.
     */
    RingBuffer(T* buffer, uint16 sz)
    {
        size = sz;
        start = 0;
        end = 0;
        buf = buffer;
    }

    /**
     * \brief Returns true when the RingBuffer is full.
     */
    bool is_full()
    {
        return (end + 1) % size == start;
    }

    /**
     * \brief Returns the number of items queued up in the RingBuffer.
     */
    uint16 available()
    {
        return (uint16)(size + end - start) % size;
    }

    /**
     * \brief Puts an item on to the RingBuffer.
     */
    void put(T b)
    {
        if(!overwrite)
        {
            if(is_full())
                return;
        }
        buf[end] = b;
        end = (end + 1) % size;
    }

    /**
     * \brief Moves the end of the ring buffer along by one position.
     */
    void increment()
    {
        end = (end + 1) % size;
    }

    /**
     * \brief Removes the next item from the buffer and returns it.
     */
    T get()
    {
        T ret = buf[start];
        start = (start + 1) % size;
        return ret;
    }

    /**
     * \brief Returns the next item from the buffer without actually removing it.
     */
    T peek_ahead(uint16 n=0)
    {
        uint16 pos = (start+n) % size;
        return buf[pos];
    }

    /**
     * \brief Makes the start and end of the ring buffer equal zero so that available() return zero.
     */
    void empty()
    {
        start = 0;
        end = 0;
    }

private:
    uint16 size;
    uint16 start;
    uint16 end;
    T* buf;
};

}

#endif // RING_BUF_H
