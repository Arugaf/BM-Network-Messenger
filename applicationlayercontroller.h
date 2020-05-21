#ifndef APPLICATIONLAYERCONTROLLER_H
#define APPLICATIONLAYERCONTROLLER_H

#include "include/utils.h"
#include "chat.h"

#include <memory>

namespace BM_Network {
    class ApplicationLayerController : virtual public IApplicationLayerController {
    public:
        ApplicationLayerController(std::shared_ptr<Chat> new_chat);
        void injectImpl(std::shared_ptr<IDataLinkControllerApplication> dl_cl);
        void sendEvent(Event event) override;
        void addUser(const std::string& user_name) override;
        void removeUser(const std::string& user_name) override;
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) override;

    private:
        std::shared_ptr<IDataLinkControllerApplication> data_link_controller;
        std::shared_ptr<Chat> chat;
    };
}

#endif // APPLICATIONLAYERCONTROLLER_H
