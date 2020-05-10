//
// Created by vladimir on 10.05.20.
//

#ifndef BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
#define BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H

#include <iostream>
#include <Controller.h>

class PhysicalLayerController: virtual public BM_Network::IDataReceiver {
    typedef char byte;

public:
    void bindDataLinkLayer(const BM_Network::IDataReceiver* receiver = nullptr);
//    void sendData(const byte* data);

    //here we receive data from DataLinkLayer
    void sendData(const byte*) override;

private:
    const BM_Network::IDataReceiver *datalink_controller;

};


#endif //BM_NETWORK_MESSENGER_PHYSICALLAYERCONTROLLER_H
