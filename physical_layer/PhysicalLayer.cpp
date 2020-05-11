//
// Created by vladimir on 10.05.20.
//

#include "PhysicalLayer.h"

void PhysicalLayer::sendData(const PhysicalLayer::byte *data) {
    serial_port_write << data;
}

void PhysicalLayer::bindDataLinkLayer(BM_Network::IDataReceiver *receiver) {
    this->datalink_controller = receiver;
}

PhysicalLayer::PhysicalLayer() {
    // Open the Serial Ports at the desired hardware devices.
    try {
        serial_port_read.Open(SERIAL_PORT_1);
        serial_port_write.Open(SERIAL_PORT_2);
    }

    catch (SerialPort::OpenFailed &error) {
        std::cerr << "The serial ports did not open correctly.\n" << error.what() << std::endl;
    }

    std::thread physical_layer_thread(&PhysicalLayer::read);
}

void PhysicalLayer::read() {
    while(true){
        std::getline(serial_port_read, buffer_read);

        datalink_controller->sendData(buffer_read.c_str());
    }
}

PhysicalLayer::~PhysicalLayer() {
    serial_port_write.Close();
    serial_port_read.Close();
}
