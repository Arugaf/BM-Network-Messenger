#include "mainwindow.h"
#include "ui_mainwindow.h"

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
#include "data_link_layer/include/Frame.h"
#include "physical_layer/PhysicalController.h"
#include "physical_layer/DatalinkControllerPL.h"

#include "chat.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    std::sort(serialPortInfoList.begin(), serialPortInfoList.end(), [](QSerialPortInfo& lhs, QSerialPortInfo& rhs) {
        return lhs.portName() < rhs.portName();
    });
    QTextStream out(stdout);
    out << QObject::tr("Total number of ports available: ") << serialPortInfoList.count() << endl;

    for (const auto& it : serialPortInfoList) {
        ui->comboBoxCOMInputPort->addItem(it.portName());
        ui->comboBoxCOMOutputPort->addItem(it.portName());
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
    } else if (QString::compare(InputCOM, OutputCOM, Qt::CaseInsensitive) == 0) {
        QMessageBox::critical(this, "Equal ports", "Choose different COM ports numbers!");
    } else {
        BM_Network::ApplicationControllerDL app_cl_dl;
        BM_Network::PhysicalControllerDL phc_cl_dl;
        auto dl_cl = std::make_shared<BM_Network::DataLinkController<>>(phc_cl_dl, app_cl_dl);

        BM_Network::DatalinkControllerPL dl_cl_pl;
        auto phc_cl = std::make_shared<BM_Network::PhysicalController>(dl_cl_pl);
        phc_cl->addDataLinkController(dl_cl);

        dl_cl->addPhysicalController(phc_cl);

         //QSharedPointer<BM_Network::DataLinkController>::create("Cancel", this);

        close();

        auto NewChat = std::make_shared<Chat>();
        NewChat->injectImpl(dl_cl);
        auto ap_cl = std::make_shared<ApplicationController>();

        dl_cl->addApplicationController(ap_cl);
        ap_cl->injectDataLinkController(dl_cl);
        ap_cl->injectChat(NewChat);
       // ap_cl_dl.injectImpl(ap_cl);

        // new_chat.setAttribute(Qt::WA_DeleteOnClose);
        NewChat->SetCurUserName(name);
        NewChat->SetCurUserCOMPorts(InputCOM, OutputCOM);
        NewChat->setModal(true);
        NewChat->exec();
    }
}

void MainWindow::on_pushButtonAbout_clicked()
{
      QMessageBox::information(this, "О программе", "Программа разработана студентами группы ИУ5-61Б:\nЕвдокимов Арсений\nЖуравлева Полина\nСукиасян Владимир");
}
