#ifndef BM_NETWORK_MESSENGER_IDATARECEIVER_H
#define BM_NETWORK_MESSENGER_IDATARECEIVER_H

#include "utils.h"

namespace BM_Network {
    class IDataReceiver {
    public:
        virtual void sendData(const byte* data) = 0;

        virtual ~IDataReceiver() = default;
    };
}

#endif //BM_NETWORK_MESSENGER_IDATARECEIVER_H
