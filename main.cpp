#include <iostream>

#include "HammingEncoder.hpp"
#include "HammingDecoder.hpp"
#include "Frame.h"
#include "Controller.h"
#include "Application.h"
#include "FrameTransformer.h"

//TODO:: transformator, кадры, контроль и обработка ошибок, управление соединением, интерфейсы, наследники


int main() {
    /*std::string kekus;

    std::cin >> kekus;

    BM_Network::IFrame* frame = new BM_Network::Frame(31, 3, 1, kekus.size(), kekus.c_str());

    dynamic_cast<BM_Network::Frame*>(frame)->visualize();

    std::cout << std::endl;

    BM_Network::HammingEncoder encode(frame->getFrame(), frame->getSize());
    encode.visualize();
    std::cout << std::endl;
    encode.spoilByte(20);

    BM_Network::HammingDecoder<BM_Network::byte> decode(encode.getCodedBytes());
    decode.visualize();
    std::cout << std::endl;

    std::cout << std::string(decode.getDecodedBytes()) << std::endl;
    std::cout << std::endl;

    BM_Network::Frame frame1(decode.getDecodedBytes(), decode.getSize());
    frame1.visualize();

    std::cout << std::endl << frame1.getData();

    delete frame;*/


    //TODO сделай примерно так

    BM_Network::ApplicationLayerController app_cl;
    //app_cl.sendEvent(BM_Network::ACK);
    //app_cl.sendMessage("kekus", "omegus", "cheburekus");

    PhysicalLayer phc_cl;
    //phc_cl.sendData("lolmega");

    BM_Network::DatalinkLayerController dl_cl;

    //binding controllers as interfaces

    phc_cl.bindDataLinkLayer((BM_Network::IDataReceiver*) &dl_cl);

//    app_cl.bindDataLinkLayer()

//    dl_cl.bindPhysicalLayer()
//    dl_cl.bindApplicationLayer()

    Application application(phc_cl, dl_cl, app_cl);

    //тут какая-то логика старта всей системы, управления жизнью контроллеров и т.п.
    application.start();

    return 0;
}



