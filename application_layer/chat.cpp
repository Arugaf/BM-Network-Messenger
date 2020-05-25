#include "application_layer/chat.h"
#include "ui_chat.h"

#include <QMessageBox>
#include <QListWidget>
#include <QList>
#include <QMutableListIterator>

Chat::Chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    ui->pushButtonSendMessage->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(false);
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonChoose->setEnabled(false);
    ui->pushButtonListen->setEnabled(false);
    ui->textEdit->setReadOnly(true);
}

Chat::~Chat()
{
    delete ui;
}


void Chat::SetCurUserName(QString Name) {
    CurUserName = Name;
}

void Chat::SetCurUserCOMPorts(QString InputCOM, QString OutputCOM) {
    CurUserInputCOMPort = InputCOM;
    CurUserOutputCOMPort = OutputCOM;
}

std::string Chat::GetCurUserName() {
    return CurUserName.toStdString();
}
std::string Chat::GetCurUserInputCOMPort() {
    return CurUserInputCOMPort.toStdString();
}
std::string Chat::GetCurUserOutputCOMPort() {
    return CurUserOutputCOMPort.toStdString();
}

void Chat::on_pushButtonDisconnect_clicked()
{

//    QMutableListIterator<QString> i(Users);
//    while(i.hasNext()) {
//      QString currentValue=i.next();
//      if(QString::compare(currentValue, CurUserName) == 0) {
//        i.remove();
//        break;
//      }
//    }

    data_link_controller->unlinkRing();
}


void Chat::on_pushButtonConnect_clicked()
{
    if (data_link_controller->connectToRing(CurUserName.toStdString())) {
        ui->labelUser->setText(CurUserName);

        ui->pushButtonSendMessage->setEnabled(true);
        ui->pushButtonDisconnect->setEnabled(true);
        ui->pushButtonChoose->setEnabled(true);
        ui->pushButtonConnect->setEnabled(false);

        //Users << CurUserName;

        ui->listWidget->clear();
        ui->listWidget->addItems(Users);
    }

}

void Chat::on_pushButtonSendMessage_clicked()
{
    QString Message = ui->lineEditMessage->text();
    if (Message.isEmpty()) {
        QMessageBox::warning(this, "No message", "Enter your message!");
    }
    else if (ui->labelReceiver->text() == "To: ...") {
        QMessageBox::warning(this, "No recipient", "Choose recipient!");

    } else {
        ui->textEdit->insertPlainText("To " + CurRecipient + ": ");
        ui->textEdit->insertPlainText(Message);
        ui->textEdit->insertPlainText("\n");
        //
        data_link_controller->sendMessage(CurRecipient.toStdString(), Message.toStdString());

        ui->lineEditMessage->clear();
        Message.clear();
    }
}

void Chat::on_pushButtonChoose_clicked()
{
    if (!ui->listWidget->currentItem()) {
        QMessageBox::warning(this, "No recipient", "Choose recipient!");
    } else {
        CurRecipient = ui->listWidget->currentItem()->text();
        ui->labelReceiver->setText("To: " + CurRecipient);
    }
}

void Chat::on_pushButtonConnectPorts_clicked()
{
    PrintMessage("SYSTEM>", "Connecting ports...");
    if (data_link_controller->connectPorts(GetCurUserInputCOMPort(), GetCurUserOutputCOMPort())) {
         PrintMessage("SYSTEM>", "Ports connected!");
         ui->pushButtonConnectPorts->setEnabled(false);
         ui->pushButtonListen->setEnabled(true);
    } else {
         PrintMessage("SYSTEM>", "Ports connection error!");
    }

}

void Chat::on_pushButtonListen_clicked()
{
    data_link_controller->startListeningOnReadPort(); // noexcept
    ui->pushButtonListen->setEnabled(false);
    ui->pushButtonConnect->setEnabled(true);
}

void Chat::AddUser(const std::string& user_name) {
    QString u_name = QString::fromStdString(user_name);
    Users << u_name;
    ui->listWidget->clear();
    ui->listWidget->addItems(Users);
}

void Chat::PrintMessage(const std::string& name, const std::string& msg) {
    QString u_name = QString::fromStdString(name);
    QString u_message = QString::fromStdString(msg);
    if (u_name.compare("SYSTEM>") == 0) {
        ui->textEdit->setFontItalic(true);
        ui->textEdit->insertPlainText(u_name + " " + u_message + '\n');
        ui->textEdit->setFontItalic(false);
    } else {
        ui->textEdit->insertPlainText(u_name + ": " + u_message + '\n');
    }
}

void Chat::Disruption()
{
    ui->pushButtonSendMessage->setEnabled(false);
    ui->pushButtonChoose->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(false);
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonListen->setEnabled(false);

    ui->pushButtonConnectPorts->setEnabled(true);

    ui->listWidget->clear();
    Users.clear();
    ui->labelReceiver->setText("To: ...");
    CurRecipient.clear();
}


void Chat::injectImpl(std::shared_ptr<BM_Network::IDataLinkControllerApplication> new_impl) {
    data_link_controller = new_impl;
}
