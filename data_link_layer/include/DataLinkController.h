//
// Created by arugaf on 10.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_DATALINKCONTROLLER_H
#define BM_NETWORK_MESSENGER_DATALINKCONTROLLER_H

#ifndef APPLICATION_LAYER
#define DATA_LAYER_POINTER ApplicationLayerStub
#endif

#ifndef PHYSICAL_LAYER
#define PHYSICAL_LAYER_POINTER PhysicalLayerStub
#endif

#include "ApplicationControllerDL.h"
#include "IDataLinkControllerApplication.h"
#include "IDataLinkControllerPhysical.h"
#include "PhysicalControllerDL.h"
#include "utils.h"

#include "HammingDecoder.hpp"
#include "HammingEncoder.hpp"
#include "Frame.h"

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <thread>

namespace BM_Network {
    template<typename DataType = byte, typename Encoder = HammingEncoder<DataType>, typename Decoder = HammingDecoder<DataType>>
    class DataLinkController : virtual public IDataLinkControllerPhysical, virtual public IDataLinkControllerApplication {
    public:
        DataLinkController(PhysicalControllerDL& ph_cl, ApplicationControllerDL& ap_cl);
        void addApplicationController(const std::shared_ptr<IApplicationLayerController>& a_c);
        void addPhysicalController(const std::shared_ptr<IPhysicalLayerController>& p_c);
        void sendMessage(const std::string& receiver, const std::string& message) override;
        void sendData(const byte* data, size_t size) override;
        bool connectPorts(const std::string& input_port, const std::string& output_port) override;
        void startListeningOnReadPort() override;
        bool connectToRing(const std::string& user_name) override;
        void unlinkRing() override;
        void connLostCallback(const PortType &portType) override;

    private:
        PhysicalControllerDL& physical_controller;
        ApplicationControllerDL& application_controller;

        std::chrono::milliseconds timeout = std::chrono::milliseconds(5000);
        byte last_msg_receiver = 0;
        bool ack = false;
        bool ret = false;
        bool connected = false;
        bool disconnect_timeout = true; // true

        std::string this_user_name;
        byte address = 0x00;

        std::unordered_map<std::string, byte> users;
        std::map<byte, std::string> addresses;

        std::unordered_map<std::string, bool> informed_users;
        bool check_if_informed = false;

        void handleFrameEvent(Frame& frame, std::unique_ptr<HammingDecoder<DataType>> decoder, const byte* data);
        void checkEvent();
        void maintainConnection();

        std::thread event_handler;
        std::queue<const byte*> events;

        bool working = false;
    };

