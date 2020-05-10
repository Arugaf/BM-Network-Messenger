//
// Created by arugaf on 10.05.2020.
//

#include <Controller.h>

#include "Controller.h"
#include "HammingDecoder.hpp"
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

BM_Network::PhysicalLayerController::PhysicalLayerController(const std::shared_ptr<IDataReceiver>& receiver)
        : physical_controller_impl(receiver) {
    if (!receiver) {
        physical_controller_impl = std::make_shared<PHYSICAL_LAYER_POINTER>();
    }
}

void BM_Network::PhysicalLayerController::sendData(const BM_Network::byte* data) {
    physical_controller_impl->sendData(data);
}
