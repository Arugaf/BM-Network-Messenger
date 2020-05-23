#ifndef BM_NETWORK_MESSENGER_IMESSAGERECEIVER_H
#define BM_NETWORK_MESSENGER_IMESSAGERECEIVER_H

#include "utils.h"

#include <iostream>

namespace BM_Network {
    class IMessageReceiver {
    public:
        virtual void
        sendMessage(const std::string& user_name, const std::string& message) = 0;

        virtual ~IMessageReceiver() = default;
    };
}

#endif //BM_NETWORK_MESSENGER_IMESSAGERECEIVER_H
