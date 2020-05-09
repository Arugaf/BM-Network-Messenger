#include <iostream>

#include "HammingEncoder.hpp"
#include "HammingDecoder.hpp"

//TODO:: Decoder, transformator, кадры, контроль и обработка ошибок, управление соединением, интерфейсы, наследники

int main() {
    const unsigned char kek[] = {0xFF, 0xFF, 0xFF};
    BM_Network::HammingEncoder<const unsigned char> encode(kek, 3);
    encode.visualize();

    BM_Network::HammingDecoder<const unsigned char> decode(encode.getCodedBytes(), encode.getSize());
    decode.visualize();
    return 0;
}



