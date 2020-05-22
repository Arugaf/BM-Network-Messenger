#ifndef BM_NETWORK_MESSENGER_IPHYSICALLAYERCONTROLLER_H
#define BM_NETWORK_MESSENGER_IPHYSICALLAYERCONTROLLER_H

#include "IConnector.h"
#include "IDataReceiver.h"
#include "utils.h"

namespace BM_Network {
    class IPhysicalLayerController : virtual public IDataReceiver, virtual public IConnector {
    public:
        void sendData(const byte* data, size_t size) override = 0;

        bool connectPorts(const std::string& input_port, const std::string& output_port) override = 0;

        void startListeningOnReadPort() override = 0;

        virtual void disconnectPorts() = 0;

        ~IPhysicalLayerController() override = default;
    };

#ifndef PHYSICAL_LAYER

    class PhysicalLayerStub : public IPhysicalLayerController {
    public:
        void sendData(const byte* data, size_t size) override {
            std::cout << "Data received,size is "<<size<<",data is: " << &data << std::endl;
        }

        void startListeningOnReadPort() override {
            std::cout << "startListeningOnReadPort" << std::endl;
        }

        bool connectPorts(const std::string& input_port, const std::string& output_port) override {
            std::cout << input_port << ' ' << output_port << std::endl;
            return true;
        }

        void disconnectPorts() override {
            std::cout << "Disconnected..." << std::endl;
        }
    };

#endif
}

#endif //BM_NETWORK_MESSENGER_IPHYSICALLAYERCONTROLLER_H
