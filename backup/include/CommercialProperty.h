#ifndef COMMERCIAL_PROPERTY_H
#define COMMERCIAL_PROPERTY_H

#include "Property.h"

// Класс коммерческой недвижимости (наследование от Property)
class CommercialProperty : public Property {
private:
    std::string businessType; // магазин, офис, склад и т.д.
    bool hasParking;
    int parkingSpaces;
    bool isVisibleFromStreet;

public:
    CommercialProperty(const std::string& id, const std::string& city, 
                       const std::string& street, const std::string& house,
                       double price, double area, const std::string& description,
                       const std::string& businessType, bool hasParking, 
                       int parkingSpaces, bool isVisibleFromStreet);
    
    // Переопределение виртуальных функций
    std::string getType() const override;
    void displayInfo() const override;
    std::string toFileString() const override;
    Property* clone() const override;

    // Специфичные геттеры
    std::string getBusinessType() const { return businessType; }
    bool getHasParking() const { return hasParking; }
    int getParkingSpaces() const { return parkingSpaces; }
    bool getIsVisibleFromStreet() const { return isVisibleFromStreet; }

    // Сеттеры с валидацией
    void setBusinessType(const std::string& type);
    void setParkingSpaces(int spaces);
};

#endif // COMMERCIAL_PROPERTY_H

