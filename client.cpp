#include "network.h"
#include <iostream>
#include <thread>
#include <string>

const int PORT = 12345;

int main() {
    if (!Network::initialize()) return 1;

    std::string serverIP;
    std::cout << "Введите IP сервера: ";
    std::getline(std::cin, serverIP);

    // Создаем сокет
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Не удалось создать сокет\n";
        return 1;
    }

    // Адрес сервера
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
#ifdef _WIN32
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);
#else
    inet_aton(serverIP.c_str(), &serverAddr.sin_addr);
#endif

    // Подключаемся к серверу
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка соединения\n";
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return 1;
    }
    std::cout << "Подключено к серверу.\n";

    // Поток для получения сообщений
    std::thread recvThread([sock]() {
        char buffer[1024];
        while (true) {
            int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) {
                std::cout << "Соединение закрыто.\n";
                break;
            }
            buffer[bytesReceived] = '\0';
            std::cout << "Получено: " << buffer << "\n";
        }
        });

    // Отправка сообщений
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") break; // Выход
        send(sock, message.c_str(), message.size(), 0);
    }

    // Завершение работы
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    Network::cleanup();

    return 0;
}