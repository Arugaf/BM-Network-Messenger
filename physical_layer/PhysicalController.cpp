//
// Created by vladimir on 10.05.20.
//

#include "PhysicalController.h"
#include "DatalinkControllerPL.h"
#include "include/utils.h"

BM_Network::PhysicalController::PhysicalController(BM_Network::DatalinkControllerPL &dl_cl) : datalink_controller(dl_cl) {

}

void BM_Network::PhysicalController::read() {
    unsigned char buf[4096];
    while (true) {
        if (RS232_IsDCDEnabled(portNumberRead) == 0) {
            datalink_controller.connLostCallback(PortType(Read));
            return;
        }
        int n = RS232_PollComport(portNumberRead, buf, 4095);
        if (n > 0) {
            buf[n] = 0;
            datalink_controller.sendData((char *) buf, n);
        }
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);  /* sleep for 100 milliSeconds */
#endif
    }
}

BM_Network::PhysicalController::~PhysicalController() {
    RS232_CloseComport(portNumberRead);
    RS232_CloseComport(portNumberWrite);
}

bool BM_Network::PhysicalController::connectPorts(const std::string &input_port, const std::string &output_port) {
    int bdrate = 9600;
    char mode[] = {'8', 'N', '1', 0};

    portNumberWrite = RS232_GetPortnr(output_port.c_str());
    if (portNumberWrite == -1) {
        return false;
    }
    if (RS232_OpenComport(portNumberWrite, bdrate, mode, 0)) {
        std::cout << "Can't open port:COM" << portNumberWrite << std::endl;
        return false;
    }

    portNumberRead = RS232_GetPortnr(input_port.c_str());
    if (portNumberRead == -1) {
        return false;
    }

    if (RS232_OpenComport(portNumberRead, bdrate, mode, 0)) {
        std::cout << "Can't open port:COM" << portNumberRead << std::endl;
        return OpenPortCallbackMessage(AlreadyOpen);
    }
    return true;
}

void BM_Network::PhysicalController::startListeningOnReadPort() {
    //Start listening loop
    std::thread a(&PhysicalController::read, this);
    a.detach();
}

void BM_Network::PhysicalController::addDataLinkController(const std::shared_ptr<BM_Network::IDataLinkControllerPhysical> &dl_cl) {
    datalink_controller.injectImpl(dl_cl);
}

void BM_Network::PhysicalController::disconnectPorts() {
    RS232_CloseComport(portNumberRead);
    RS232_CloseComport(portNumberWrite);
}

void BM_Network::PhysicalController::sendData(const BM_Network::byte *data, size_t size) {
    if (RS232_IsDSREnabled(portNumberWrite) == 0) {
        datalink_controller.connLostCallback(PortType(Write));
        return;
    }
    RS232_SendBuf(portNumberWrite, (unsigned char *) data, (int)size);
#ifdef _WIN32
    Sleep(100);
#else
    usleep(100000);  /* sleep for 1 Second */
#endif
}

void BM_Network::DatalinkControllerPL::connLostCallback(const PortType &portType) {
    datalink_controller_impl->connLostCallback(portType);
}

void BM_Network::DatalinkControllerPL::injectImpl(const std::shared_ptr<IDataLinkControllerPhysical> &impl) {
    datalink_controller_impl = impl;
}

BM_Network::DatalinkControllerPL::DatalinkControllerPL() {

}

void BM_Network::DatalinkControllerPL::sendData(const byte *data, size_t size) {
    datalink_controller_impl->sendData(data, size);
}
