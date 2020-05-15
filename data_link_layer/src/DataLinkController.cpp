//
// Created by arugaf on 10.05.2020.
//

#include <DataLinkController.h>

#include "DataLinkController.h"
#include "HammingDecoder.hpp"
#include "HammingEncoder.hpp"
#include "Frame.h"

BM_Network::ApplicationControllerDL::ApplicationControllerDL(const std::shared_ptr<IApplicationLayerController>& event_receiver)
        : application_controller_impl(event_receiver) {
    if (!event_receiver) {
        application_controller_impl = std::make_shared<DATA_LAYER_POINTER>();
    }
}

void BM_Network::ApplicationControllerDL::sendMessage(const std::string& receiver, const std::string& sender,
                                                      const std::string& message) {
    application_controller_impl->sendMessage(receiver, sender, message);
}

void BM_Network::ApplicationControllerDL::sendEvent(BM_Network::Event event) {
    application_controller_impl->sendEvent(event);
}

BM_Network::PhysicalControllerDL::PhysicalControllerDL(const std::shared_ptr<IPhysicalLayerController>& receiver)
        : physical_controller_impl(receiver) {
    if (!receiver) {
        physical_controller_impl = std::make_shared<PHYSICAL_LAYER_POINTER>();
    }
}

void BM_Network::PhysicalControllerDL::sendData(const BM_Network::byte* data) {
    physical_controller_impl->sendData(data);
}

bool BM_Network::PhysicalControllerDL::connectPorts(const std::string& input_port, const std::string& output_port) {
    return physical_controller_impl->connectPorts(input_port, output_port);
}
