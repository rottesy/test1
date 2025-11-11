#include "../include/AuctionManager.h"
#include "../include/Bid.h"
#include "../include/Constants.h"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace
{
constexpr char FILE_DELIMITER = '|';
constexpr size_t BID_PREFIX_LENGTH = 4;
} // namespace

AuctionManager::AuctionManager() : dataFilePath("data/auctions.txt") {}

void AuctionManager::addAuction(std::shared_ptr<Auction> auction)
{
    if (!auction)
    {
        throw AuctionManagerException("Cannot add null auction");
    }
    if (findAuction(auction->getId()) != nullptr)
    {
        throw AuctionManagerException("Auction with ID " + auction->getId() + " already exists");
    }
    auctions.push_back(auction);
}

bool AuctionManager::removeAuction(const std::string &id)
{
    auto it = std::remove_if(auctions.begin(), auctions.end(),
                             [&id](const std::shared_ptr<Auction> &auction) { return auction->getId() == id; });

    if (it != auctions.end())
    {
        auctions.erase(it, auctions.end());
        return true;
    }
    return false;
}

Auction *AuctionManager::findAuction(const std::string &id) const
{
    auto it = std::find_if(auctions.begin(), auctions.end(),
                           [&id](const std::shared_ptr<Auction> &auction) { return auction->getId() == id; });

    return (it != auctions.end()) ? it->get() : nullptr;
}

std::vector<Auction *> AuctionManager::getAllAuctions() const
{
    std::vector<Auction *> result;
    result.reserve(auctions.size());
    for (const auto &auction : auctions)
    {
        result.push_back(auction.get());
    }
    return result;
}

std::vector<Auction *> AuctionManager::getActiveAuctions() const
{
    std::vector<Auction *> result;
    for (const auto &auction : auctions)
    {
        if (auction->isActive())
        {
            result.push_back(auction.get());
        }
    }
    return result;
}

std::vector<Auction *> AuctionManager::getCompletedAuctions() const
{
    std::vector<Auction *> result;
    for (const auto &auction : auctions)
    {
        if (auction->isCompleted())
        {
            result.push_back(auction.get());
        }
    }
    return result;
}

std::vector<Auction *> AuctionManager::getAuctionsByProperty(const std::string &propertyId) const
{
    std::vector<Auction *> result;
    for (const auto &auction : auctions)
    {
        if (auction->getPropertyId() == propertyId)
        {
            result.push_back(auction.get());
        }
    }
    return result;
}

void AuctionManager::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw AuctionManagerException("Cannot open file for writing: " + filename);
    }

    for (const auto &auction : auctions)
    {
        file << auction->toFileString() << "\n";

        auto bids = auction->getBids();
        for (const auto &bid : bids)
        {
            file << "BID|" << auction->getId() << FILE_DELIMITER << bid->toFileString() << "\n";
        }
    }
    file.close();
}

void AuctionManager::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    auctions.clear();
    std::string line;
    Auction *currentAuction = nullptr;
    const std::string BID_PREFIX = "BID|";

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        if (line.length() >= BID_PREFIX_LENGTH && line.substr(0, BID_PREFIX_LENGTH) == BID_PREFIX)
        {
            if (currentAuction == nullptr)
            {
                continue;
            }

            try
            {
                std::istringstream iss(line.substr(BID_PREFIX_LENGTH));
                std::string auctionId, clientId, clientName, amountStr, timestamp;

                if (!std::getline(iss, auctionId, FILE_DELIMITER) || auctionId.empty())
                {
                    continue;
                }
                if (!std::getline(iss, clientId, FILE_DELIMITER) || clientId.empty())
                {
                    continue;
                }
                if (!std::getline(iss, clientName, FILE_DELIMITER))
                {
                    continue;
                }
                if (!std::getline(iss, amountStr, FILE_DELIMITER) || amountStr.empty())
                {
                    continue;
                }
                std::getline(iss, timestamp, FILE_DELIMITER);

                double amount = std::stod(amountStr);
                if (amount <= 0.0)
                {
                    continue;
                }

                auto bid = std::make_shared<Bid>(clientId, clientName, amount);
                currentAuction->addBidDirect(bid);
            }
            catch (const std::exception &e)
            {
                continue;
            }
        }
        else
        {
            std::istringstream iss(line);
            std::string id, propertyId, propertyAddress, startingPriceStr, buyoutPriceStr, status, createdAt,
                completedAt;

            std::getline(iss, id, FILE_DELIMITER);
            std::getline(iss, propertyId, FILE_DELIMITER);
            std::getline(iss, propertyAddress, FILE_DELIMITER);
            std::getline(iss, startingPriceStr, FILE_DELIMITER);
            std::getline(iss, buyoutPriceStr, FILE_DELIMITER);
            std::getline(iss, status, FILE_DELIMITER);
            std::getline(iss, createdAt, FILE_DELIMITER);
            std::getline(iss, completedAt, FILE_DELIMITER);

            try
            {
                double startingPrice = std::stod(startingPriceStr);
                auto auction = std::make_shared<Auction>(id, propertyId, propertyAddress, startingPrice);

                if (status == Constants::AuctionStatus::COMPLETED)
                {
                    auction->complete();
                }
                else if (status == Constants::AuctionStatus::CANCELLED)
                {
                    auction->cancel();
                }
                auctions.push_back(auction);
                currentAuction = auction.get();
            }
            catch (const std::exception &e)
            {
                continue;
            }
        }
    }

    file.close();
}
