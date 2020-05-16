//
// Created by vladimir on 10.05.20.
//

#ifndef BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
#define BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H

#include "IPhysical.h"
#include "../data_link_layer/include/IDatalink.h"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <thread>

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

    OpenPortCallbackMessage openPort(const char* portName, const PortType &portType) override;

//    void waitMainThread();

private:
    IDatalink *datalinkLayer = nullptr;

    int portNumberRead;
    int portNumberWrite;

//    std::thread *physical_layer_thread;
    void read();
};


#endif //BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
