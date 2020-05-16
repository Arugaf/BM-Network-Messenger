//
// Created by vladimir on 13.05.20.
//
#ifndef BM_NETWORK_MESSENGER_UTILS_H
#define BM_NETWORK_MESSENGER_UTILS_H

enum PortType{
    READ,
    WRITE
};

enum OpenPortCallbackMessage{
    Success,
    OpenFailed,
    AlreadyOpen
};

#endif
