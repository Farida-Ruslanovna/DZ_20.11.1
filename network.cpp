#include "network.h"
#include <iostream>

bool Network::initialize() {
#ifdef _WIN32
    WSADATA wsaData;
    int wsaret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaret != 0) {
        std::cerr << "WSAStartup failed\n";
        return false;
    }
#endif
    return true;
}

void Network::cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}