#ifndef BM_NETWORK_MESSENGER_APPLICATIONCONTROLLERDL_H
#define BM_NETWORK_MESSENGER_APPLICATIONCONTROLLERDL_H

#include "IApplicationLayerController.h"
#include "utils.h"

#include <memory>

namespace BM_Network {
    class ApplicationControllerDL {
    public:
        explicit ApplicationControllerDL();
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message);
        void sendEvent(Event event);
        void addUser(const std::string& user);
        void injectImpl(const std::shared_ptr<IApplicationLayerController>& impl);

    private:
        std::shared_ptr<IApplicationLayerController> application_controller_impl;
    };
}

#endif //BM_NETWORK_MESSENGER_APPLICATIONCONTROLLERDL_H
