//
// Created by arugaf on 09.05.2020.
//

#include "BitIO.h"

void BM_Network::BitIO::writeBit(BM_Network::byte* bytes, BM_Network::byte bit, size_t index) {
    if (readBit(&bit, 0)) {
        bytes[index / 8] |= 1u << (index % 8);
    } else {
        bytes[index / 8] &= ~(1u << (index % 8));
    }
}

BM_Network::byte BM_Network::BitIO::readBit(const BM_Network::byte* bytes, BM_Network::size_t index) {
    return (static_cast<unsigned short>(bytes[index / 8] >> (index % 8)) & 1u);
}
