//
// Created by vladimir on 13.05.20.
//

#ifndef BM_NETWORK_MESSENGER_IDATALINK_H
#define BM_NETWORK_MESSENGER_IDATALINK_H

#include "../../Utils.h"

class IDatalink{
    typedef char byte;

public:
    virtual void sendData(const byte* data) = 0;

    virtual void connLostCallback(const PortType& portType) = 0;
};

#endif //BM_NETWORK_MESSENGER_IDATALINK_H
