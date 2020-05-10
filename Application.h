//
// Created by vladimir on 10.05.20.
//

#ifndef PROJECT_APPLICATION_H
#define PROJECT_APPLICATION_H

#include "data_link_layer/include/Controller.h"
#include "physical_layer/PhysicalLayerController.h"

class Application {

public:
    explicit Application(
            PhysicalLayerController &ph_cl,
            BM_Network::DatalinkLayerController &dt_cl,
            BM_Network::ApplicationLayerController &ap_cl
    ) : ph_cl(ph_cl), dt_cl(dt_cl), ap_cl(ap_cl) {}

    void start();

    PhysicalLayerController &ph_cl;
    BM_Network::DatalinkLayerController &dt_cl;
    BM_Network::ApplicationLayerController &ap_cl;
};

#endif //PROJECT_APPLICATION_H
