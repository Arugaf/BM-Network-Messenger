#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include <string>
#include <thread>
#include <memory>
#include "include/utils.h"
#include "include/utils.h"
#include "include/IApplicationLayerController.h"
#include "include/IDataLinkControllerApplication.h"

namespace Ui {
class Chat;
}

class Chat : public QDialog
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    void SetCurUserName(QString Name);
    void SetCurUserCOMPorts(QString InputCOM, QString OutputCOM);

    std::string GetCurUserName();
    std::string GetCurUserInputCOMPort();
    std::string GetCurUserOutputCOMPort();
    void Disruption();

    void AddUser(const std::string& user_name);
    void PrintMessage(const std::string& name, const std::string& message);
    //void sendEvent(BM_Network::Event event) override {};
    //void removeUser(const std::string& user_name) override {};
    //void sendMessage(const std::string& user_name, const std::string& message) override {};

    void injectImpl(std::shared_ptr<BM_Network::IDataLinkControllerApplication> new_impl);

private slots:

    void on_pushButtonDisconnect_clicked();

    void on_pushButtonConnect_clicked();

    void on_pushButtonSendMessage_clicked();

    void on_pushButtonChoose_clicked();

    void on_pushButtonConnectPorts_clicked();

    void on_pushButtonListen_clicked();

private:
    Ui::Chat *ui;
    QString CurUserName;
    QString CurRecipient;
    QString CurUserInputCOMPort;
    QString CurUserOutputCOMPort;
    QList<QString> Users;
    std::shared_ptr<BM_Network::IDataLinkControllerApplication> data_link_controller;
};

class ApplicationController : public BM_Network::IApplicationLayerController {
public:
    void sendEvent(BM_Network::Event event) override {
        while (active_event);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        active_event = true;
        switch (event) {
            case BM_Network::NO_ACK: {
                chat->PrintMessage("SYSTEM>", "Message is lost...");
                break;
            }
            case BM_Network::ACK: {
                chat->PrintMessage("SYSTEM>", "Message delivered.");
                break;
            }
            case BM_Network::CONNECT: {
                chat->PrintMessage("SYSTEM>", "Connected!");
                break;
            }
            case BM_Network::CONNECT_REQUEST: {
                chat->PrintMessage("SYSTEM>", "New user added!");
                break;
            }
            case BM_Network::DISCONNECT: {
                chat->PrintMessage("SYSTEM>", "Successfull disconnection...");
                chat->Disruption();
                break;
            }
            case BM_Network::DISCONNECT_REQUEST: {
                chat->PrintMessage("SYSTEM>", "Disconnect request received.");
                break;
            }
            case BM_Network::DISRUPTION: {
                chat->PrintMessage("SYSTEM>", "Connection lost...");
                chat->Disruption();
                break;
            }
        }
        active_event = false;
    };

    void addUser(const std::string& user_name) override {
        chat->AddUser(user_name);
    };

    void removeUser(const std::string& user_name) override {
        return;
    };

    void sendMessage(const std::string& user_name, const std::string& message) override {
        chat->PrintMessage(user_name, message);
    };

    ~ApplicationController() override = default;

    void injectDataLinkController(std::shared_ptr<BM_Network::IDataLinkControllerApplication> new_impl) {
        dl_cl = new_impl;
    };
    void injectChat(std::shared_ptr<Chat> new_impl) {
        chat = new_impl;
    };

private:
    std::shared_ptr<BM_Network::IDataLinkControllerApplication> dl_cl;
    std::shared_ptr<Chat> chat;

    bool active_event = false;
};


#endif // CHAT_H
