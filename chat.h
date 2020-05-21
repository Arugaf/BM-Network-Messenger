#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include <string>
#include <memory>
#include "include/utils.h"

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

    void injectImpl(std::shared_ptr<BM_Network::IDataLinkControllerApplication> new_impl);

private slots:

    void on_pushButtonDisconnect_clicked();

    void on_pushButtonConnect_clicked();

    void on_pushButtonSendMessage_clicked();

    void on_pushButtonChoose_clicked();

    void on_pushButtonConnectPorts_clicked();

private:
    Ui::Chat *ui;
    QString CurUserName;
    QString CurRecipient;
    QString CurUserInputCOMPort;
    QString CurUserOutputCOMPort;
    QList<QString> Users;
    std::shared_ptr<BM_Network::IDataLinkControllerApplication> data_link_controller;
};

#endif // CHAT_H
