//
// Created by vova_ on 21.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_DATALINKCONTROLLERPL_H
#define BM_NETWORK_MESSENGER_DATALINKCONTROLLERPL_H

#include <memory>

#include "include/IDataLinkControllerPhysical.h"

namespace BM_Network {
    class DatalinkControllerPL {
    public:
        explicit DatalinkControllerPL();
        void sendData(const byte *data, size_t size);
        virtual void connLostCallback(const PortType& portType);
        void injectImpl(const std::shared_ptr<IDataLinkControllerPhysical>& impl);
    private:
        std::shared_ptr<IDataLinkControllerPhysical> datalink_controller_impl;
    };
}



#endif //BM_NETWORK_MESSENGER_DATALINKCONTROLLERPL_H
