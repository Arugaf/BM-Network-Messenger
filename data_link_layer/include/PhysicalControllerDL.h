#ifndef BM_NETWORK_MESSENGER_PHYSICALCONTROLLERDL_H
#define BM_NETWORK_MESSENGER_PHYSICALCONTROLLERDL_H

#include "IPhysicalLayerController.h"
#include "utils.h"

#include <memory>

namespace BM_Network {
    class PhysicalControllerDL {
    public:
        explicit PhysicalControllerDL();
        void sendData(const byte* data);
        bool connectPorts(const std::string& input_port, const std::string& output_port);
        void disconnectPorts();
        void startListeningOnReadPort();
        void injectImpl(const std::shared_ptr<IPhysicalLayerController>& impl);

    private:
        std::shared_ptr<IPhysicalLayerController> physical_controller_impl;
    };
}

#endif //BM_NETWORK_MESSENGER_PHYSICALCONTROLLERDL_H
