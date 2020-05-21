#ifndef BM_NETWORK_MESSENGER_IDATALINKCONTROLLERAPPLICATION_H
#define BM_NETWORK_MESSENGER_IDATALINKCONTROLLERAPPLICATION_H

#include "IMessageReceiver.h"
#include "IConnector.h"
#include "utils.h"

namespace BM_Network {
    class IDataLinkControllerApplication : virtual public IConnector, virtual public IMessageReceiver {
    public:
        bool connectPorts(const std::string &input_port, const std::string &output_port) override = 0;
        void sendMessage(const std::string &receiver, const std::string &sender, const std::string &message) override = 0;
        void startListeningOnReadPort() override = 0;
        virtual bool connectToRing(const std::string& user_name) = 0;
        virtual void unlinkRing() = 0;
    };
}

#endif //BM_NETWORK_MESSENGER_IDATALINKCONTROLLERAPPLICATION_H
