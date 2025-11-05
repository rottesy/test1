#include "../include/PropertyManager.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

PropertyManager::PropertyManager() : dataFilePath("data/properties.txt") {
}

void PropertyManager::addProperty(std::unique_ptr<Property> property) {
    if (!property) {
        throw PropertyManagerException("Cannot add null property");
    }
    if (findProperty(property->getId()) != nullptr) {
        throw PropertyManagerException("Property with ID " + property->getId() + " already exists");
    }
    properties.push_back(std::move(property));
}

void PropertyManager::addApartment(const std::string& id, const std::string& city, const std::string& street, const std::string& house,
                                   double price, double area, const std::string& description,
                                   int rooms, int floor, bool hasBalcony, bool hasElevator) {
    try {
        auto apartment = std::make_unique<Apartment>(id, city, street, house, price, area, description,
                                                     rooms, floor, hasBalcony, hasElevator);
        addProperty(std::move(apartment));
    } catch (const std::exception& e) {
        throw PropertyManagerException(std::string("Failed to add apartment: ") + e.what());
    }
}

void PropertyManager::addHouse(const std::string& id, const std::string& city,
                              const std::string& street, const std::string& house,
                              double price, double area, const std::string& description,
                              int floors, int rooms, double landArea, bool hasGarage, bool hasGarden) {
    try {
        auto houseObj = std::make_unique<House>(id, city, street, house, price, area, description,
                                            floors, rooms, landArea, hasGarage, hasGarden);
        addProperty(std::move(houseObj));
    } catch (const std::exception& e) {
        throw PropertyManagerException(std::string("Failed to add house: ") + e.what());
    }
}

void PropertyManager::addCommercialProperty(const std::string& id, const std::string& city, const std::string& street, const std::string& house,
                                            double price, double area, const std::string& description,
                                            const std::string& businessType, bool hasParking,
                                            int parkingSpaces, bool isVisibleFromStreet) {
    try {
        auto commercial = std::make_unique<CommercialProperty>(id, city, street, house, price, area, description,
                                                               businessType, hasParking, 
                                                               parkingSpaces, isVisibleFromStreet);
        addProperty(std::move(commercial));
    } catch (const std::exception& e) {
        throw PropertyManagerException(std::string("Failed to add commercial property: ") + e.what());
    }
}

bool PropertyManager::removeProperty(const std::string& id) {
    auto it = std::remove_if(properties.begin(), properties.end(),
        [&id](const std::unique_ptr<Property>& prop) {
            return prop->getId() == id;
        });
    
    if (it != properties.end()) {
        properties.erase(it, properties.end());
        return true;
    }
    return false;
}


Property* PropertyManager::findProperty(const std::string& id) const {
    auto it = std::find_if(properties.begin(), properties.end(),
        [&id](const std::unique_ptr<Property>& prop) {
            return prop->getId() == id;
        });
    
    return (it != properties.end()) ? it->get() : nullptr;
}

std::vector<Property*> PropertyManager::getAllProperties() const {
    std::vector<Property*> result;
    for (const auto& prop : properties) {
        result.push_back(prop.get());
    }
    return result;
}

std::vector<Property*> PropertyManager::getAvailableProperties() const {
    std::vector<Property*> result;
    for (const auto& prop : properties) {
        if (prop->getIsAvailable()) {
            result.push_back(prop.get());
        }
    }
    return result;
}

std::vector<Property*> PropertyManager::searchByPriceRange(double minPrice, double maxPrice) const {
    std::vector<Property*> result;
    for (const auto& prop : properties) {
        double price = prop->getPrice();
        if (price >= minPrice && price <= maxPrice) {
            result.push_back(prop.get());
        }
    }
    return result;
}

