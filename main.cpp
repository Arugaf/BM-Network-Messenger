#include <iostream>

#include "HammingEncoder.hpp"
#include "HammingDecoder.hpp"
#include "Frame.h"
#include "DataLinkController.h"
#include "PhysicalController.h"

//TODO:: transformator, кадры, контроль и обработка ошибок, управление соединением, интерфейсы, наследники


int main() {
    BM_Network::ApplicationControllerDL app_cl_dl;
    BM_Network::PhysicalControllerDL phc_cl_dl;
    BM_Network::DataLinkController dl_cl(phc_cl_dl, app_cl_dl);

    BM_Network::DatalinkControllerPL dl_cl_pl;
    BM_Network::PhysicalController phc_cl(dl_cl_pl);
    phc_cl.addDataLinkController(std::make_shared<BM_Network::DataLinkController<>>(dl_cl));

    dl_cl.addPhysicalController(std::make_shared<BM_Network::PhysicalController>(phc_cl));

//    BM_Network::byte bytes[] = {'A', 'b', 'c'};
//    BM_Network::Frame frame(0x7F, -2, BM_Network::FrameType::LFrame, 3, bytes);
//    BM_Network::HammingEncoder encoder(frame.getFrame(), frame.getSize());
//    dl_cl.sendData(encoder.getCodedBytes());
//    std::cout << std::endl;
//
//    std::string msg = "Pruvet!";
//    BM_Network::Frame msg1frame(0x01, 0x01, BM_Network::FrameType::InfFrame, msg.size(), msg.c_str());
//    BM_Network::HammingEncoder msg1encoder(msg1frame.getFrame(), msg1frame.getSize());
//    dl_cl.sendData(msg1encoder.getCodedBytes());
//    std::cout << std::endl;
//
//    BM_Network::byte bytes2[] = {'B', 'o', 'b', 'i', 'k'};
//    BM_Network::Frame frame2(0x7F, -2, BM_Network::FrameType::LFrame, 5, bytes2);
//    BM_Network::HammingEncoder encoder2(frame2.getFrame(), frame2.getSize());
//    dl_cl.sendData(encoder2.getCodedBytes());
//    std::cout << std::endl;
//
//    BM_Network::byte bytes3[] = {'E', 'd', 'f', -2, 0x03, 'B', 'o', 'b', 'i', 'k', -2, 0x02};
//    BM_Network::Frame fram3(0x7F, 0x03, BM_Network::FrameType::LFrame, 12, bytes3);
//    BM_Network::HammingEncoder encoder3(fram3.getFrame(), fram3.getSize());
//    dl_cl.sendData(encoder3.getCodedBytes());
//    std::cout << std::endl;

    dl_cl.connectPorts("COM3","COM2");
//    std::cout<<"Port opened\n Next?";
//    std::cin.get();

    dl_cl.startListeningOnReadPort();
//    std::cout<<"Port listened\n Next?";
//    std::cin.get();

    dl_cl.connectToRing("user2");
//    std::cout<<"User2 connected\n Next?";
//    std::cin.get();

    //разкоменти для того, кто отправляет
    while(1){
        dl_cl.sendMessage("user1", "user2", "Privet");
        std::cout<<"Message sent\n Next?";
        std::cin.get();
    }

    return 0;
}



