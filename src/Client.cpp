#include "../include/Client.h"
#include <stdexcept>
#include <regex>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cctype>

Client::Client(const std::string& id, const std::string& name, 
               const std::string& phone, const std::string& email)
    : id(id), name(name), phone(phone), email(email) {
    if (!validateId(id)) {
        throw std::invalid_argument("Invalid ID: must be 6-8 digits only");
    }
    if (!validatePhone(phone)) {
        throw std::invalid_argument("Invalid phone number format");
    }
    if (!validateEmail(email)) {
        throw std::invalid_argument("Invalid email format");
    }
    
    // Установка даты регистрации
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    registrationDate = oss.str();
}

std::ostream& operator<<(std::ostream& os, const Client& client) {
    os << "ID: " << client.id << "\n"
       << "Name: " << client.name << "\n"
       << "Phone: " << client.phone << "\n"
       << "Email: " << client.email << "\n"
       << "Registration Date: " << client.registrationDate;
    return os;
}

bool Client::operator==(const Client& other) const {
    return id == other.id;
}

bool Client::operator<(const Client& other) const {
    return name < other.name;
}

void Client::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }
    this->name = name;
}

void Client::setPhone(const std::string& phone) {
    if (!validatePhone(phone)) {
        throw std::invalid_argument("Invalid phone number format");
    }
    this->phone = phone;
}

void Client::setEmail(const std::string& email) {
    if (!validateEmail(email)) {
        throw std::invalid_argument("Invalid email format");
    }
    this->email = email;
}

bool Client::validateId(const std::string& id) {
    // ID должен содержать только цифры и быть длиной от 6 до 8 символов
    if (id.empty() || id.length() < 6 || id.length() > 8) {
        return false;
    }
    for (char c : id) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool Client::validatePhone(const std::string& phone) {
    // Белорусский формат: +375XXXXXXXXX (9 цифр после +375)
    std::regex pattern(R"(\+375\d{9})");
    return std::regex_match(phone, pattern);
}

bool Client::validateEmail(const std::string& email) {
    std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, pattern);
}

std::string Client::toString() const {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

