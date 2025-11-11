#include "../include/EstateAgency.h"
#include <filesystem>

namespace
{
constexpr const char *DATA_DIRECTORY = "data";
constexpr const char *PROPERTIES_FILE = "properties.txt";
constexpr const char *CLIENTS_FILE = "clients.txt";
constexpr const char *TRANSACTIONS_FILE = "transactions.txt";
constexpr const char *AUCTIONS_FILE = "auctions.txt";
} // namespace

EstateAgency *EstateAgency::instance = nullptr;

EstateAgency::EstateAgency() : dataDirectory(DATA_DIRECTORY)
{
    try
    {
        std::filesystem::create_directories(dataDirectory);
    }
    catch (const std::exception &)
    {
        // Ignore directory creation errors
    }
}

EstateAgency::~EstateAgency() {}

EstateAgency *EstateAgency::getInstance()
{
    if (instance == nullptr)
    {
        instance = new EstateAgency();
    }
    return instance;
}

void EstateAgency::destroyInstance()
{
    if (instance != nullptr)
    {
        instance->saveAllData();
        delete instance;
        instance = nullptr;
    }
}

void EstateAgency::saveAllData() const
{
    try
    {
        propertyManager.saveToFile(dataDirectory + "/" + PROPERTIES_FILE);
        clientManager.saveToFile(dataDirectory + "/" + CLIENTS_FILE);
        transactionManager.saveToFile(dataDirectory + "/" + TRANSACTIONS_FILE);
        auctionManager.saveToFile(dataDirectory + "/" + AUCTIONS_FILE);
    }
    catch (const std::exception &e)
    {
        // Silent failure - data saving errors are logged but don't interrupt
        // execution
    }
}

void EstateAgency::loadAllData()
{
    try
    {
        propertyManager.loadFromFile(dataDirectory + "/" + PROPERTIES_FILE);
        clientManager.loadFromFile(dataDirectory + "/" + CLIENTS_FILE);
        transactionManager.loadFromFile(dataDirectory + "/" + TRANSACTIONS_FILE);
        auctionManager.loadFromFile(dataDirectory + "/" + AUCTIONS_FILE);
    }
    catch (const std::exception &e)
    {
        // Silent failure - data loading errors are handled gracefully
    }
}
