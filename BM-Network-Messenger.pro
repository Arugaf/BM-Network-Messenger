QT += core gui serialport widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_ICONS = canine_y8V_icon.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application_layer/chat.cpp \
    application_layer/mainwindow.cpp \
    data_link_layer/src/BitIO.cpp \
    data_link_layer/src/DataLinkController.cpp \
    data_link_layer/src/Frame.cpp \
    data_link_layer/src/Hamming3ErrorController.cpp \
    main.cpp \
    physical_layer/PhysicalController.cpp \
    physical_layer/rs232.c

HEADERS += \
    application_layer/chat.h \
    application_layer/mainwindow.h \
    data_link_layer/include/ApplicationControllerDL.h \
    data_link_layer/include/BitIO.h \
    data_link_layer/include/DataLinkController.h \
    data_link_layer/include/Frame.h \
    data_link_layer/include/Hamming3ErrorController.h \
    data_link_layer/include/HammingDecoder.hpp \
    data_link_layer/include/HammingEncoder.hpp \
    data_link_layer/include/PhysicalControllerDL.h \
    include/IApplicationLayerController.h \
    include/IConnector.h \
    include/IDataLinkControllerApplication.h \
    include/IDataLinkControllerPhysical.h \
    include/IDataReceiver.h \
    include/IMessageReceiver.h \
    include/IPhysicalLayerController.h \
    include/utils.h \
    physical_layer/DatalinkControllerPL.h \
    physical_layer/IPhysical.h \
    physical_layer/PhysicalController.h \
    physical_layer/rs232.h

FORMS += \
    application_layer/chat.ui \
    application_layer/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
