<<<<<<< HEAD
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

    std::string wait;

    std::string input_port;
    std::string output_port;

    again:
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

    //разкоменти для того, кто отправляет
    std::string message;
    std::string receiver;
    while (true) {
        std::cin >> receiver >> message;
        if (receiver == "start_again") {
            goto again;
        }
        if (receiver == "end") {
            dl_cl->unlinkRing();
        } else if (receiver == "quit") {
            dl_cl->unlinkRing();
            return 0;
        } else {
            dl_cl->sendMessage(receiver, message);
        }
    }

    return 0;
}

>>>>>>> 32d12b9... Add files via upload
