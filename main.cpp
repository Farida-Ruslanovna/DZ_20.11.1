// DZ_20.11.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <thread>
#include <string>
#include "network.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Функция для приема сообщений по протоколу с длиной
void receiveMessages(int sock) {
    while (true) {
        uint32_t len_net;
        // Получаем 4 байта длины сообщения
        int r = recv(sock, (char*)&len_net, sizeof(len_net), 0);
        if (r <= 0) {
            std::cout << "Соединение разорвано или ошибка.\n";
            break;
        }
        // Конвертируем из сети в host порядок
        uint32_t len = ntohl(len_net);
        if (len == 0) {
            // 0 длина — сигнал завершения
            std::cout << "Соединение закрыто.\n";
            break;
        }
        // Читаем само сообщение
        std::string msg;
        msg.resize(len);
        int totalReceived = 0;
        while (totalReceived < (int)len) {
            int r2 = recv(sock, &msg[totalReceived], len - totalReceived, 0);
            if (r2 <= 0) {
                std::cout << "Соединение разорвано или ошибка.\n";
                return;
            }
            totalReceived += r2;
        }
        std::cout << "Друг: " << msg << "\n";
    }
}

// Функция для отправки сообщений по протоколу с длиной
void sendMessages(int sock) {
    std::string line;
    while (true) {
        std::getline(std::cin, line);
        if (line == "/exit") {
            // Отправляем нулевую длину как сигнал завершения
            uint32_t len = htonl(0);
            send(sock, (char*)&len, sizeof(len), 0);
            break;
        }
        // Отправляем длину
        uint32_t len = htonl(line.size());
        send(sock, (char*)&len, sizeof(len), 0);
        // Отправляем само сообщение
        send(sock, line.c_str(), line.size(), 0);
    }
}

// Функция запуска сервера
void runServer(int port) {
    if (!Network::initialize()) return;

    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY; // любой интерфейс

    if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Ошибка привязки\n";
#ifdef _WIN32
        closesocket(listenSocket);
#else
        close(listenSocket);
#endif
        return;
    }

    listen(listenSocket, 1);
    std::cout << "Ожидание клиента...\n";

    sockaddr_in clientAddr{};
    socklen_t clientSize = sizeof(clientAddr);
    int clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket < 0) {
        std::cerr << "Ошибка при подключении\n";
#ifdef _WIN32
        closesocket(listenSocket);
#else
        close(listenSocket);
#endif
        return;
    }
    std::cout << "Клиент подключен!\n";

    // Запускаем поток для приема сообщений
    std::thread receiver(receiveMessages, clientSocket);
    // В основном — для отправки
    sendMessages(clientSocket);
    // Ждем завершения
    receiver.join();

#ifdef _WIN32
    closesocket(clientSocket);
    closesocket(listenSocket);
#else
    close(clientSocket);
    close(listenSocket);
#endif
    Network::cleanup();
}

// Функция запуска клиента
void runClient(const std::string& ip, int port) {
    if (!Network::initialize()) return;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
#ifdef _WIN32
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
#else
    inet_aton(ip.c_str(), &serverAddr.sin_addr);
#endif

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка соединения\n";
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return;
    }

    std::cout << "Подключено к серверу. Введите сообщения. /exit для выхода.\n";

    // Запускаем поток для приема сообщений
    std::thread receiver(receiveMessages, sock);
    // Вводим сообщения
    sendMessages(sock);
    // Ждем завершения
    receiver.join();

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    Network::cleanup();
}

// Главная функция
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    if (argc < 2) {
        std::cout << "Использование:\n";
        std::cout << "  " << argv[0] << " server <port>\n";
        std::cout << "  " << argv[0] << " client <ip> <port>\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "server") {
        int port = 12345;
        if (argc >= 3) port = std::stoi(argv[2]);
        runServer(port);
    }
    else if (mode == "client") {
        if (argc < 4) {
            std::cerr << "Укажите IP и порт\n";
            return 1;
        }
        std::string ip = argv[2];
        int port = std::stoi(argv[3]);
        runClient(ip, port);
    }
    else {
        std::cerr << "Некорректный режим. Используйте server или client.\n";
        return 1;
    }
    return 0;
}