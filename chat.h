#ifndef CHAT_H
#define CHAT_H

#include <vector>
#include <string>
#include "message.h"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

// Класс Chat для управления коллекцией сообщений
class Chat {
private:
    std::vector<Message> messages; // Вектор сообщений

public:
    // Добавление сообщения
    void addMessage(const Message& msg);

    // Вывод всех сообщений
    void showMessages() const;

    // Сохранение всех сообщений в файл
    void saveToFile(const std::string& filename) const;

    // Загрузка сообщений из файла
    void loadFromFile(const std::string& filename);
};

#endif 