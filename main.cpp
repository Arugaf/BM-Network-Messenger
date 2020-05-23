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
    auto dl_cl = std::make_shared<BM_Network::DataLinkController<>>(phc_cl_dl, app_cl_dl);

    BM_Network::DatalinkControllerPL dl_cl_pl;
    auto phc_cl = std::make_shared<BM_Network::PhysicalController>(dl_cl_pl);
    phc_cl->addDataLinkController(dl_cl);

    dl_cl->addPhysicalController(phc_cl);

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

    std::string wait;

    std::string input_port;
    std::string output_port;

    std::cin >> input_port >> output_port;

    dl_cl->connectPorts(input_port, output_port);
    std::cout << "Connected" << std::endl;
    std::cin >> wait;

    dl_cl->startListeningOnReadPort();
    std::cout << "Started listening" << std::endl;

    std::string user_name;
    std::cin >> user_name;
    dl_cl->connectToRing(user_name);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    //std::cout<<"User2 connected\n Next?";
    std::cout << "Ring" << std::endl;
    std::cin >> wait;

    //разкоменти для того, кто отправляет
    std::string message;
    std::string receiver;
    while(1) {
        std::cin >> receiver >> message;
        dl_cl->sendMessage(receiver, message);
    }

    // TODO: поддержка соединения, подтверждения приема сообщения, подтверждение приема поддержания, подтверждение разрыва соединения, переделать handleFrameEvent

    return 0;
}



