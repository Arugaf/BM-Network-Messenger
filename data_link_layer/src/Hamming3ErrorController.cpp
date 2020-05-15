//
// Created by arugaf on 10.05.2020.
//

#include "Hamming3ErrorController.h"

bool BM_Network::Hamming3ErrorController::checkErrors(BM_Network::byte check_byte) const {
    std::bitset<7> check_bits(check_byte);
    auto first = check_bits[0] ^ check_bits[2] ^ check_bits[4] ^ check_bits[6];
    auto second = check_bits[1] ^ check_bits[2] ^ check_bits[5] ^ check_bits[6];
    auto third = check_bits[3] ^ check_bits[4] ^ check_bits[5] ^ check_bits[6];
    return first || second || third;
}
