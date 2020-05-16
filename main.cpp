#include <iostream>

#include "HammingEncoder.hpp"
#include "HammingDecoder.hpp"
#include "Frame.h"
#include "DataLinkController.h"

//TODO:: transformator, кадры, контроль и обработка ошибок, управление соединением, интерфейсы, наследники


int main() {
    BM_Network::ApplicationControllerDL app_cl;

    BM_Network::PhysicalControllerDL phc_cl;

    BM_Network::byte bytes[] = {'A', 'b', 'c'};
    BM_Network::Frame frame(0x7F, -2, BM_Network::FrameType::LFrame, 3, bytes);
    BM_Network::HammingEncoder encoder(frame.getFrame(), frame.getSize());
    BM_Network::DataLinkController dl_cl(phc_cl, app_cl);
    dl_cl.sendData(encoder.getCodedBytes());
    std::cout << std::endl;

    std::string msg = "Pruvet!";
    BM_Network::Frame msg1frame(0x01, 0x01, BM_Network::FrameType::InfFrame, msg.size(), msg.c_str());
    BM_Network::HammingEncoder msg1encoder(msg1frame.getFrame(), msg1frame.getSize());
    dl_cl.sendData(msg1encoder.getCodedBytes());
    std::cout << std::endl;

    BM_Network::byte bytes2[] = {'B', 'o', 'b', 'i', 'k'};
    BM_Network::Frame frame2(0x7F, -2, BM_Network::FrameType::LFrame, 5, bytes2);
    BM_Network::HammingEncoder encoder2(frame2.getFrame(), frame2.getSize());
    dl_cl.sendData(encoder2.getCodedBytes());
    std::cout << std::endl;

    BM_Network::byte bytes3[] = {'E', 'd', 'f', -2, 0x03, 'B', 'o', 'b', 'i', 'k', -2, 0x02};
    BM_Network::Frame fram3(0x7F, 0x03, BM_Network::FrameType::LFrame, 12, bytes3);
    BM_Network::HammingEncoder encoder3(fram3.getFrame(), fram3.getSize());
    dl_cl.sendData(encoder3.getCodedBytes());
    std::cout << std::endl;

    dl_cl.sendMessage("Abc", "Abc", "Pruvet!");
    dl_cl.sendMessage("Abc", "Abc", "Pruvet!");


    return 0;
}



