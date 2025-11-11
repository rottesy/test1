#ifndef APARTMENT_H
#define APARTMENT_H

#include "Property.h"

class Apartment : public Property {
private:
  int rooms;
  int floor;
  bool hasBalcony;
  bool hasElevator;

public:
  Apartment(const std::string &id, const std::string &city,
            const std::string &street, const std::string &house, double price,
            double area, const std::string &description, int rooms, int floor,
            bool hasBalcony, bool hasElevator);

  // Overridden methods
  std::string getType() const override;
  void displayInfo() const override;
  std::string toFileString() const override;
  Property *clone() const override;

  // Getters
  int getRooms() const { return rooms; }
  int getFloor() const { return floor; }
  bool getHasBalcony() const { return hasBalcony; }
  bool getHasElevator() const { return hasElevator; }

  // Setters
  void setRooms(int rooms);
  void setFloor(int floor);
};

#endif // APARTMENT_H
