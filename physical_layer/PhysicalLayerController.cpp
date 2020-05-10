//
// Created by vladimir on 10.05.20.
//

#include "PhysicalLayerController.h"

//PhysicalLayerController::PhysicalLayerController(const std::shared_ptr<IDataReceiver>& receiver)
//        : physical_controller_impl(receiver) {
//    if (!receiver) {
//        physical_controller_impl = std::make_shared<PHYSICAL_LAYER_POINTER>();
//    }
//}
//
//void PhysicalLayerController::sendData(const BM_Network::byte* data) {
//    physical_controller_impl->sendData(data);
//}

void PhysicalLayerController::sendData(const PhysicalLayerController::byte *) {

}

void PhysicalLayerController::bindDataLinkLayer(const BM_Network::IDataReceiver *receiver) {
    this->datalink_controller = receiver;
}