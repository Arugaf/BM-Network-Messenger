//
// Created by vladimir on 10.05.20.
//

#ifndef BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
#define BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H

#include <Controller.h>
#include <SerialStream.h>

#include <iostream>
#include <thread>
#include <SerialPort.h>


constexpr const char *const SERIAL_PORT_1 = "COM1";
constexpr const char *const SERIAL_PORT_2 = "COM2";

class PhysicalLayer : virtual public BM_Network::IDataReceiver {
    typedef char byte;

public:
    explicit PhysicalLayer();

    ~PhysicalLayer() override;

    void bindDataLinkLayer(BM_Network::IDataReceiver *receiver = nullptr);

    //here we receive data from DataLinkLayer
    void sendData(const byte *) override;

private:
    BM_Network::IDataReceiver *datalink_controller;

    LibSerial::SerialStream serial_port_read;
    LibSerial::SerialStream serial_port_write;

    std::string buffer_read="";

    void read();
};


#endif //BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
