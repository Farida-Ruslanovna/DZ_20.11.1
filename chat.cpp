#include "chat.h"

// Добавление сообщения
void Chat::addMessage(const Message& msg) {
    messages.push_back(msg);
}

// Вывод всех сообщений
void Chat::showMessages() const {
    for (const auto& msg : messages) {
        std::cout << "От: " << msg.getSender()
            << " Кому: " << msg.getReceiver()
            << "\nСообщение: " << msg.getText() << "\n\n";
    }
}

// Сохранение всех сообщений в файл
void Chat::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs) return;

    for (const auto& msg : messages) {
        ofs << msg.getText() << '\n'
            << msg.getSender() << '\n'
            << msg.getReceiver() << '\n';
    }
    chmod(filename.c_str(), S_IRUSR | S_IWUSR);
}

// Загрузка сообщений из файла
void Chat::loadFromFile(const std::string& filename) {
    messages.clear();

    std::ifstream ifs(filename);
    if (!ifs) return;

    while (ifs) {
        std::string text, sender, receiver;
        std::getline(ifs, text);
        if (ifs.eof() || text.empty()) break;

        std::getline(ifs, sender);
        if (ifs.eof() || sender.empty()) break;

        std::getline(ifs, receiver);
        if (ifs.eof() || receiver.empty()) break;

        messages.emplace_back(text, sender, receiver);
    }
}

