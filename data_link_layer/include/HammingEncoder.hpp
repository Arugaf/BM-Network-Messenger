//
// Created by arugaf on 08.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_HAMMINGENCODER_HPP
#define BM_NETWORK_MESSENGER_HAMMINGENCODER_HPP

#include "BitIO.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <type_traits>

namespace BM_Network {
    typedef unsigned long size_t;
    typedef unsigned char byte;

    template<typename T, unsigned int R = 3,
             typename restriction = typename std::enable_if<std::is_fundamental<T>::value>::type>
    class HammingEncoder {
    public:
        HammingEncoder(const T* data, int data_size);
        ~HammingEncoder();

        byte* getCodedBytes();
        auto getSize();
        void visualize();

        void spoilByte(size_t bit);
    private:
        byte* encoded_bytes;

        size_t size;
        size_t bits;
        short overhead;

        constexpr static unsigned int length = (1u << R) - 1;
        constexpr static float multiplier = static_cast<float>(length) / (length - R);
    };

    template<typename T, unsigned int R, typename restriction>
    HammingEncoder<T, R, restriction>::HammingEncoder(const T* data, const int data_size) :
                            bits(0),
                            overhead(static_cast<short>(data_size * sizeof(T) * 8 * multiplier) % 8),
                            size(data_size * sizeof(T) * multiplier) {
        overhead ? size++ : size;
        encoded_bytes = new byte[size];
        std::fill(encoded_bytes, encoded_bytes + size, 0);

        auto data_bytes = reinterpret_cast<const byte*>(data);
        auto data_bytes_size = data_size * sizeof(T);

        for (size_t i = 0; i < data_bytes_size; ++i) {
            byte byte_to_code = data_bytes[i];
            for (size_t j = 0; j < 8; ++j, ++bits) {
                while (!((bits % length + 1) & bits % length)) {
                    BitIO::writeBit(encoded_bytes, 0, bits++);
                }
                BitIO::writeBit(encoded_bytes, static_cast<byte>(byte_to_code >> static_cast<unsigned short>(j)) &
                                                                                                            1u, bits);
            }
        }

        for (size_t current_bit = 0; current_bit < bits; current_bit += length) {
            BitIO::writeBit(encoded_bytes, BitIO::readBit(encoded_bytes, current_bit + 2) ^
                                               BitIO::readBit(encoded_bytes, current_bit + 4) ^
                                               BitIO::readBit(encoded_bytes, current_bit + 6), current_bit);

            BitIO::writeBit(encoded_bytes, BitIO::readBit(encoded_bytes, current_bit + 2) ^
                                               BitIO::readBit(encoded_bytes, current_bit + 5) ^
                                               BitIO::readBit(encoded_bytes, current_bit + 6), current_bit + 1);

            BitIO::writeBit(encoded_bytes, BitIO::readBit(encoded_bytes, current_bit + 4) ^
                                               BitIO::readBit(encoded_bytes, current_bit + 5) ^
                                               BitIO::readBit(encoded_bytes, current_bit + 6), current_bit + 3);
        }
    }

    template<typename T, unsigned int R, typename restriction>
    HammingEncoder<T, R, restriction>::~HammingEncoder() {
        delete[] encoded_bytes;
    }

    template<typename T, unsigned int R, typename restriction>
    byte* HammingEncoder<T, R, restriction>::getCodedBytes() {
        return encoded_bytes;
    }

    template<typename T, unsigned int R, typename restriction>
    auto HammingEncoder<T, R, restriction>::getSize() {
        return size;
    }

    template<typename T, unsigned int R, typename restriction>
    void HammingEncoder<T, R, restriction>::visualize() {
        for (size_t i = 0; i < size; ++i) {
            std::cout << static_cast<int>(encoded_bytes[i]) << ' ' << std::bitset<8>(encoded_bytes[i]) << std::endl;
        }
    }

    template<typename T, unsigned int R, typename restriction>
    void HammingEncoder<T, R, restriction>::spoilByte(size_t bit) {
        if (bit < bits) {
            BitIO::writeBit(encoded_bytes, ~(BitIO::readBit(encoded_bytes, bit)), bit);
        }
    }
}

#endif //BM_NETWORK_MESSENGER_HAMMINGENCODER_HPP
