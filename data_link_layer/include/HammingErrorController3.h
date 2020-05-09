//
// Created by arugaf on 09.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_HAMMINGERRORCONTROLLER3_H
#define BM_NETWORK_MESSENGER_HAMMINGERRORCONTROLLER3_H

#include <bitset>

namespace BM_Network {
    typedef unsigned char byte;

    class HammingErrorController3 {
    public:
        static bool checkErrors(byte check_byte);
    };
}

#endif //BM_NETWORK_MESSENGER_HAMMINGERRORCONTROLLER3_H
