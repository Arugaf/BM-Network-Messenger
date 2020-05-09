//
// Created by arugaf on 09.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_HAMMINGDECODER_H
#define BM_NETWORK_MESSENGER_HAMMINGDECODER_H

#include "BitIO.h"
#include "HammingErrorController3.h"

#include <bitset>
#include <iostream>
#include <type_traits>

namespace BM_Network {
    typedef unsigned long size_t;
    typedef unsigned char byte;

    template<typename T, unsigned int R = 3,
             typename restriction = typename std::enable_if<std::is_fundamental<T>::value>::type,
             typename error_controller = HammingErrorController3>
    class HammingDecoder {
    public:
        HammingDecoder(const byte* data, size_t data_size);
        ~HammingDecoder();

        T* getDecodedBytes();
        void visualize();
    private:
        byte* decoded_bytes;

        size_t size;
        size_t bits;

        constexpr static unsigned int length = (1u << R) - 1;
        constexpr static float multiplier = static_cast<float>(length) / (length - R);
    };

    template<typename T, unsigned int R, typename restriction, typename ec>
    HammingDecoder<T, R, restriction, ec>::HammingDecoder(const byte* data, size_t data_size) :
                                                                        bits(data_size * 8 - data_size * 8 % length),
                                                                        size(data_size / multiplier / sizeof(T)) {
        decoded_bytes = new byte[size];

        byte check_byte;
        for (size_t current_bit = 0, i = 0; current_bit < bits; ++i) {
            while (!((current_bit % length + 1) & current_bit % length)) {
                BitIO::writeBit(&check_byte, BitIO::readBit(data, current_bit), current_bit % length);
                ++current_bit;
            }
            BitIO::writeBit(decoded_bytes, BitIO::readBit(data, current_bit), i);
            BitIO::writeBit(&check_byte, BitIO::readBit(data, current_bit), current_bit % length);
            ++current_bit;

            if (!(current_bit % length)) {
                if (HammingErrorController3::checkErrors(check_byte)) {
                    std::cout << "Error!" << std::endl;
                }
                check_byte = 0;
            }
        }
    }

    template<typename T, unsigned int R, typename restriction, typename ec>
    HammingDecoder<T, R, restriction, ec>::~HammingDecoder() {
        delete[] decoded_bytes;
    }

    template<typename T, unsigned int R, typename restriction, typename ec>
    void HammingDecoder<T, R, restriction, ec>::visualize() {
        for (size_t i = 0; i < size; ++i) {
            std::cout << static_cast<long long>(reinterpret_cast<T*>(decoded_bytes)[i]) <<
                         ' ' <<
                         std::bitset<8 * sizeof(T)>(reinterpret_cast<T*>(decoded_bytes)[i]) <<
                         std::endl;
        }
    }

    template<typename T, unsigned int R, typename restriction, typename ec>
    T* HammingDecoder<T, R, restriction, ec>::getDecodedBytes() {
        return reinterpret_cast<T*>(decoded_bytes);
    }
}

#endif //BM_NETWORK_MESSENGER_HAMMINGDECODER_H
