/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RING_BUF_H
#define RING_BUF_H

#include <stdint.h>

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
 * @tparam T type of object that is being buffered. For a UART this would typically be a char or uint8_t.
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
    RingBuffer(T* buffer, uint16_t sz)
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
    uint16_t available()
    {
        return (uint16_t)(size + end - start) % size;
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
    T peek_ahead(uint16_t n=0)
    {
        uint16_t pos = (start+n) % size;
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
    uint16_t size;
    uint16_t start;
    uint16_t end;
    T* buf;
};

}

#endif // RING_BUF_H
