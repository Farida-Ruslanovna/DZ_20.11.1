#include "message.h"

// Конструктор
Message::Message(const std::string& text, const std::string& sender, const std::string& receiver)
    : _text(text), _sender(sender), _receiver(receiver) {
}

// Геттеры
std::string Message::getText() const { return _text; }
std::string Message::getSender() const { return _sender; }
std::string Message::getReceiver() const { return _receiver; }

// Сеттеры
void Message::setText(const std::string& text) { _text = text; }
void Message::setSender(const std::string& sender) { _sender = sender; }
void Message::setReceiver(const std::string& receiver) { _receiver = receiver; }

// Сохранение сообщения в файл
bool Message::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs) return false;
    ofs << _text << '\n' << _sender << '\n' << _receiver << '\n';
    chmod(filename.c_str(), S_IRUSR | S_IWUSR); // права 600
    return true;
}

// Загрузка сообщения из файла
bool Message::loadFromFile(const std::string& filename) {
    if (access(filename.c_str(), F_OK) != 0) return false;
    std::ifstream ifs(filename);
    if (!ifs) return false;
    std::getline(ifs, _text);
    std::getline(ifs, _sender);
    std::getline(ifs, _receiver);
    return true;
}