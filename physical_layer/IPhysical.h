//
// Created by vladimir on 13.05.20.
//

#ifndef BM_NETWORK_MESSENGER_IPHYSICAL_H
#define BM_NETWORK_MESSENGER_IPHYSICAL_H

#include <string>
#include "utils.h"

namespace BM_Network {
    class IPhysical {
        typedef char byte;
    public:
        virtual void sendData(const byte *data) = 0;

        virtual OpenPortCallbackMessage openPort(const char *portName, const PortType &portType) = 0;
    };
}
#endif //BM_NETWORK_MESSENGER_IPHYSICAL_H
