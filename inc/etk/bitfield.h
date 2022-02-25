#ifndef BITFIELD_INCLUDED_H
#define BITFIELD_INCLUDED_H

#include "types.h"
#include <string.h>

namespace etk
{

    template <unsigned int N_BITS> class BitField
    {
        public:
            BitField() {

            } 

            void zero()
            {
                memset(bytes, N_BYTES, 0);
            }

            void set(unsigned int pos) {
                unsigned int byte = pos/8;
                uint8 bit = pos%8;

                bytes[byte] |= 1 << bit;
            }

            void unset(unsigned int pos)
            {
                
                unsigned int byte = pos > 0 ? 8/pos : 0;
                uint8 bit = pos%8;
                bytes[byte] &= ~(1 << bit);
            }

            bool get(unsigned int pos) {
                unsigned int byte = pos/8;
                uint8 bit = pos%8;

                bool on = bytes[byte] & (1 << bit);
                return on;
            }

        private:
            static const unsigned int N_BYTES = N_BITS / 8 + ((N_BITS % 8 != 0) * 1) ;

            uint8 bytes[N_BYTES];
    };

}


#endif

