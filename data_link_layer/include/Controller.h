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

#include "HammingDecoder.hpp"
#include "HammingEncoder.hpp"
#include "Frame.h"

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

    class PhysicalLayerController {
    public:
        explicit PhysicalLayerController(const std::shared_ptr<IDataReceiver>& receiver = nullptr);
        void sendData(const byte* data);
    private:
        std::shared_ptr<IDataReceiver> physical_controller_impl;
    };

    // TODO: Подстановку можно сделать через шаблоны, а не макросы

    // ---------- DATA LINK LAYER ----------

    class IMediatorError {
    public:
        virtual void notifyError() = 0;
        virtual ~IMediatorError() = default;
    };

    template<typename Encoder = HammingEncoder<byte>, typename Decoder = HammingDecoder<byte>>
    class Controller : virtual public IMessageReceiver, virtual public IMediatorError, virtual public IDataReceiver {
    public:
        Controller(PhysicalLayerController& ph_cl, ApplicationLayerController& ap_cl);
        void sendMessage(const std::string &receiver, const std::string &sender, const std::string &message) override;
        void sendData(const byte* data) override;
        void notifyError() override {};
    private:
        PhysicalLayerController& physical_controller;
        ApplicationLayerController& application_controller;

        byte address = 0x02;
        // Использовать bimap??
        std::map<std::string, byte> users = {{"user1", 0x01},
                                             {"user2", 0x02}};
        std::map<byte, std::string> addresses = {{0x01, "user1"},
                                                 {0x02, "user2"}};

        void handleFrameEvent(Frame& frame, std::unique_ptr<IDecoder<byte>> decoder, const byte* data);
    };

    template<typename Encoder, typename Decoder>
    BM_Network::Controller<Encoder, Decoder>::Controller(BM_Network::PhysicalLayerController& ph_cl,
                                       BM_Network::ApplicationLayerController& ap_cl) : physical_controller(ph_cl),
                                                                                        application_controller(ap_cl) {

    }

    template<typename Encoder, typename Decoder>
    void BM_Network::Controller<Encoder, Decoder>::sendMessage(const std::string& receiver, const std::string& sender,
                                             const std::string& message) {
        Frame frame(users[receiver], users[sender], InfFrame, message.size(), message.c_str());
        std::unique_ptr<IEncoder> encoder = std::make_unique<Encoder>(frame.getFrame(), frame.getSize());
        physical_controller.sendData(encoder->getCodedBytes());
    }

    template<typename Encoder, typename Decoder>
    void BM_Network::Controller<Encoder, Decoder>::sendData(const BM_Network::byte* data) {
        std::unique_ptr<IDecoder<byte>> decoder = std::make_unique<Decoder>(data);
        Frame frame(decoder->getDecodedBytes(), decoder->getSize());
        handleFrameEvent(frame, std::move(decoder), data);
    }

    template<typename Encoder, typename Decoder>
    void Controller<Encoder, Decoder>::handleFrameEvent(Frame& frame, std::unique_ptr<IDecoder<byte>> decoder, const byte* data) {
        auto frame_type = frame.getType();
        switch (frame_type) {
            case InfFrame: {
                auto destination = frame.getDestination();
                if (destination != address) {
                    physical_controller.sendData(data);
                } else {
                    application_controller.sendMessage(addresses[destination], addresses[frame.getSender()], frame.getData());
                }
            }

            case LFrame: {

            }

            default: {

            }
        }
    }
}

// TODO: Разбить файлы, организовать нормальную структуру, понять как обрабатывать ошибки, а также кадры RET и ACK, как устанавливать соединение и раздавать адреса, bimap для users/addresses, создание объектов-контроллеров без использование контроллеров иных уровней

#endif //BM_NETWORK_MESSENGER_CONTROLLER_H
