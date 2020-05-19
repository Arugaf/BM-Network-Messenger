#ifndef BM_NETWORK_MESSENGER_ICONNECTOR_H
#define BM_NETWORK_MESSENGER_ICONNECTOR_H

#include "utils.h"

namespace BM_Network {
    class IConnector {
    public:
        virtual bool connectPorts(const std::string& input_port, const std::string& output_port) = 0;
        virtual ~IConnector() = default;
    };
}

#endif //BM_NETWORK_MESSENGER_ICONNECTOR_H
