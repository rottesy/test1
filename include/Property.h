#ifndef PROPERTY_H
#define PROPERTY_H

#include <iostream>
#include <string>

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
  Property(const std::string &id, const std::string &city,
           const std::string &street, const std::string &house, double price,
           double area, const std::string &description);
  virtual ~Property() = default;

  // Pure virtual methods
  virtual std::string getType() const = 0;
  virtual void displayInfo() const = 0;
  virtual std::string toFileString() const = 0;
  virtual Property *clone() const = 0;

  // Friend operators
  friend std::ostream &operator<<(std::ostream &os, const Property &prop);

  // Comparison operators
  bool operator==(const Property &other) const;
  bool operator!=(const Property &other) const;
  bool operator<(const Property &other) const;
  bool operator>(const Property &other) const;

  // Getters
  std::string getId() const { return id; }
  std::string getCity() const { return city; }
  std::string getStreet() const { return street; }
  std::string getHouse() const { return house; }
  std::string getAddress() const { return city + ", " + street + ", " + house; }
  double getPrice() const { return price; }
  double getArea() const { return area; }
  std::string getDescription() const { return description; }
  bool getIsAvailable() const { return isAvailable; }

  // Setters
  void setPrice(double newPrice);
  void setArea(double newArea);
  void setAddress(const std::string &city, const std::string &street,
                  const std::string &house);
  void setDescription(const std::string &newDesc);
  void setAvailable(bool available) { isAvailable = available; }

  // Static validation methods
  static bool validatePrice(double price);
  static bool validateArea(double area);
  static bool validateId(const std::string &id);
  static bool validateAddressPart(const std::string &part);
};

std::ostream &operator<<(std::ostream &os, const Property &prop);

#endif // PROPERTY_H
