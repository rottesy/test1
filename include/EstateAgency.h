#ifndef ESTATE_AGENCY_H
#define ESTATE_AGENCY_H

#include "AuctionManager.h"
#include "ClientManager.h"
#include "PropertyManager.h"
#include "TransactionManager.h"
#include <string>

// Главный класс агентства недвижимости (Singleton паттерн)
class EstateAgency {
private:
  static EstateAgency *instance;
  PropertyManager propertyManager;
  ClientManager clientManager;
  TransactionManager transactionManager;
  AuctionManager auctionManager;
  std::string dataDirectory;

  EstateAgency(); // Приватный конструктор для Singleton

public:
  ~EstateAgency();

  // Singleton метод
  static EstateAgency *getInstance();
  static void destroyInstance();

  // Доступ к менеджерам
  PropertyManager &getPropertyManager() { return propertyManager; }
  ClientManager &getClientManager() { return clientManager; }
  TransactionManager &getTransactionManager() { return transactionManager; }
  AuctionManager &getAuctionManager() { return auctionManager; }

  // Сохранение и загрузка всех данных
  void saveAllData() const;
  void loadAllData();
  void setDataDirectory(const std::string &dir) { dataDirectory = dir; }
  std::string getDataDirectory() const { return dataDirectory; }
};

#endif // ESTATE_AGENCY_H
