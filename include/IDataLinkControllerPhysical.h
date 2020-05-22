#ifndef BM_NETWORK_MESSENGER_IDATALINKCONTROLLERPHYSICAL_H
#define BM_NETWORK_MESSENGER_IDATALINKCONTROLLERPHYSICAL_H

#include "IDataReceiver.h"
#include "utils.h"

namespace BM_Network {
    class IDataLinkControllerPhysical : virtual public IDataReceiver {
    public:
        void sendData(const byte *data, size_t size) override = 0;
        virtual void connLostCallback(const PortType& portType) = 0;
        ~IDataLinkControllerPhysical() override = default;
    };
}

#endif //BM_NETWORK_MESSENGER_IDATALINKCONTROLLERPHYSICAL_H
