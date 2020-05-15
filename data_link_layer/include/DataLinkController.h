//
// Created by arugaf on 10.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_DATALINKCONTROLLER_H
#define BM_NETWORK_MESSENGER_DATALINKCONTROLLER_H

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

#include "BM_Network.h"

#include "HammingDecoder.hpp"
#include "HammingEncoder.hpp"
#include "Frame.h"

#include <iostream>
#include <map>
#include <memory>

namespace BM_Network {
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
    };

#endif

    class ApplicationControllerDL {
    public:
        explicit ApplicationControllerDL(const std::shared_ptr<IApplicationLayerController>& event_receiver = nullptr);
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message);
        void sendEvent(Event event);

    private:
        std::shared_ptr<IApplicationLayerController> application_controller_impl;
    };

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
        void sendData(const byte *data) override = 0;
        bool connectPorts(const std::string& input_port, const std::string& output_port) override = 0;

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
    };

#endif

    class PhysicalControllerDL {
    public:
        explicit PhysicalControllerDL(const std::shared_ptr<IPhysicalLayerController>& receiver = nullptr);
        void sendData(const byte* data);
        bool connectPorts(const std::string& input_port, const std::string& output_port);
    private:
        std::shared_ptr<IPhysicalLayerController> physical_controller_impl;
    };

    // TODO: Подстановку можно сделать через шаблоны, а не макросы

    // ---------- DATA LINK LAYER ----------

    template<typename DataType = byte, typename Encoder = HammingEncoder<DataType>, typename Decoder = HammingDecoder<DataType>>
    class DataLinkController : virtual public IMessageReceiver, virtual public IDataReceiver, virtual public IConnector {
    public:
        DataLinkController(PhysicalControllerDL& ph_cl, ApplicationControllerDL& ap_cl);
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) override;
        void sendData(const byte* data) override;
        bool connectPorts(const std::string& input_port, const std::string& output_port) override;

    private:
        PhysicalControllerDL& physical_controller;
        ApplicationControllerDL& application_controller;

        byte address = 0x02;
        // Использовать bimap??
        std::unordered_map<std::string, byte> users = {{"user1", 0x01},
                                             {"user2", 0x02}};
        std::map<byte, std::string> addresses = {{0x01, "user1"},
                                                 {0x02, "user2"}};

        void handleFrameEvent(Frame& frame, std::unique_ptr<IDecoder<DataType>> decoder, const byte* data);
    };

    template<typename DataType, typename Encoder, typename Decoder>
    BM_Network::DataLinkController<DataType, Encoder, Decoder>::DataLinkController(BM_Network::PhysicalControllerDL& ph_cl,
                                                                         BM_Network::ApplicationControllerDL& ap_cl) : physical_controller(ph_cl),
                                                                                                                       application_controller(ap_cl) {

    }

    template<typename DataType, typename Encoder, typename Decoder>
    void BM_Network::DataLinkController<DataType, Encoder, Decoder>::sendMessage(const std::string& receiver, const std::string& sender,
                                                                       const std::string& message) {
        Frame frame(users[receiver], users[sender], InfFrame, message.size(), message.c_str());
        std::unique_ptr<IEncoder> encoder = std::make_unique<Encoder>(frame.getFrame(), frame.getSize());
        physical_controller.sendData(encoder->getCodedBytes());
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void BM_Network::DataLinkController<DataType, Encoder, Decoder>::sendData(const BM_Network::byte* data) {
        auto decoder = std::make_unique<Decoder>(data);
        Frame frame(decoder->getDecodedBytes(), decoder->getSize());
        handleFrameEvent(frame, std::move(decoder), data);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::handleFrameEvent(Frame& frame, std::unique_ptr<IDecoder<DataType>> decoder, const byte* data) {
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

    template<typename DataType, typename Encoder, typename Decoder>
    bool DataLinkController<DataType, Encoder, Decoder>::connectPorts(const std::string& input_port, const std::string& output_port)  {
        return physical_controller.connectPorts(input_port, output_port);
    }
}

// TODO: Разбить файлы, организовать нормальную структуру, понять как обрабатывать ошибки, а также кадры RET и ACK, как устанавливать соединение и раздавать адреса, bimap для users/addresses, создание объектов-контроллеров без использование контроллеров иных уровней
// TODO: Интерфейсные методы (getData) должны возвращать код успеха/ошибки

#endif //BM_NETWORK_MESSENGER_DATALINKCONTROLLER_H

//TODO: дисконнект, интерфейс для аппликейшн лэйер