std::vector<Property*> PropertyManager::searchByAddress(const std::string& city, const std::string& street, const std::string& house) const {
    std::vector<Property*> result;
    
    std::string lowerCity = city;
    std::string lowerStreet = street;
    std::string lowerHouse = house;
    std::transform(lowerCity.begin(), lowerCity.end(), lowerCity.begin(), ::tolower);
    std::transform(lowerStreet.begin(), lowerStreet.end(), lowerStreet.begin(), ::tolower);
    std::transform(lowerHouse.begin(), lowerHouse.end(), lowerHouse.begin(), ::tolower);
    
    for (const auto& prop : properties) {
        std::string propCity = prop->getCity();
        std::string propStreet = prop->getStreet();
        std::string propHouse = prop->getHouse();
        
        std::transform(propCity.begin(), propCity.end(), propCity.begin(), ::tolower);
        std::transform(propStreet.begin(), propStreet.end(), propStreet.begin(), ::tolower);
        std::transform(propHouse.begin(), propHouse.end(), propHouse.begin(), ::tolower);
        
        bool matches = true;
        if (!lowerCity.empty() && propCity.find(lowerCity) == std::string::npos) {
            matches = false;
        }
        if (!lowerStreet.empty() && propStreet.find(lowerStreet) == std::string::npos) {
            matches = false;
        }
        if (!lowerHouse.empty() && propHouse.find(lowerHouse) == std::string::npos) {
            matches = false;
        }
        
        if (matches && (!lowerCity.empty() || !lowerStreet.empty() || !lowerHouse.empty())) {
            result.push_back(prop.get());
        }
    }
    return result;
}

void PropertyManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw PropertyManagerException("Cannot open file for writing: " + filename);
    }
    
    for (const auto& prop : properties) {
        file << prop->toFileString() << "\n";
    }
    file.close();
}

void PropertyManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return; // Файл не существует, это нормально
    }
    
    properties.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string type;
        std::getline(iss, type, '|');
        
        try {
            if (type == "APARTMENT") {
                std::string id, city, street, house, desc, avail;
                double price, area;
                int rooms, floor;
                bool balcony, elevator;
                
                std::getline(iss, id, '|');
                std::getline(iss, city, '|');
                std::getline(iss, street, '|');
                std::getline(iss, house, '|');
                iss >> price; iss.ignore();
                iss >> area; iss.ignore();
                std::getline(iss, desc, '|');
                std::getline(iss, avail, '|');
                iss >> rooms; iss.ignore();
                iss >> floor; iss.ignore();
                int b, e;
                iss >> b; iss.ignore();
                iss >> e;
                
                auto apartment = std::make_unique<Apartment>(id, city, street, house, price, area, desc,
                                                             rooms, floor, b == 1, e == 1);
                apartment->setAvailable(avail == "1");
                properties.push_back(std::move(apartment));
            } else if (type == "HOUSE") {
                std::string id, city, street, house, desc, avail;
                double price, area, landArea;
                int floors, rooms;
                bool garage, garden;
                
                std::getline(iss, id, '|');
                std::getline(iss, city, '|');
                std::getline(iss, street, '|');
                std::getline(iss, house, '|');
                iss >> price; iss.ignore();
                iss >> area; iss.ignore();
                std::getline(iss, desc, '|');
                std::getline(iss, avail, '|');
                iss >> floors; iss.ignore();
                iss >> rooms; iss.ignore();
                iss >> landArea; iss.ignore();
                int g, gar;
                iss >> g; iss.ignore();
                iss >> gar;
                
                auto houseObj = std::make_unique<House>(id, city, street, house, price, area, desc,
                                                    floors, rooms, landArea, g == 1, gar == 1);
                houseObj->setAvailable(avail == "1");
                properties.push_back(std::move(houseObj));
            } else if (type == "COMMERCIAL") {
                std::string id, city, street, house, desc, avail, businessType;
                double price, area;
                bool parking, visible;
                int parkingSpaces;
                
                std::getline(iss, id, '|');
                std::getline(iss, city, '|');
                std::getline(iss, street, '|');
                std::getline(iss, house, '|');
                iss >> price; iss.ignore();
                iss >> area; iss.ignore();
                std::getline(iss, desc, '|');
                std::getline(iss, avail, '|');
                std::getline(iss, businessType, '|');
                int p, v;
                iss >> p; iss.ignore();
                iss >> parkingSpaces; iss.ignore();
                iss >> v;
                
                auto commercial = std::make_unique<CommercialProperty>(id, city, street, house, price, area, desc,
                                                                      businessType, p == 1, 
                                                                      parkingSpaces, v == 1);
                commercial->setAvailable(avail == "1");
                properties.push_back(std::move(commercial));
            }
        } catch (const std::exception& e) {
            // Пропускаем некорректные записи
            continue;
        }
    }
    
    file.close();
}

