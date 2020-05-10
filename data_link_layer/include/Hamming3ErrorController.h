//
// Created by arugaf on 09.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_HAMMING3ERRORCONTROLLER_H
#define BM_NETWORK_MESSENGER_HAMMING3ERRORCONTROLLER_H

#include <bitset>
#include <iostream>

namespace BM_Network {
    typedef char byte;

    class Mediator; // TODO: Передавать mediator в конструктор

    template<typename T>
    class ErrorController {
    public:
        ErrorController() = default;
        [[nodiscard]] virtual bool checkErrors(T) const = 0;
        virtual void notify() const = 0;
        virtual ~ErrorController() = default;
    private:
        Mediator* mediator;
    };

    class Hamming3ErrorController : public ErrorController<byte> {
    public:
        Hamming3ErrorController() = default;
        [[nodiscard]] bool checkErrors(byte check_byte) const override;
        void notify() const override;
        ~Hamming3ErrorController() override = default;
    };
}

#endif //BM_NETWORK_MESSENGER_HAMMING3ERRORCONTROLLER_H
