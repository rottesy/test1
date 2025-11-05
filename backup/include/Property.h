#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

// Базовый абстрактный класс недвижимости
class Property {
protected:
    std::string id;
    std::string city;
    std::string street;
    std::string house;
    double price;
    double area;
    std::string description;
    bool isAvailable;

public:
    Property(const std::string& id, const std::string& city, 
             const std::string& street, const std::string& house,
             double price, double area, const std::string& description);
    virtual ~Property() = default;

    // Виртуальные функции (абстрактные методы)
    virtual std::string getType() const = 0;
    virtual void displayInfo() const = 0;
    virtual std::string toFileString() const = 0;
    virtual Property* clone() const = 0;

    // Дружественная функция для вывода
    friend std::ostream& operator<<(std::ostream& os, const Property& prop);

    // Перегрузка операций
    bool operator==(const Property& other) const;
    bool operator!=(const Property& other) const;
    bool operator<(const Property& other) const;
    bool operator>(const Property& other) const;
    
    // Геттеры
    std::string getId() const { return id; }
    std::string getCity() const { return city; }
    std::string getStreet() const { return street; }
    std::string getHouse() const { return house; }
    std::string getAddress() const { return city + ", " + street + ", " + house; } // Полный адрес для обратной совместимости
    double getPrice() const { return price; }
    double getArea() const { return area; }
    std::string getDescription() const { return description; }
    bool getIsAvailable() const { return isAvailable; }

    // Сеттеры с валидацией
    void setPrice(double newPrice);
    void setArea(double newArea);
    void setAddress(const std::string& city, const std::string& street, const std::string& house);
    void setDescription(const std::string& newDesc);
    void setAvailable(bool available) { isAvailable = available; }

    // Статические функции для валидации
    static bool validatePrice(double price);
    static bool validateArea(double area);
    static bool validateId(const std::string& id);
    static bool validateAddressPart(const std::string& part); // Валидация города, улицы или дома
};

// Дружественная функция перегрузки оператора вывода
std::ostream& operator<<(std::ostream& os, const Property& prop);

#endif // PROPERTY_H

