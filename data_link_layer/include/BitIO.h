//
// Created by arugaf on 09.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_BITIO_H
#define BM_NETWORK_MESSENGER_BITIO_H

namespace BM_Network {
    typedef unsigned long size_t;
    typedef unsigned char byte;

    class BitIO {
    public:
        static void writeBit(byte* bytes, unsigned char bit, size_t index);
        static byte readBit(const byte* bytes, size_t index);
    };
}

#endif //BM_NETWORK_MESSENGER_BITIO_H
