//
// Created by arugaf on 09.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_HAMMING3ERRORCONTROLLER_H
#define BM_NETWORK_MESSENGER_HAMMING3ERRORCONTROLLER_H

#include "include/utils.h"

#include <bitset>
#include <iostream>

namespace BM_Network {
    template<typename T>
    class ErrorController {
    public:
        ErrorController() = default;
        [[nodiscard]] virtual bool checkErrors(T) const = 0;
        virtual ~ErrorController() = default;
    };

    class Hamming3ErrorController : public ErrorController<byte> {
    public:
        Hamming3ErrorController() = default;
        [[nodiscard]] bool checkErrors(byte check_byte) const override;
        ~Hamming3ErrorController() override = default;
    };
}

#endif //BM_NETWORK_MESSENGER_HAMMING3ERRORCONTROLLER_H
