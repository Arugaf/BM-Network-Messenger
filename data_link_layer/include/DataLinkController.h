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

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <thread>

namespace BM_Network {
    class ApplicationControllerDL {
    public:
        explicit ApplicationControllerDL();
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message);
        void sendEvent(Event event);
        void addUser(const std::string& user);
        void injectImpl(const std::shared_ptr<IApplicationLayerController>& impl);

    private:
        std::shared_ptr<IApplicationLayerController> application_controller_impl;
    };

    class PhysicalControllerDL {
    public:
        explicit PhysicalControllerDL();
        void sendData(const byte* data);
        bool connectPorts(const std::string& input_port, const std::string& output_port);
        void disconnectPorts();
        void injectImpl(const std::shared_ptr<IPhysicalLayerController>& impl);

    private:
        std::shared_ptr<IPhysicalLayerController> physical_controller_impl;
    };

    template<typename DataType = byte, typename Encoder = HammingEncoder<DataType>, typename Decoder = HammingDecoder<DataType>>
    class DataLinkController : virtual public IDataLinkControllerPhysical, virtual public IDataLinkControllerApplication {
    public:
        DataLinkController(PhysicalControllerDL& ph_cl, ApplicationControllerDL& ap_cl);
        void addApplicationController(const std::shared_ptr<IApplicationLayerController>& a_c);
        void addPhysicalController(const std::shared_ptr<IPhysicalLayerController>& p_c);
        void sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) override;
        void sendData(const byte* data) override;
        bool connectPorts(const std::string& input_port, const std::string& output_port) override;
        bool connectToRing(const std::string& user_name) override;
        void unlinkRing() override;
        void connLostCallback(const PortType &portType) override;

    private:
        PhysicalControllerDL& physical_controller;
        ApplicationControllerDL& application_controller;

        std::chrono::milliseconds timeout = std::chrono::milliseconds(5000);
        bool ack = true;
        bool connected = false;
        bool disconnect_timeout = false; // true
        int ack_counter = 0;

        std::string this_user_name;
        byte address = 0x00;

        std::unordered_map<std::string, byte> users;
        std::map<byte, std::string> addresses;

        void handleFrameEvent(Frame& frame, std::unique_ptr<HammingDecoder<DataType>> decoder, const byte* data);
        void maintainConnection();
    };

    template<typename DataType, typename Encoder, typename Decoder>
    BM_Network::DataLinkController<DataType, Encoder, Decoder>::DataLinkController(BM_Network::PhysicalControllerDL& ph_cl,
                                                                         BM_Network::ApplicationControllerDL& ap_cl) : physical_controller(ph_cl),
                                                                                                                       application_controller(ap_cl) {

    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    BM_Network::DataLinkController<DataType, Encoder, Decoder>::
    sendMessage(const std::string& receiver, const std::string& sender, const std::string& message) {
        Frame frame(users[receiver], users[sender], InfFrame, message.size(), message.c_str());
        auto encoder = std::make_unique<Encoder>(frame.getFrame(), frame.getSize());
        physical_controller.sendData(encoder->getCodedBytes());

        auto timer = std::chrono::milliseconds(0);
        auto timeout_count = 0;
        while(timeout_count < 3) {
            while(timer < timeout) {
                if (ack) {
                    ack = false;
                    application_controller.sendEvent(Event::ACK);
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                timer += std::chrono::milliseconds(200);                                                                                                                 if (timer == std::chrono::milliseconds(1000)) {ack = true;}
            }
            ++timeout_count;
            timer = std::chrono::milliseconds(0);
            physical_controller.sendData(encoder->getCodedBytes());
        }
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void BM_Network::DataLinkController<DataType, Encoder, Decoder>::sendData(const BM_Network::byte* data) {
        auto decoder = std::make_unique<Decoder>(data);
        Frame frame(decoder->getDecodedBytes(), decoder->getSize());
        handleFrameEvent(frame, std::move(decoder), data);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::handleFrameEvent(Frame& frame, std::unique_ptr<HammingDecoder<DataType>> decoder, const byte* data) { ack = true; Frame last_frame("", 0);
        if (!connected && disconnect_timeout && frame.getType() != LFrame) {
            physical_controller.sendData(data);
        } else if (!disconnect_timeout && !connected) {
            if (frame.getSender() == -2) {
                address = 0x01;
                users[frame.getData()] = address; // this_user_name
                addresses[address] = frame.getData(); // this_user_name
                application_controller.addUser(addresses[address]);
            } else {
                address = frame.getSender();
                std::string str;
                bool is_address = false;
                for (const auto& it : frame.getData()) {
                    if (is_address) {
                        users[str] = it;
                        addresses[it] = str;
                        str.clear();
                        is_address = false;
                        application_controller.addUser(addresses[it]);
                        continue;
                    }

                    if (it != -2) {
                        str.push_back(it);
                    } else {
                        is_address = true;
                        continue;
                    }
                }
            }

            connected = true;
        } else if (connected) {
            if (frame.getDestination() != address && frame.getDestination() != 0x7F) {
                physical_controller.sendData(data);
            } else if (frame.getDestination() == address) {
                switch (frame.getType()) {
                    case InfFrame: {
                        if (decoder->isError()) {
                            Frame new_frame(frame.getSender(), address, FrameType::RFrame, 0);
                            Encoder new_encoder(new_frame.getFrame(), new_frame.getSize());
                            physical_controller.sendData(new_encoder.getCodedBytes());
                        } else {
                            application_controller.sendMessage(addresses[frame.getDestination()], addresses[frame.getSender()], frame.getData());

                            Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                            Encoder new_encoder(a_frame.getFrame(), a_frame.getSize());
                            physical_controller.sendData(new_encoder.getCodedBytes());
                        }
                        break;
                    }
                    case AFrame: {
                        ack = true;
                        ++ack_counter;
                        break;
                    }
                    case RFrame: {
                        physical_controller.sendData(last_frame.getFrame());
                        break;
                    }
                }
            } else if (frame.getDestination() == 0x7F) {
                switch (frame.getType()) {
                    case LFrame: {
                        if (frame.getData().empty()) {
                            Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                            Encoder new_encoder(frame.getFrame(), frame.getSize());
                            physical_controller.sendData(new_encoder.getCodedBytes());
                        } else {
                            application_controller.sendEvent(Event::CONNECT_REQUEST);

                            if (frame.getSender() == -2) {
                                std::string new_info = frame.getData();
                                new_info.push_back(-2);
                                new_info.push_back(addresses.size() + 1);
                                for (const auto& it : users) {
                                    for (const auto& iti : it.first) {
                                        new_info.push_back(iti);
                                    }
                                    new_info.push_back(-2);
                                    new_info.push_back(it.second);
                                }

                                Frame new_frame(0x7F, addresses.size() + 1, LFrame, new_info.size(), new_info.c_str());
                                addresses[addresses.size() + 1] = frame.getData();
                                users[frame.getData()] = users.size() + 1;

                                application_controller.addUser(frame.getData());

                                Encoder encoder(new_frame.getFrame(), new_frame.getSize());
                                physical_controller.sendData(encoder.getCodedBytes());
                                // new_frame.visualize();

                                Frame a_frame(new_frame.getSender(), address, FrameType::AFrame, 0);
                                Encoder new_encoder(a_frame.getFrame(), a_frame.getSize());
                                physical_controller.sendData(new_encoder.getCodedBytes());
                            } else {
                                std::string str;
                                bool is_address = false;
                                for (const auto& it : frame.getData()) {
                                    if (is_address) {
                                        users[str] = it;
                                        addresses[it] = str;

                                        application_controller.addUser(addresses[it]);
                                        break;
                                    }

                                    if (it != -2) {
                                        str.push_back(it);
                                    } else {
                                        is_address = true;
                                        continue;
                                    }
                                }

                                physical_controller.sendData(data);

                                Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                                Encoder new_encoder(a_frame.getFrame(), a_frame.getSize());
                                physical_controller.sendData(new_encoder.getCodedBytes());
                            }
                        }
                        break;
                    }

                    case UFrame: {
                        if (frame.getSender() != address) {
                            physical_controller.sendData(data);

                            Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                            Encoder new_encoder(a_frame.getFrame(), a_frame.getSize());
                            physical_controller.sendData(new_encoder.getCodedBytes());

                            connected = false;
                            disconnect_timeout = true;

                            physical_controller.disconnectPorts();

                            users.clear();
                            addresses.clear();

                            application_controller.sendEvent(DISCONNECT_REQUEST);
                            application_controller.sendEvent(DISRUPTION);
                        }
                        break;
                    }
                }
            }
        }

    }

    template<typename DataType, typename Encoder, typename Decoder>
    bool DataLinkController<DataType, Encoder, Decoder>::connectPorts(const std::string& input_port, const std::string& output_port)  {
        return physical_controller.connectPorts(input_port, output_port);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    bool
    DataLinkController<DataType, Encoder, Decoder>::
    connectToRing(const std::string& user_name) {
        disconnect_timeout = false;

        this_user_name = user_name;

        Frame frame(0x7F, -2, FrameType::LFrame, user_name.size(), user_name.c_str());
        Encoder encoder(frame.getFrame(), frame.getSize());
        physical_controller.sendData(encoder.getCodedBytes());

        auto timer = std::chrono::milliseconds(0);
        auto timeout_count = 0;
        while(timeout_count < 3) {
            while(timer < timeout) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                timer += std::chrono::milliseconds(200);
                if (connected) {
                    application_controller.sendEvent(CONNECT);
                    return true;
                }
            }
            ++timeout_count;
            timer = std::chrono::milliseconds(0);
            physical_controller.sendData(encoder.getCodedBytes());
        }
        disconnect_timeout = true;

        application_controller.sendEvent(NO_ACK);
        application_controller.sendEvent(DISRUPTION);

        return false;
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::maintainConnection() {
        Frame frame(0x7F, address, LFrame, 0);
        auto encoder = std::make_unique<Encoder>(frame.getFrame(), frame.getSize());
        physical_controller.sendData(encoder->getCodedBytes());

        auto timer = std::chrono::milliseconds(0);
        auto timeout_count = 0;
        while (timeout_count < 3) {
            while (timer < timeout) {
                if (ack) {
                    ack = false;
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                timer += std::chrono::milliseconds(200);
            }
            ++timeout_count;
            timer = std::chrono::milliseconds(0);
            physical_controller.sendData(encoder->getCodedBytes());
        }
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::unlinkRing() {
        Frame uframe(0x7F, address, UFrame);
        auto counter = users.size();
        while (counter <= ack_counter) {                                                                                                                                                                    ++ack_counter;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        connected = false;
        disconnect_timeout = true;

        physical_controller.disconnectPorts();

        users.clear();
        addresses.clear();

        application_controller.sendEvent(DISCONNECT);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::addApplicationController(
            const std::shared_ptr<IApplicationLayerController>& a_c) {
        application_controller.injectImpl(a_c);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::addPhysicalController(
            const std::shared_ptr<IPhysicalLayerController>& p_c) {
        physical_controller.injectImpl(p_c);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::connLostCallback(const PortType& portType) {
        connected = false;
        disconnect_timeout = true;

        physical_controller.disconnectPorts();

        users.clear();
        addresses.clear();

        application_controller.sendEvent(DISRUPTION);
    }
}

// TODO: Разбить файлы, организовать нормальную структуру, понять как обрабатывать ошибки, а также кадры RET и ACK, как устанавливать соединение и раздавать адреса, bimap для users/addresses, создание объектов-контроллеров без использование контроллеров иных уровней
// TODO: Интерфейсные методы (getData) должны возвращать код успеха/ошибки

#endif //BM_NETWORK_MESSENGER_DATALINKCONTROLLER_H

//TODO: дисконнект, интерфейс для аппликейшн лэйер
