//
// Created by vladimir on 10.05.20.
//

#ifndef BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
#define BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H

#include "IPhysical.h"
#include <IDatalink.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

class PhysicalLayer: virtual public IPhysical{
    typedef char byte;

public:

    explicit PhysicalLayer() = default;

    ~PhysicalLayer();

    void bindDatalinkLayer(IDatalink *receiver = nullptr);

    //here we receive data from DataLinkLayer
    void sendData(const byte *) override;

    OpenPortCallbackMessage openPort(std::string portName, const PortType &portType) override;

    void waitMainThread();

private:
    IDatalink *datalinkLayer = nullptr;

    LibSerial::SerialStream serial_port_read;
    LibSerial::SerialStream serial_port_write;

    std::string buffer_read = "";

    std::thread *physical_layer_thread;

    void read();
};


#endif //BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
