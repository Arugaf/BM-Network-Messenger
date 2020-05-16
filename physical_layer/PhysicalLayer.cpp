//
// Created by vladimir on 10.05.20.
//

#include "PhysicalLayer.h"
#include "../../../../../Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/include/thread"

void PhysicalLayer::bindDatalinkLayer(IDatalink *receiver) {
    this->datalinkLayer = receiver;
}

void PhysicalLayer::read() {
    unsigned char buf[4096];
    while (true) {
        if (RS232_IsDCDEnabled(portNumberRead) == 0) {
            datalinkLayer->connLostCallback(PortType(READ));
            return;
        }
        int n = RS232_PollComport(portNumberRead, buf, 4095);
        if (n > 0) {
            buf[n] = 0;
            datalinkLayer->sendData((char *) buf);
        }
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);  /* sleep for 100 milliSeconds */
#endif
    }
}

PhysicalLayer::~PhysicalLayer() {

}

void PhysicalLayer::sendData(const PhysicalLayer::byte *data) {
    if (RS232_IsDCDEnabled(portNumberWrite == 0)) {
        datalinkLayer->connLostCallback(PortType(WRITE));
        return;
    }
    RS232_cputs(portNumberWrite, data);
#ifdef _WIN32
    Sleep(100);
#else
    usleep(100000);  /* sleep for 1 Second */
#endif
}

OpenPortCallbackMessage PhysicalLayer::openPort(const char *portName, const PortType &portType) {
    switch (portType) {
        case PortType::READ: {
            int bdrate = 9600;
            char mode[] = {'8', 'N', '1', 0};

            portNumberRead = RS232_GetPortnr(portName);

            if (portNumberRead == -1) {
                return OpenPortCallbackMessage(OpenFailed);
            }
            if (RS232_OpenComport(portNumberRead, bdrate, mode, 0)) {
                std::cout << "Can't open port:COM" << portNumberRead << std::endl;
                return OpenPortCallbackMessage(AlreadyOpen);
            }
            //Start listening loop
            std::thread a(&PhysicalLayer::read,this);
            a.detach();
            break;
        }
        case PortType::WRITE: {
            int bdrate = 9600;
            char mode[] = {'8', 'N', '1', 0};

            portNumberWrite = RS232_GetPortnr(portName);

            if (portNumberWrite == -1) {
                return OpenPortCallbackMessage(OpenFailed);
            }
            if (RS232_OpenComport(portNumberWrite, bdrate, mode, 0)) {
                std::cout << "Can't open port:COM" << portNumberWrite << std::endl;
                return OpenPortCallbackMessage(AlreadyOpen);
            }
            break;
        }
    }
}
