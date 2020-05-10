//
// Created by arugaf on 10.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_CONTROLLER_H
#define BM_NETWORK_MESSENGER_CONTROLLER_H

#ifndef APPLICATION_LAYER
#define DATA_LAYER_POINTER ApplicationLayerStub

#else
#define DATA_LAYER_POINTER // TODO:
#endif

#ifndef PHYSICAL_LAYER
#define PHYSICAL_LAYER_POINTER PhysicalLayerStub

#else
#define PHYSICAL_LAYER_POINTER // TODO:
#endif

#include <iostream>
#include <map>
#include <memory>

namespace BM_Network {
    typedef char byte;

    // ---------- APPLICATION LAYER ----------

    enum Event {
        NO_ACK = 1,             // Message lost
        DISCONNECT = 2,         //
        DISRUPTION = 3,         //
        CONNECT = 4,            //
        CONNECT_REQUEST = 5,    //
        DISCONNECT_REQUEST = 6, //
        ACK = 7                 // Message delivered
    };

    class IMessageReceiver {
    public:
        virtual void
        sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) = 0;

        virtual ~IMessageReceiver() = default;
    };

    class IEventReceiver {
    public:
        virtual void sendEvent(Event event) = 0;

        virtual ~IEventReceiver() = default;
    };

#ifndef APPLICATION_LAYER

    class ApplicationLayerStub : virtual public IMessageReceiver, virtual public IEventReceiver {
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) override {
            std::cout
            << "Receiver is: " << receiver << ';' << std::endl
            << "Sender is: " << sender << ';' << std::endl
            << "Message is: " << message << std::endl;
        };

        void sendEvent(Event event) override {
            std::cout << "Event received: " << event << std::endl;
        };
    };

#endif

    class ApplicationLayerController {
    public:
        explicit ApplicationLayerController(const std::shared_ptr<IMessageReceiver>& msg_receiver = nullptr,
                                            const std::shared_ptr<IEventReceiver>& event_receiver = nullptr);
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message);
        void sendEvent(Event event);

    private:
        std::shared_ptr<IMessageReceiver> application_controller_msg_impl;
        std::shared_ptr<IEventReceiver> application_controller_event_impl;
    };

    // ---------- PHYSICAL LAYER ----------

    class IDataReceiver {
    public:
        virtual void sendData(const byte* data) = 0;
        virtual ~IDataReceiver() = default;
    };

#ifndef PHYSICAL_LAYER

    class PhysicalLayerStub : public IDataReceiver {
    public:
        void sendData(const byte* data) override {
            std::cout << "Data received, address is: " << &data << std::endl;
        }
    };

#endif


    // TODO: Подстановку можно сделать через шаблоны, а не макросы

    // ---------- DATA LINK LAYER ----------

    class IMediatorError {
    public:
        virtual void notifyError() = 0;
        virtual ~IMediatorError() = default;
    };

    class DatalinkLayerController : virtual public IMessageReceiver, virtual public IMediatorError, virtual public IDataReceiver {
    public:
        void sendMessage(const std::string &receiver, const std::string &sender, const std::string &message) override;
        void notifyError() override {};
        void sendData(const byte*) override {};
    private:
        std::map<std::string, byte> users = {{"user1", 0x01},
                                             {"user2", 0x02}};
    };
}

#endif //BM_NETWORK_MESSENGER_CONTROLLER_H
