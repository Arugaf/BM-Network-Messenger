//
// Created by arugaf on 08.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_HAMMINGENCODER_HPP
#define BM_NETWORK_MESSENGER_HAMMINGENCODER_HPP

#include "utils.h"

#include "BitIO.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <type_traits>

namespace BM_Network {
    template<typename data_type,
             unsigned int R = 3,
             typename restriction = typename std::enable_if<std::is_fundamental<data_type>::value>::type>
    class HammingEncoder {
    public:
        HammingEncoder(const data_type* data, size_t data_size);
        ~HammingEncoder();

        [[nodiscard]] const byte* getCodedBytes() const;
        [[nodiscard]] size_t getSize() const;

        void visualize() const;

        void spoilByte(size_t bit_index);
    private:
        byte* encoded_bytes;

        size_t size;
        size_t bits;
        short overhead;

        constexpr static unsigned int length = (1u << R) - 1;
        constexpr static float multiplier = static_cast<float>(length) / (length - R);
    };

    template<typename data_type, unsigned int R, typename restriction>
    HammingEncoder<data_type, R, restriction>::HammingEncoder(const data_type* data, const size_t data_size) :
                            bits(0),
                            overhead(static_cast<short>(data_size * sizeof(data_type) * 8 * multiplier) % 8),
                            size(data_size * sizeof(data_type) * multiplier) {
        overhead ? size++ : size;
        encoded_bytes = new byte[size + 1];
        std::fill(encoded_bytes, encoded_bytes + size, 0);

        auto data_bytes = reinterpret_cast<const byte*>(data);
        auto data_bytes_size = data_size * sizeof(data_type);

        for (size_t i = 0; i < data_bytes_size; ++i) {
            byte byte_to_code = data_bytes[i];
            for (size_t j = 0; j < 8; ++j, ++bits) {
                while (!((bits % length + 1u) & bits % length)) {
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

        encoded_bytes[size] = 0;
    }

    template<typename data_type, unsigned int R, typename restriction>
    HammingEncoder<data_type, R, restriction>::~HammingEncoder() {
        delete[] encoded_bytes;
    }

    template<typename data_type, unsigned int R, typename restriction>
    const byte* HammingEncoder<data_type, R, restriction>::getCodedBytes() const {
        return encoded_bytes;
    }

    template<typename data_type, unsigned int R, typename restriction>
    size_t HammingEncoder<data_type, R, restriction>::getSize() const {
        return size;
    }

    template<typename data_type, unsigned int R, typename restriction>
    void HammingEncoder<data_type, R, restriction>::visualize() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << static_cast<data_type>(encoded_bytes[i]) << ' ' << std::bitset<8>(encoded_bytes[i]) << std::endl;
        }
    }

    template<typename data_type, unsigned int R, typename restriction>
    void HammingEncoder<data_type, R, restriction>::spoilByte(size_t bit_index) {
        if (bit_index < bits) {
            BitIO::writeBit(encoded_bytes, ~(BitIO::readBit(encoded_bytes, bit_index)), bit_index);
        }
    }
}

#endif //BM_NETWORK_MESSENGER_HAMMINGENCODER_HPP
