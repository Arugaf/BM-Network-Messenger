//
// Created by vladimir on 10.05.20.
//

#ifndef BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
#define BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else

#include <unistd.h>
#include "DatalinkControllerPL.h"
#include "IDataLinkControllerPhysical.h"
#include "IPhysicalLayerController.h"

#endif

#include "rs232.h"

namespace BM_Network {
    class PhysicalController : virtual public IPhysicalLayerController {
        typedef char byte;
    public:
        explicit PhysicalController(DatalinkControllerPL &dl_cl);
        void addDataLinkController(const std::shared_ptr<IDataLinkControllerPhysical>& p_c);
        ~PhysicalController() override;
        bool connectPorts(const std::string &input_port, const std::string &output_port) override;
        void disconnectPorts() override;
        void sendData(const byte *, size_t size) override;
        void startListeningOnReadPort() override;

    private:
        DatalinkControllerPL &datalink_controller;
        int portNumberRead = 0;
        int portNumberWrite = 0;

        void read();
    };
}

#endif //BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H