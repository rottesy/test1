#include "../include/House.h"
#include <stdexcept>
#include <sstream>

House::House(const std::string& id, const std::string& city, 
             const std::string& street, const std::string& house,
             double price, double area, const std::string& description,
             int floors, int rooms, double landArea, bool hasGarage, bool hasGarden)
    : Property(id, city, street, house, price, area, description),
      floors(floors), rooms(rooms), landArea(landArea), 
      hasGarage(hasGarage), hasGarden(hasGarden) {
    if (floors < 1 || floors > 10) {
        throw std::invalid_argument("Number of floors must be between 1 and 10");
    }
    if (rooms < 1 || rooms > 50) {
        throw std::invalid_argument("Number of rooms must be between 1 and 50");
    }
    if (landArea < 0 || landArea > 10000) {
        throw std::invalid_argument("Land area must be between 0 and 10000 m²");
    }
}

std::string House::getType() const {
    return "House";
}

void House::displayInfo() const {
    std::cout << *this << "\n";
    std::cout << "Floors: " << floors << "\n";
    std::cout << "Rooms: " << rooms << "\n";
    std::cout << "Land Area: " << landArea << " м²\n";
    std::cout << "Garage: " << (hasGarage ? "Yes" : "No") << "\n";
    std::cout << "Garden: " << (hasGarden ? "Yes" : "No") << "\n";
}

std::string House::toFileString() const {
    std::ostringstream oss;
    oss << "HOUSE|"
        << id << "|" << city << "|" << street << "|" << house << "|" << price << "|" << area << "|"
        << description << "|" << (isAvailable ? "1" : "0") << "|"
        << floors << "|" << rooms << "|" << landArea << "|"
        << (hasGarage ? "1" : "0") << "|" << (hasGarden ? "1" : "0");
    return oss.str();
}

Property* House::clone() const {
    return new House(id, city, street, house, price, area, description,
                    floors, rooms, landArea, hasGarage, hasGarden);
}

void House::setFloors(int floors) {
    if (floors < 1 || floors > 10) {
        throw std::invalid_argument("Number of floors must be between 1 and 10");
    }
    this->floors = floors;
}

void House::setRooms(int rooms) {
    if (rooms < 1 || rooms > 50) {
        throw std::invalid_argument("Number of rooms must be between 1 and 50");
    }
    this->rooms = rooms;
}

void House::setLandArea(double landArea) {
    if (landArea < 0 || landArea > 10000) {
        throw std::invalid_argument("Land area must be between 0 and 10000 m²");
    }
    this->landArea = landArea;
}

