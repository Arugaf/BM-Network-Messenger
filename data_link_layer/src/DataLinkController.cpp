//
// Created by arugaf on 10.05.2020.
//

#include <DataLinkController.h>

#include "DataLinkController.h"
#include "HammingDecoder.hpp"
#include "HammingEncoder.hpp"
#include "Frame.h"

BM_Network::ApplicationControllerDL::ApplicationControllerDL() {
#ifndef APPLICATION_LAYER
    application_controller_impl = std::make_shared<DATA_LAYER_POINTER>();
#endif
}

void BM_Network::ApplicationControllerDL::sendMessage(const std::string& receiver, const std::string& sender,
                                                      const std::string& message) {
    application_controller_impl->sendMessage(receiver, sender, message);
}

void BM_Network::ApplicationControllerDL::sendEvent(BM_Network::Event event) {
    application_controller_impl->sendEvent(event);
}

void BM_Network::ApplicationControllerDL::addUser(const std::string& user) {
    application_controller_impl->addUser(user);
}

void BM_Network::ApplicationControllerDL::injectImpl(const std::shared_ptr<IApplicationLayerController>& impl) {

}

BM_Network::PhysicalControllerDL::PhysicalControllerDL() {
#ifndef PHYSICAL_LAYER
    physical_controller_impl = std::make_shared<PHYSICAL_LAYER_POINTER>();
#endif
}

void BM_Network::PhysicalControllerDL::sendData(const BM_Network::byte* data) {
    physical_controller_impl->sendData(data);
}

bool BM_Network::PhysicalControllerDL::connectPorts(const std::string& input_port, const std::string& output_port) {
    return physical_controller_impl->connectPorts(input_port, output_port);
}

void BM_Network::PhysicalControllerDL::disconnectPorts() {
    physical_controller_impl->disconnectPorts();
}
