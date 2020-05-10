#include <iostream>

#include "HammingEncoder.hpp"
#include "HammingDecoder.hpp"
#include "Frame.h"
#include "Controller.h"

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

    BM_Network::ApplicationLayerController app_cl;
    //app_cl.sendEvent(BM_Network::ACK);
    //app_cl.sendMessage("kekus", "omegus", "cheburekus");

    BM_Network::PhysicalLayerController phc_cl;
    //phc_cl.sendData("lolmega");

    BM_Network::Controller dl_cl(phc_cl, app_cl);
    //dl_cl.sendMessage("user1", "user2", "omegalul kekus cheburekus");
    BM_Network::Frame iframe(0x2, 0x1, BM_Network::InfFrame);
    BM_Network::HammingEncoder iencoder(iframe.getFrame(), iframe.getSize());
    dl_cl.sendData(iencoder.getCodedBytes());

    return 0;
}



