#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chat.h"
#include "user.h"

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>

#include <string>
#include <iostream>
#include <memory>

#include "data_link_layer/include/DataLinkController.h"
#include "data_link_layer/include/HammingEncoder.hpp"
#include "data_link_layer/include/HammingDecoder.hpp"
#include "applicationlayercontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    QTextStream out(stdout);
    out << QObject::tr("Total number of ports available: ") << serialPortInfoList.count() << endl;

    for (int i = 0; i < serialPortInfoList.count(); i++) {
         ui->comboBoxCOMInputPort->addItem(serialPortInfoList[i].portName());
         ui->comboBoxCOMOutputPort->addItem(serialPortInfoList[i].portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButtonEnter_clicked()
{

    QString name = ui->lineEditName->text();
    QString InputCOM = ui->comboBoxCOMInputPort->currentText();
    QString OutputCOM = ui->comboBoxCOMOutputPort->currentText();

    if (name.isEmpty() && InputCOM.isEmpty()) {
        QMessageBox::critical(this, "No data", "Enter name and COM Port numbers!");
    } else if (name.isEmpty()) {
        QMessageBox::critical(this, "No name", "Enter name!");
    } else if (InputCOM.isEmpty()) {
         QMessageBox::critical(this, "No COM Ports", "Enter COM Port numbers!");
    } else if (QString::compare(InputCOM, OutputCOM, Qt::CaseInsensitive) == 0){
        QMessageBox::critical(this, "Equal ports", "Choose different COM ports numbers!");
    } else {
        BM_Network::PhysicalControllerDL ph_cl_dl;
        BM_Network::ApplicationControllerDL ap_cl_dl;

        auto dl_cl = std::make_shared<BM_Network::DataLinkController<BM_Network::byte,
                BM_Network::HammingEncoder<BM_Network::byte>, BM_Network::HammingDecoder<BM_Network::byte>>>(ph_cl_dl, ap_cl_dl);
         //QSharedPointer<BM_Network::DataLinkController>::create("Cancel", this);



        close();
        //connect ports if else true false {}
        auto NewChat = std::make_shared<Chat>();
        auto ap_cl = std::make_shared<BM_Network::ApplicationLayerController>(NewChat);
        NewChat->injectImpl(dl_cl);
        ap_cl->injectImpl(dl_cl);
        ap_cl_dl.injectImpl(ap_cl);

        // new_chat.setAttribute(Qt::WA_DeleteOnClose);
        NewChat->SetCurUserName(name);
        NewChat->SetCurUserCOMPorts(InputCOM, OutputCOM);
        NewChat->setModal(true);
        NewChat->exec();
    }
}

void MainWindow::on_pushButtonAbout_clicked()
{
      QMessageBox::information(this, "О программе", "Программа разработана студентами группы ИУ5-61Б: Евдокимов Арсений\nЖуравлева Полина\nСукиасян Владимир");
}
