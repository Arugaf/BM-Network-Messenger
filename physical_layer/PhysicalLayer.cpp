//
// Created by vladimir on 10.05.20.
//

#include <thread>
#include "PhysicalLayer.h"

void PhysicalLayer::bindDatalinkLayer(IDatalink *receiver) {
    this->datalinkLayer = receiver;
    DCB
}

void PhysicalLayer::read() {
    try {
        while (true) {
            if (serial_port_read.IsOpen()) {
                std::getline(serial_port_read, buffer_read);
                datalinkLayer->sendData(buffer_read.c_str());
            } else {
                datalinkLayer->connLostCallback(PortType(READ));
            }
        }
    }
    catch (std::ifstream::failure &failure) {
        std::cerr << "Exception happened: " << failure.what() << "\n"
                  << "Error bits are: "
                  << "\nfailbit: " << serial_port_read.fail()
                  << "\neofbit: " << serial_port_read.eof()
                  << "\nbadbit: " << serial_port_read.bad() << std::endl;
        datalinkLayer->connLostCallback(PortType(READ));
    }
}

PhysicalLayer::~PhysicalLayer() {
    serial_port_write.Close();
    serial_port_read.Close();
}

void PhysicalLayer::sendData(const PhysicalLayer::byte *data) {
    try {
        if (serial_port_write.IsOpen()) {
            serial_port_write << data;
        } else {
            datalinkLayer->connLostCallback(PortType(WRITE));
        }
    }

    catch (std::ofstream::failure &failure) {
        std::cerr << "Exception happened: " << failure.what() << "\n"
                  << "Error bits are: "
                  << "\nfailbit: " << serial_port_write.fail()
                  << "\neofbit: " << serial_port_write.eof()
                  << "\nbadbit: " << serial_port_write.bad() << std::endl;
        datalinkLayer->connLostCallback(PortType(WRITE));
    }
}

OpenPortCallbackMessage PhysicalLayer::openPort(std::string portName, const PortType &portType) {
    try {
        switch (portType) {
            case PortType::READ: {
                serial_port_read.Open(portName);

                //Start listening port, blocking current thread
                this->physical_layer_thread=new std::thread(&PhysicalLayer::read, this);
            }
            case PortType::WRITE: {
                serial_port_write.Open(portName);
            }
        }
    }
    catch (SerialPort::OpenFailed &error) {
        std::cerr << "The serial ports did not open correctly.\n" << error.what() << std::endl;
        return OpenPortCallbackMessage(OpenFailed);
    }
    catch (SerialPort::AlreadyOpen &error) {
        std::cerr << "The serial is already used.\n" << error.what() << std::endl;
        return OpenPortCallbackMessage(AlreadyOpen);

    }
    catch (SerialPort::NotOpen &error) {
        std::cerr << "The serial ports did not open correctly.\n" << error.what() << std::endl;
        return OpenPortCallbackMessage(OpenFailed);
    }
}

void PhysicalLayer::waitMainThread() {
    this->physical_layer_thread->join();
}
