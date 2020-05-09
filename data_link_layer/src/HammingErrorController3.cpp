//
// Created by arugaf on 09.05.2020.
//

#include <HammingErrorController3.h>

bool BM_Network::HammingErrorController3::checkErrors(byte check_byte) {
    std::bitset<7>check_bits(check_byte);
    auto first = check_bits[0] ^ check_bits[2] ^ check_bits[4] ^ check_bits[6];
    auto second = check_bits[1] ^ check_bits[2] ^ check_bits[5] ^ check_bits[6];
    auto third = check_bits[3] ^ check_bits[4] ^ check_bits[5] ^ check_bits[6];
    return first || second || third;
}
