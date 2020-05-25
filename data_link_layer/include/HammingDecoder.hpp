//
// Created by arugaf on 09.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_HAMMINGDECODER_H
#define BM_NETWORK_MESSENGER_HAMMINGDECODER_H

#include "include/utils.h"

#include "BitIO.h"
#include "Hamming3ErrorController.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <type_traits>

namespace BM_Network {
    template<typename T, unsigned int R = 3, typename error_controller = Hamming3ErrorController,
             typename restriction = typename std::enable_if<std::is_fundamental<T>::value>::type>
    class HammingDecoder {
    public:
        explicit HammingDecoder(const byte* data, byte stop_byte = 0xFF);
        ~HammingDecoder();

        [[nodiscard]] const T* getDecodedBytes() const;
        [[nodiscard]] size_t getSize() const;
        [[nodiscard]] bool isError() const;

        void visualize() const;
    private:
        T* decoded_bytes;

        size_t size;
        size_t bits;

        bool error;

        constexpr static unsigned int length = (1u << R) - 1;
    };

    template<typename T, unsigned int R, typename error_controller, typename restriction>
    HammingDecoder<T, R, error_controller, restriction>::HammingDecoder(const byte* data, byte stop_byte) : bits(0),
                                                                                                            error(false) {
        std::string decoded_bytes_string;

        byte check_byte = 0;
        byte byte_to_write = 0;

        ErrorController<byte>* error_controller_impl = new Hamming3ErrorController;

        for (size_t i = 0; byte_to_write != stop_byte || i <= 8; ++i) {
            if (!(i % 8) && i) {
                decoded_bytes_string.push_back(byte_to_write);
                byte_to_write = 0;
            }

            while (!((bits % length + 1) & bits % length)) {
                BitIO::writeBit(&check_byte, BitIO::readBit(data, bits), bits % length);
                ++bits;
            }
            BitIO::writeBit(&byte_to_write, BitIO::readBit(data, bits), i % 8);
            BitIO::writeBit(&check_byte, BitIO::readBit(data, bits), bits % length);
            ++bits;

            if (!(bits % length)) {
                error = error || error_controller_impl->checkErrors(check_byte);
                check_byte = 0;
            }
        }

        decoded_bytes_string.push_back(byte_to_write);

        size = decoded_bytes_string.size() / sizeof(T);
        decoded_bytes_string.size() % sizeof(T) ? ++size : size;

        auto decoded_bytes_b = new byte[decoded_bytes_string.size() + 1];
        std::copy(decoded_bytes_string.begin(), decoded_bytes_string.end(), decoded_bytes_b);
        decoded_bytes_b[decoded_bytes_string.size()] = 0;

        decoded_bytes = reinterpret_cast<T*>(decoded_bytes_b);

        delete error_controller_impl;
    }

    template<typename T, unsigned int R, typename error_controller, typename restriction>
    HammingDecoder<T, R, error_controller, restriction>::~HammingDecoder() {
        delete[] decoded_bytes;
    }

    template<typename T, unsigned int R, typename error_controller, typename restriction>
    void HammingDecoder<T, R, error_controller, restriction>::visualize() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << reinterpret_cast<T*>(decoded_bytes)[i] <<
                         ' ' <<
                         std::bitset<8 * sizeof(T)>(reinterpret_cast<T*>(decoded_bytes)[i]) <<
                         std::endl;
        }
    }

    template<typename T, unsigned int R, typename error_controller, typename restriction>
    const T* HammingDecoder<T, R, error_controller, restriction>::getDecodedBytes() const {
        return decoded_bytes;
    }

    template<typename T, unsigned int R, typename error_controller, typename restriction>
    size_t HammingDecoder<T, R, error_controller, restriction>::getSize() const {
        return size;
    }

    template<typename T, unsigned int R, typename error_controller, typename restriction>
    bool HammingDecoder<T, R, error_controller, restriction>::isError() const {
        return error;
    }
}

#endif //BM_NETWORK_MESSENGER_HAMMINGDECODER_H
