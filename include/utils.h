//
// Created by arugaf on 16.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_UTILS_H
#define BM_NETWORK_MESSENGER_UTILS_H

#include <iostream>

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
        ACK = 7,                // Message delivered
        BAD_PORT = 8
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

    // ---------- APPLICATION LAYER ----------

    class IMessageReceiver {
    public:
        virtual void
        sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) = 0;

        virtual ~IMessageReceiver() = default;
    };

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

#endif

    // ---------- PHYSICAL LAYER ----------

    class IDataReceiver {
    public:
        virtual void sendData(const byte* data) = 0;
        virtual ~IDataReceiver() = default;
    };

    class IConnector {
    public:
        virtual bool connectPorts(const std::string& input_port, const std::string& output_port) = 0;
        virtual ~IConnector() = default;
    };

    class IPhysicalLayerController : virtual public IDataReceiver, virtual public IConnector {
    public:
        void sendData(const byte*  data) override = 0;
        bool connectPorts(const std::string& input_port, const std::string& output_port) override = 0;
        virtual void disconnectPorts() = 0;

        ~IPhysicalLayerController() override = default;
    };

#ifndef PHYSICAL_LAYER

    class PhysicalLayerStub : public IPhysicalLayerController {
    public:
        void sendData(const byte* data) override {
            std::cout << "Data received, address is: " << &data << std::endl;
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

    // ---------- DATA LINK LAYER ----------

    class IDataLinkControllerPhysical : virtual public IMessageReceiver, virtual public IDataReceiver {
    public:
        void sendData(const byte *data) override = 0;
        void sendMessage(const std::string &receiver, const std::string &sender, const std::string &message) override = 0;
        virtual void connLostCallback(const PortType& portType) = 0;
        ~IDataLinkControllerPhysical() override = default;
    };

    class IDataLinkControllerApplication : virtual public IConnector {
    public:
        bool connectPorts(const std::string &input_port, const std::string &output_port) override = 0;
        virtual bool connectToRing(const std::string& user_name) = 0;
        virtual void unlinkRing() = 0;
    };
}

#endif //BM_NETWORK_MESSENGER_UTILS_H
