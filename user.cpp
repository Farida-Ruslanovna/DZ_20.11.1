#include "user.h"

// Проверка существования файла
bool User::fileExists(const std::string& filename) {
    return access(filename.c_str(), F_OK) == 0;
}

// Загрузка данных пользователя из файла
bool User::loadFromFile(const std::string& filename) {
    if (!fileExists(filename))
        return false;
    std::ifstream ifs(filename);
    if (!ifs)
        return false;
    std::getline(ifs, _name);
    std::getline(ifs, _login);
    std::getline(ifs, _pass);
    return true;
}

// Сохранение данных пользователя в файл
bool User::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs)
        return false;
    ofs << _name << '\n' << _login << '\n' << _pass << '\n';
    chmod(filename.c_str(), S_IRUSR | S_IWUSR);
    return true;
}