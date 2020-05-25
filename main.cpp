#include <iostream>
#include <QApplication>

#include "data_link_layer/include/HammingEncoder.hpp"
#include "data_link_layer/include/HammingDecoder.hpp"
#include "data_link_layer/include/Frame.h"
#include "data_link_layer/include/DataLinkController.h"
#include "physical_layer/PhysicalController.h"
#include "physical_layer/DatalinkControllerPL.h"

#include "application_layer/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
