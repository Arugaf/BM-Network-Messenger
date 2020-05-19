#ifndef BM_NETWORK_MESSENGER_IAPPLICATIONLAYERCONTROLLER_H
#define BM_NETWORK_MESSENGER_IAPPLICATIONLAYERCONTROLLER_H

#include "IMessageReceiver.h"
#include "utils.h"

namespace BM_Network {
    class IApplicationLayerController : virtual public IMessageReceiver {
    public:
        virtual void
        sendEvent(Event event) = 0;

        virtual void
        addUser(const std::string& user_name) = 0;

        virtual void
        removeUser(const std::string& user_name) = 0;

        void
        sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) override = 0;

        ~IApplicationLayerController() override = default;
    };

#ifndef APPLICATION_LAYER

    class ApplicationLayerStub : virtual public IApplicationLayerController {
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) override {
            std::cout
                    << "Receiver is: " << receiver << ';' << std::endl
                    << "Sender is: " << sender << ';' << std::endl
                    << "Message is: " << message << std::endl;
        };

        void sendEvent(Event event) override {
            std::cout << "Event received: " << event << std::endl;
        };

        void addUser(const std::string& user_name) override {
            std::cout << "New user: " << user_name << std::endl;
        }

        void removeUser(const std::string& user_name) override {
            std::cout << "User removed: " << user_name << std::endl;
        }
    };
}

#endif

#endif //BM_NETWORK_MESSENGER_IAPPLICATIONLAYERCONTROLLER_H