    template<typename DataType, typename Encoder, typename Decoder>
    BM_Network::DataLinkController<DataType, Encoder, Decoder>::
    DataLinkController(BM_Network::PhysicalControllerDL& ph_cl, BM_Network::ApplicationControllerDL& ap_cl) : physical_controller(ph_cl),
                                                                                                              application_controller(ap_cl) {

    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    BM_Network::DataLinkController<DataType, Encoder, Decoder>::
    sendMessage(const std::string& receiver, const std::string& message) {
        last_msg_receiver = users[receiver];
        Frame frame(users[receiver], users[this_user_name], InfFrame, message.size(), message.c_str());
        Encoder encoder(frame.getFrame(), frame.getSize());
        physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());

        auto timer = std::chrono::milliseconds(0);
        auto timeout_count = 0;
        auto error_count = 0;
        while (timeout_count < 3 && error_count < 3) {
            while(timer < timeout) {
                if (ret) {
                    ret = false;
                    physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());
                    timeout_count = 0;
                    timer = std::chrono::milliseconds(0);
                    ++error_count;
                }

                if (ack) {
                    ack = false;
                    application_controller.sendEvent(Event::ACK);
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                timer += std::chrono::milliseconds(200);
            }
            ++timeout_count;
            timer = std::chrono::milliseconds(0);
            physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());
        }
        application_controller.sendEvent(Event::NO_ACK);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    BM_Network::DataLinkController<DataType, Encoder, Decoder>::
    sendData(const BM_Network::byte* data, size_t size) {
        auto saved_data = new byte[size];
        std::copy(data, data + size, saved_data);
        /*auto decoder = std::make_unique<Decoder>(data);
        Frame frame(decoder->getDecodedBytes(), decoder->getSize());*/
        events.push(saved_data);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::handleFrameEvent(Frame& frame, std::unique_ptr<HammingDecoder<DataType>> decoder, const byte* data) {
        Encoder enc(decoder->getDecodedBytes(),decoder->getSize());

        //ack = true;
        Frame last_frame("", 0);
        if ((!connected && disconnect_timeout) || (!connected && !disconnect_timeout && frame.getType() != LFrame)) {
            physical_controller.sendData(data, enc.getSize());
        } else if (!disconnect_timeout && !connected && frame.getType() == LFrame) {
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
                        informed_users[str] = false;
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

                check_if_informed = true;
                informed_users[this_user_name] = true;
            }

            connected = true;
        } else if (connected) {
            if (frame.getDestination() != address && frame.getDestination() != 0x7F) {
                physical_controller.sendData(data, enc.getSize());
            } else if (frame.getDestination() == address) {
                switch (frame.getType()) {
                    case InfFrame: {
                        if (decoder->isError()) {
                            Frame new_frame(frame.getSender(), address, FrameType::RFrame, 0);
                            Encoder new_encoder(new_frame.getFrame(), new_frame.getSize());
                            physical_controller.sendData(new_encoder.getCodedBytes(), new_encoder.getSize());
                        } else {
                            application_controller.sendMessage(addresses[frame.getSender()], frame.getData());

                            Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                            Encoder a_encoder(a_frame.getFrame(), a_frame.getSize());
                            physical_controller.sendData(a_encoder.getCodedBytes(), a_encoder.getSize());
                        }
                        break;
                    }
                    case AFrame: {
                        if (check_if_informed) {
                            informed_users[addresses[frame.getSender()]] = true;
                        } else if (frame.getSender() == last_msg_receiver) {
                            ack = true;
                            last_msg_receiver = 0;
                        }
                        break;
                    }
                    case RFrame: {
                        ret = true;
                        break;
                    }
                }
            } else if (frame.getDestination() == 0x7F) {
                switch (frame.getType()) {
                    case LFrame: {
                        if (frame.getData().empty()) {
                            Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                            Encoder new_encoder(frame.getFrame(), frame.getSize());
                            physical_controller.sendData(new_encoder.getCodedBytes(), new_encoder.getSize());
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
                                physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());

                                Frame a_frame(new_frame.getSender(), address, FrameType::AFrame, 0);
                                Encoder a_encoder(a_frame.getFrame(), a_frame.getSize());
                                physical_controller.sendData(a_encoder.getCodedBytes(), a_encoder.getSize());
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

                                physical_controller.sendData(data, enc.getSize());

                                Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                                Encoder a_encoder(a_frame.getFrame(), a_frame.getSize());
                                physical_controller.sendData(a_encoder.getCodedBytes(), a_encoder.getSize());
                            }
                        }
                        break;
                    }

                    case UFrame: {
                        if (frame.getSender() != address) {
                            application_controller.sendEvent(DISCONNECT_REQUEST);
                            physical_controller.sendData(data, enc.getSize());

                            Frame a_frame(frame.getSender(), address, FrameType::AFrame, 0);
                            Encoder a_encoder(a_frame.getFrame(), a_frame.getSize());
                            physical_controller.sendData(a_encoder.getCodedBytes(), a_encoder.getSize());
                        }
                        break;
                    }
                }
            }
        }

    }

    template<typename DataType, typename Encoder, typename Decoder>
    bool
    DataLinkController<DataType, Encoder, Decoder>::
    connectPorts(const std::string& input_port, const std::string& output_port)  {
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
        physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());

        auto timer = std::chrono::milliseconds(0);
        auto timeout_count = 0;
        while(timeout_count < 3) {
            while(timer < timeout) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                timer += std::chrono::milliseconds(200);
                if (connected) {
                    bool all_informed = true;
                    for (const auto& it : informed_users) {
                        if (!it.second) {
                            all_informed = false;
                            break;
                        }
                    }
                    if (all_informed) {
                        check_if_informed = false;
                        informed_users.clear();
                        application_controller.sendEvent(CONNECT);
                        return true;
                    }
                }
            }
            ++timeout_count;
            timer = std::chrono::milliseconds(0);
            physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());
        }
        disconnect_timeout = true;

        application_controller.sendEvent(NO_ACK);
        application_controller.sendEvent(DISRUPTION);

        return false;
    }

    // todo
    template<typename DataType, typename Encoder, typename Decoder>
    void DataLinkController<DataType, Encoder, Decoder>::maintainConnection() {
        Frame frame(0x7F, address, LFrame, 0);
        auto encoder = std::make_unique<Encoder>(frame.getFrame(), frame.getSize());
        physical_controller.sendData(encoder->getCodedBytes(), encoder->getSize());

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
            physical_controller.sendData(encoder->getCodedBytes(), encoder->getSize());
        }
    }

    // todo
    template<typename DataType, typename Encoder, typename Decoder>
    void
    DataLinkController<DataType, Encoder, Decoder>::
    unlinkRing() {
        Frame uframe(0x7F, address, UFrame);
        Encoder encoder(uframe.getFrame(), uframe.getSize());

        informed_users.clear();
        for (const auto& it : users) {
            informed_users[it.first] = false;
        }

        informed_users[this_user_name] = true;
        check_if_informed = true;

        physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());

        auto timer = std::chrono::milliseconds(0);
        auto timeout_count = 0;

        while(timeout_count < 3) {
            while(timer < timeout) {
                bool all_informed = true;
                for (const auto& it : informed_users) {
                    if (!it.second) {
                        all_informed = false;
                        break;
                    }
                }

                if (all_informed) {
                    working = false;
                    last_msg_receiver = 0;
                    ack = false;
                    ret = false;
                    connected = false;
                    disconnect_timeout = true;
                    this_user_name.clear();
                    address = 0x00;
                    check_if_informed = false;

                    users.clear();
                    addresses.clear();
                    informed_users.clear();

                    while (!events.empty()) {
                        events.pop();
                    }

                    if (event_handler.joinable()) {
                        event_handler.join();
                    }

                    application_controller.sendEvent(DISCONNECT);
                    physical_controller.disconnectPorts();
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                timer += std::chrono::milliseconds(200);
            }
            ++timeout_count;
            timer = std::chrono::milliseconds(0);
            physical_controller.sendData(encoder.getCodedBytes(), encoder.getSize());
        }

        application_controller.sendEvent(NO_ACK);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    DataLinkController<DataType, Encoder, Decoder>::
    addApplicationController(const std::shared_ptr<IApplicationLayerController>& a_c) {
        application_controller.injectImpl(a_c);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    DataLinkController<DataType, Encoder, Decoder>::
    addPhysicalController(const std::shared_ptr<IPhysicalLayerController>& p_c) {
        physical_controller.injectImpl(p_c);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    DataLinkController<DataType, Encoder, Decoder>::
    connLostCallback(const PortType& portType) {
        if (working) {
            working = false;
            last_msg_receiver = 0;
            ack = false;
            ret = false;
            connected = false;
            disconnect_timeout = true;
            this_user_name.clear();
            address = 0x00;
            check_if_informed = false;

            users.clear();
            addresses.clear();
            informed_users.clear();

            while (!events.empty()) {
                events.pop();
            }

            if (event_handler.joinable()) {
                event_handler.join();
            }

            application_controller.sendEvent(DISRUPTION);
            physical_controller.disconnectPorts();
        }
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    DataLinkController<DataType, Encoder, Decoder>::
    startListeningOnReadPort() {
        physical_controller.startListeningOnReadPort();
        working = true;
        event_handler = std::thread(&DataLinkController::checkEvent, this);
    }

    template<typename DataType, typename Encoder, typename Decoder>
    void
    DataLinkController<DataType, Encoder, Decoder>::
    checkEvent() {
        while (working) {
            if (!events.empty()) {
                auto data = events.front();
                auto decoder = std::make_unique<Decoder>(data);
                Frame frame(decoder->getDecodedBytes(), decoder->getSize());
                handleFrameEvent(frame, std::move(decoder), data);
                delete[] data;
                events.pop();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

// TODO: Разбить файлы, организовать нормальную структуру, понять как обрабатывать ошибки, а также кадры RET и ACK, как устанавливать соединение и раздавать адреса, bimap для users/addresses, создание объектов-контроллеров без использование контроллеров иных уровней
// TODO: Интерфейсные методы (getData) должны возвращать код успеха/ошибки

#endif //BM_NETWORK_MESSENGER_DATALINKCONTROLLER_H

//TODO: дисконнект, интерфейс для аппликейшн лэйер
// TODO: .inl && контроллеры убрать
