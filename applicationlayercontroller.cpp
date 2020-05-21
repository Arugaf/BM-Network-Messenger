#include "applicationlayercontroller.h"

namespace BM_Network {
ApplicationLayerController::ApplicationLayerController(std::shared_ptr<Chat> new_chat) : chat(new_chat)
{

}
void ApplicationLayerController::injectImpl(std::shared_ptr<IDataLinkControllerApplication> dl_cl) {
    data_link_controller = dl_cl;
}

void ApplicationLayerController::sendEvent(Event event) {
    switch (event) {
    case NO_ACK: {
        chat->PrintMessage("SYSTEM>", "Last message lost");
        break;
    }
    case ACK: {
        chat->PrintMessage("SYSTEM>", "Message delivered");
        break;
    }
    case CONNECT: {
        chat->PrintMessage("SYSTEM>", "Connected!");
        break;
    }
    case CONNECT_REQUEST: {
        chat->PrintMessage("SYSTEM>", "New connection");
        break;
    }
    case DISCONNECT: {
        chat->PrintMessage("SYSTEM>", "Successfull disconnection...");
        break;
    }
    case DISCONNECT_REQUEST: {
        chat->PrintMessage("SYSTEM>", "Disconnect request received, disconnection...");
        break;
    }
    case DISRUPTION: {
        chat->PrintMessage("SYSTEM>", "Disconnect request received, disconnection...");
        chat->Disruption();
        break;
    }

    }
}

void ApplicationLayerController::addUser(const std::string& user_name) {
    chat->AddUser(user_name);
}

void ApplicationLayerController::removeUser(const std::string& user_name) {
    user_name.size();
    return;
}

void ApplicationLayerController::sendMessage(const std::string &receiver, const std::string &sender, const std::string &message) {
    receiver.size();
    chat->PrintMessage(sender, message);
}
}


