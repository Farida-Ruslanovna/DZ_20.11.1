#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

// Класс Message для хранения сообщений
class Message {
private:
    std::string _text;     // Текст сообщения
    std::string _sender;   // Отправитель
    std::string _receiver; // Получатель

public:
    Message() = default;

    // Конструктор с параметрами
    Message(const std::string& text, const std::string& sender, const std::string& receiver);

    // Геттеры
    std::string getText() const;
    std::string getSender() const;
    std::string getReceiver() const;

    // Сеттеры
    void setText(const std::string& text);
    void setSender(const std::string& sender);
    void setReceiver(const std::string& receiver);

    // Сохранение сообщения в файл
    bool saveToFile(const std::string& filename) const;

    // Загрузка сообщения из файла
    bool loadFromFile(const std::string& filename);
};

#endif 