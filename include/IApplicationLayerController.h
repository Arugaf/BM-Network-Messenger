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
        sendMessage(const std::string& user_name, const std::string& message) override = 0;

        ~IApplicationLayerController() override = default;
    };

#ifndef APPLICATION_LAYER

    class ApplicationLayerStub : virtual public IApplicationLayerController {
        void sendMessage(const std::string& user_name, const std::string& message) override {
            std::cout << "Message from: " << user_name << ';' << std::endl
                      << "Message is: " << message << std::endl;
        };

        void sendEvent(Event event) override {
            std::cout << "Event received: ";
            switch (event) {
                case NO_ACK: {
                    std::cout << "NO_ACK" << std::endl;
                    break;
                }
                case DISCONNECT: {
                    std::cout << "DISCONNECT" << std::endl;
                    break;
                }
                case DISRUPTION: {
                    std::cout << "DISRUPTION" << std::endl;
                    break;
                }
                case CONNECT: {
                    std::cout << "CONNECT" << std::endl;
                    break;
                }
                case CONNECT_REQUEST: {
                    std::cout << "CONNECT_REQUEST" << std::endl;
                    break;
                }
                case DISCONNECT_REQUEST: {
                    std::cout << "DISCONNECT_REQUEST" << std::endl;
                    break;
                }
                case ACK: {
                    std::cout << "ACK" << std::endl;
                    break;
                }
            }
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
