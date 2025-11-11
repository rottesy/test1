#ifndef ESTATE_AGENCY_H
#define ESTATE_AGENCY_H

#include "AuctionManager.h"
#include "ClientManager.h"
#include "PropertyManager.h"
#include "TransactionManager.h"
#include <string>

class EstateAgency
{
  private:
    static EstateAgency *instance;
    PropertyManager propertyManager;
    ClientManager clientManager;
    TransactionManager transactionManager;
    AuctionManager auctionManager;
    std::string dataDirectory;

    EstateAgency();

  public:
    ~EstateAgency();

    // Singleton methods
    static EstateAgency *getInstance();
    static void destroyInstance();

    // Manager accessors
    PropertyManager &getPropertyManager() { return propertyManager; }
    ClientManager &getClientManager() { return clientManager; }
    TransactionManager &getTransactionManager() { return transactionManager; }
    AuctionManager &getAuctionManager() { return auctionManager; }

    // Data operations
    void saveAllData() const;
    void loadAllData();

    // Configuration
    void setDataDirectory(const std::string &dir) { dataDirectory = dir; }
    std::string getDataDirectory() const { return dataDirectory; }
};

#endif // ESTATE_AGENCY_H
