#include "../include/Apartment.h"
#include <stdexcept>
#include <sstream>

Apartment::Apartment(const std::string& id, const std::string& city, 
                     const std::string& street, const std::string& house,
                     double price, double area, const std::string& description,
                     int rooms, int floor, bool hasBalcony, bool hasElevator)
    : Property(id, city, street, house, price, area, description),
      rooms(rooms), floor(floor), hasBalcony(hasBalcony), hasElevator(hasElevator) {
    if (rooms < 1 || rooms > 10) {
        throw std::invalid_argument("Number of rooms must be between 1 and 10");
    }
    if (floor < 1 || floor > 100) {
        throw std::invalid_argument("Floor must be between 1 and 100");
    }
}

std::string Apartment::getType() const {
    return "Apartment";
}

void Apartment::displayInfo() const {
    std::cout << *this << "\n";
    std::cout << "Rooms: " << rooms << "\n";
    std::cout << "Floor: " << floor << "\n";
    std::cout << "Balcony: " << (hasBalcony ? "Yes" : "No") << "\n";
    std::cout << "Elevator: " << (hasElevator ? "Yes" : "No") << "\n";
}

std::string Apartment::toFileString() const {
    std::ostringstream oss;
    oss << "APARTMENT|"
        << id << "|" << city << "|" << street << "|" << house << "|" << price << "|" << area << "|"
        << description << "|" << (isAvailable ? "1" : "0") << "|"
        << rooms << "|" << floor << "|" << (hasBalcony ? "1" : "0") << "|"
        << (hasElevator ? "1" : "0");
    return oss.str();
}

Property* Apartment::clone() const {
    return new Apartment(id, city, street, house, price, area, description,
                        rooms, floor, hasBalcony, hasElevator);
}

void Apartment::setRooms(int rooms) {
    if (rooms < 1 || rooms > 10) {
        throw std::invalid_argument("Number of rooms must be between 1 and 10");
    }
    this->rooms = rooms;
}

void Apartment::setFloor(int floor) {
    if (floor < 1 || floor > 100) {
        throw std::invalid_argument("Floor must be between 1 and 100");
    }
    this->floor = floor;
}

