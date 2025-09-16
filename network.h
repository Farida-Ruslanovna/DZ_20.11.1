#ifndef NETWORK_H
#define NETWORK_H

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class Network {
public:
    static bool initialize(); // инициализация WinSock или ничего
    static void cleanup();    // очистка ресурсов
};

#endif