//
// Created by arugaf on 10.05.2020.
//

#include <Controller.h>

#include "Controller.h"
#include "HammingEncoder.hpp"
#include "Frame.h"

BM_Network::ApplicationLayerController::ApplicationLayerController(
        const std::shared_ptr<IMessageReceiver>& msg_receiver,
        const std::shared_ptr<IEventReceiver>& event_receiver)
        : application_controller_msg_impl(msg_receiver), application_controller_event_impl(event_receiver) {
    if (!msg_receiver || !event_receiver) {
        application_controller_event_impl = std::make_shared<DATA_LAYER_POINTER>();
        application_controller_msg_impl = std::make_shared<DATA_LAYER_POINTER>();
    }
}

void BM_Network::ApplicationLayerController::sendMessage(const std::string& receiver, const std::string& sender,
                                                         const std::string& message) {
    application_controller_msg_impl->sendMessage(receiver, sender, message);
}

void BM_Network::ApplicationLayerController::sendEvent(BM_Network::Event event) {
    application_controller_event_impl->sendEvent(event);
}

void BM_Network::DatalinkLayerController::sendMessage(const std::string& receiver, const std::string& sender,
                                         const std::string& message) {
    std::unique_ptr<IFrame> frame = std::make_unique<Frame>(users[receiver], users[sender], InfFrame, message.size(), message.c_str());
    std::unique_ptr<IEncoder> encoder = std::make_unique<HammingEncoder<byte>>(frame->getFrame(), frame->getSize());
    physical_controller.sendData(encoder->getCodedBytes());
}
