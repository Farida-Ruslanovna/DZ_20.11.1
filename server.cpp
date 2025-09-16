#include "network.h"
#include <iostream>
#include <thread>
#include <string>

const int PORT = 12345;

// Обработчик для получения сообщений от клиента
void handleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
        // Получение данных
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cout << "Клиент отключился или ошибка.\n";
            break;
        }
        buffer[bytesReceived] = '\0'; // Завершение строки
        std::cout << "Получено: " << buffer << "\n";
    }
    // Закрытие сокета клиента
#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

int main() {
    if (!Network::initialize()) return 1;

    // Создаем TCP-сокет
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Не удалось создать сокет\n";
        return 1;
    }

    // Заполняем адрес сервера
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Любой интерфейс

    // Связываем сокет с адресом
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка привязки\n";
#ifdef _WIN32
        closesocket(serverSocket);
#else
        close(serverSocket);
#endif
        return 1;
    }

    // Начинаем слушать входящие соединения
    listen(serverSocket, 1);
    std::cout << "Сервер запущен. Ожидаем подключение...\n";

    sockaddr_in clientAddr{};
    socklen_t clientSize = sizeof(clientAddr);
    // Принятие входящего соединения
    int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket < 0) {
        std::cerr << "Ошибка при подключении\n";
#ifdef _WIN32
        closesocket(serverSocket);
#else
        close(serverSocket);
#endif
        return 1;
    }

    std::cout << "Клиент подключен.\n";

    // Обработка входящих сообщений в отдельном потоке
    std::thread recvThread([clientSocket]() {
        handleClient(clientSocket);
        });

    // Отправка сообщений
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") break; // Выход из чата
        send(clientSocket, message.c_str(), message.size(), 0);
    }

    // Завершение работы
#ifdef _WIN32
    closesocket(clientSocket);
    closesocket(serverSocket);
#else
    close(clientSocket);
    close(serverSocket);
#endif

    Network::cleanup();

    return 0;
}