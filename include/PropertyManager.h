#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include "Apartment.h"
#include "CommercialProperty.h"
#include "House.h"
#include "Property.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>

class PropertyManagerException : public std::exception {
private:
  std::string message;

public:
  PropertyManagerException(const std::string &msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};

template <typename T> class ContainerManager {
protected:
  std::vector<std::shared_ptr<T>> items;

public:
  void add(std::shared_ptr<T> item) { items.push_back(item); }

  void remove(const std::string &id) {
    auto it = std::remove_if(
        items.begin(), items.end(),
        [&id](const std::shared_ptr<T> &item) { return item->getId() == id; });
    if (it != items.end()) {
      items.erase(it, items.end());
    }
  }

  std::shared_ptr<T> findById(const std::string &id) const {
    auto it = std::find_if(
        items.begin(), items.end(),
        [&id](const std::shared_ptr<T> &item) { return item->getId() == id; });
    return (it != items.end()) ? *it : nullptr;
  }

  size_t size() const { return items.size(); }

  typename std::vector<std::shared_ptr<T>>::const_iterator begin() const {
    return items.begin();
  }

  typename std::vector<std::shared_ptr<T>>::const_iterator end() const {
    return items.end();
  }
};

class PropertyManager {
private:
  std::vector<std::unique_ptr<Property>> properties;
  std::string dataFilePath;

public:
  PropertyManager();

  void addProperty(std::unique_ptr<Property> property);
  void addApartment(const std::string &id, const std::string &city,
                    const std::string &street, const std::string &house,
                    double price, double area, const std::string &description,
                    int rooms, int floor, bool hasBalcony, bool hasElevator);
  void addHouse(const std::string &id, const std::string &city,
                const std::string &street, const std::string &house,
                double price, double area, const std::string &description,
                int floors, int rooms, double landArea, bool hasGarage,
                bool hasGarden);
  void addCommercialProperty(const std::string &id, const std::string &city,
                             const std::string &street,
                             const std::string &house, double price,
                             double area, const std::string &description,
                             const std::string &businessType, bool hasParking,
                             int parkingSpaces, bool isVisibleFromStreet);

  bool removeProperty(const std::string &id);
  Property *findProperty(const std::string &id) const;

  std::vector<Property *> getAllProperties() const;
  std::vector<Property *> getAvailableProperties() const;
  std::vector<Property *> searchByPriceRange(double minPrice,
                                             double maxPrice) const;
  std::vector<Property *> searchByAddress(const std::string &city,
                                          const std::string &street = "",
                                          const std::string &house = "") const;

  void saveToFile(const std::string &filename) const;
  void loadFromFile(const std::string &filename);

  size_t getCount() const { return properties.size(); }
};

#endif // PROPERTY_MANAGER_H
