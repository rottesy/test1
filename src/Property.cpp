#include "../include/Property.h"
#include <stdexcept>
#include <cctype>
#include <algorithm>

// Конструктор
Property::Property(const std::string& id, const std::string& city, 
                   const std::string& street, const std::string& house,
                   double price, double area, const std::string& description)
    : id(id), city(city), street(street), house(house), description(description), isAvailable(true) {
    if (!validateId(id)) {
        throw std::invalid_argument("Invalid ID: must be 6-8 digits only");
    }
    if (!validateAddressPart(city) || city.empty()) {
        throw std::invalid_argument("Invalid city");
    }
    if (!validateAddressPart(street) || street.empty()) {
        throw std::invalid_argument("Invalid street");
    }
    if (!validateAddressPart(house) || house.empty()) {
        throw std::invalid_argument("Invalid house");
    }
    if (!validatePrice(price)) {
        throw std::invalid_argument("Invalid price: must be between 10000 and 1000000000");
    }
    if (!validateArea(area)) {
        throw std::invalid_argument("Invalid area: must be positive");
    }
    this->price = price;
    this->area = area;
}

// Перегрузка оператора вывода (дружественная функция)
std::ostream& operator<<(std::ostream& os, const Property& prop) {
    os << "ID: " << prop.id << "\n"
       << "Type: " << prop.getType() << "\n"
       << "Address: " << prop.city << ", " << prop.street << ", " << prop.house << "\n"
       << "Price: " << std::fixed << std::setprecision(2) << prop.price << " руб.\n"
       << "Area: " << prop.area << " м²\n"
       << "Description: " << prop.description << "\n"
       << "Available: " << (prop.isAvailable ? "Yes" : "No");
    return os;
}

// Перегрузка операций сравнения
bool Property::operator==(const Property& other) const {
    return id == other.id;
}

bool Property::operator!=(const Property& other) const {
    return !(*this == other);
}

bool Property::operator<(const Property& other) const {
    return price < other.price;
}

bool Property::operator>(const Property& other) const {
    return price > other.price;
}

// Сеттеры с валидацией
void Property::setPrice(double newPrice) {
    if (!validatePrice(newPrice)) {
        throw std::invalid_argument("Invalid price: must be between 10000 and 1000000000");
    }
    price = newPrice;
}

void Property::setArea(double newArea) {
    if (!validateArea(newArea)) {
        throw std::invalid_argument("Invalid area: must be positive");
    }
    area = newArea;
}

void Property::setAddress(const std::string& newCity, const std::string& newStreet, const std::string& newHouse) {
    if (!validateAddressPart(newCity) || newCity.empty()) {
        throw std::invalid_argument("Invalid city");
    }
    if (!validateAddressPart(newStreet) || newStreet.empty()) {
        throw std::invalid_argument("Invalid street");
    }
    if (!validateAddressPart(newHouse) || newHouse.empty()) {
        throw std::invalid_argument("Invalid house");
    }
    city = newCity;
    street = newStreet;
    house = newHouse;
}

void Property::setDescription(const std::string& newDesc) {
    description = newDesc;
}

// Статические функции валидации
bool Property::validatePrice(double price) {
    // Минимальная цена 10000 рублей, максимальная 1 миллиард
    return price >= 10000.0 && price <= 1000000000.0;
}

bool Property::validateArea(double area) {
    return area > 0.0 && area <= 100000.0; // До 100 тысяч м²
}

bool Property::validateId(const std::string& id) {
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

bool Property::validateAddressPart(const std::string& part) {
    // Проверяем, что часть адреса не пуста и не слишком длинная (до 100 символов)
    if (part.empty() || part.length() > 100) {
        return false;
    }
    // Проверяем, что есть хотя бы один печатный символ
    bool hasPrintable = false;
    for (char c : part) {
        if (std::isprint(static_cast<unsigned char>(c))) {
            hasPrintable = true;
            break;
        }
    }
    return hasPrintable;
}
