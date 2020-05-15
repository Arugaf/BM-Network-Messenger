#ifndef BM_NETWORK_MESSENGER_BM_NETWORK_H
#define BM_NETWORK_MESSENGER_BM_NETWORK_H

namespace BM_Network {
    typedef char byte;
    typedef unsigned long long size_t;

    enum Event {
        NO_ACK = 1,             // Message lost
        DISCONNECT = 2,         //
        DISRUPTION = 3,         //
        CONNECT = 4,            //
        CONNECT_REQUEST = 5,    //
        DISCONNECT_REQUEST = 6, //
        ACK = 7                 // Message delivered
    };

    enum FrameType {
        InfFrame = 1, // Information frame for data exchange
        LFrame = 2,   // Link frame for connection establishing
        UFrame = 3,   // Uplink frame for disconnection
        AFrame = 4,   // ACK frame for receipt confirmation
        RFrame = 5    // Ret frame for repeated request
    };

    enum PortType {
        Read,
        Write
    };
}

#endif //BM_NETWORK_MESSENGER_BM_NETWORK_H
