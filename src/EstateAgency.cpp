#include "../include/EstateAgency.h"
#include <filesystem>

EstateAgency* EstateAgency::instance = nullptr;

EstateAgency::EstateAgency() : dataDirectory("data") {
    // Создаем директорию для данных, если она не существует
    std::filesystem::create_directories(dataDirectory);
}

EstateAgency::~EstateAgency() {
}

EstateAgency* EstateAgency::getInstance() {
    if (instance == nullptr) {
        instance = new EstateAgency();
    }
    return instance;
}

void EstateAgency::destroyInstance() {
    if (instance != nullptr) {
        instance->saveAllData();
        delete instance;
        instance = nullptr;
    }
}

void EstateAgency::saveAllData() const {
    try {
        propertyManager.saveToFile(dataDirectory + "/properties.txt");
        clientManager.saveToFile(dataDirectory + "/clients.txt");
        transactionManager.saveToFile(dataDirectory + "/transactions.txt");
        auctionManager.saveToFile(dataDirectory + "/auctions.txt");
    } catch (const std::exception& e) {
        // Логирование ошибок сохранения
    }
}

void EstateAgency::loadAllData() {
    try {
        propertyManager.loadFromFile(dataDirectory + "/properties.txt");
        clientManager.loadFromFile(dataDirectory + "/clients.txt");
        transactionManager.loadFromFile(dataDirectory + "/transactions.txt");
        auctionManager.loadFromFile(dataDirectory + "/auctions.txt");
    } catch (const std::exception& e) {
        // Логирование ошибок загрузки
    }
}

