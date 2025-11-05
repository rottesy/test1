#include "../include/AuctionManager.h"
#include "../include/Bid.h"
#include <sstream>
#include <algorithm>
#include <fstream>

AuctionManager::AuctionManager() : dataFilePath("data/auctions.txt") {
}

void AuctionManager::addAuction(std::shared_ptr<Auction> auction) {
    if (!auction) {
        throw AuctionManagerException("Cannot add null auction");
    }
    if (findAuction(auction->getId()) != nullptr) {
        throw AuctionManagerException("Auction with ID " + auction->getId() + " already exists");
    }
    auctions.push_back(auction);
}

bool AuctionManager::removeAuction(const std::string& id) {
    auto it = std::remove_if(auctions.begin(), auctions.end(),
        [&id](const std::shared_ptr<Auction>& auction) {
            return auction->getId() == id;
        });
    
    if (it != auctions.end()) {
        auctions.erase(it, auctions.end());
        return true;
    }
    return false;
}

Auction* AuctionManager::findAuction(const std::string& id) const {
    auto it = std::find_if(auctions.begin(), auctions.end(),
        [&id](const std::shared_ptr<Auction>& auction) {
            return auction->getId() == id;
        });
    
    return (it != auctions.end()) ? it->get() : nullptr;
}

std::vector<Auction*> AuctionManager::getAllAuctions() const {
    std::vector<Auction*> result;
    for (const auto& auction : auctions) {
        result.push_back(auction.get());
    }
    return result;
}

std::vector<Auction*> AuctionManager::getActiveAuctions() const {
    std::vector<Auction*> result;
    for (const auto& auction : auctions) {
        if (auction->isActive()) {
            result.push_back(auction.get());
        }
    }
    return result;
}

std::vector<Auction*> AuctionManager::getCompletedAuctions() const {
    std::vector<Auction*> result;
    for (const auto& auction : auctions) {
        if (auction->isCompleted()) {
            result.push_back(auction.get());
        }
    }
    return result;
}

std::vector<Auction*> AuctionManager::getAuctionsByProperty(const std::string& propertyId) const {
    std::vector<Auction*> result;
    for (const auto& auction : auctions) {
        if (auction->getPropertyId() == propertyId) {
            result.push_back(auction.get());
        }
    }
    return result;
}

void AuctionManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw AuctionManagerException("Cannot open file for writing: " + filename);
    }
    
    for (const auto& auction : auctions) {
        file << auction->toFileString() << "\n";
        
        // Сохраняем ставки
        auto bids = auction->getBids();
        for (const auto& bid : bids) {
            file << "BID|" << auction->getId() << "|" << bid->toFileString() << "\n";
        }
    }
    file.close();
}

void AuctionManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    
    auctions.clear();
    std::string line;
    Auction* currentAuction = nullptr;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        if (line.length() >= 4 && line.substr(0, 4) == "BID|") {
            // Загрузка ставки
            if (currentAuction == nullptr) continue;
            
            try {
                std::istringstream iss(line.substr(4)); // Пропускаем "BID|"
                std::string auctionId, clientId, clientName, amountStr, timestamp;
                
                if (!std::getline(iss, auctionId, '|') || auctionId.empty()) continue;
                if (!std::getline(iss, clientId, '|') || clientId.empty()) continue;
                if (!std::getline(iss, clientName, '|')) continue; // clientName может быть пустым
                if (!std::getline(iss, amountStr, '|') || amountStr.empty()) continue;
                std::getline(iss, timestamp, '|'); // timestamp опционален
                
                double amount = std::stod(amountStr);
                if (amount <= 0) continue; // Валидация суммы
                
                auto bid = std::make_shared<Bid>(clientId, clientName, amount);
                // При загрузке игнорируем проверки минимального шага и статуса
                if (currentAuction) {
                    currentAuction->addBidDirect(bid);
                }
            } catch (const std::exception& e) {
                // Игнорируем ошибки парсинга отдельных ставок
                continue;
            }
        } else {
            // Загрузка аукциона
            std::istringstream iss(line);
            std::string id, propertyId, propertyAddress, startingPriceStr, buyoutPriceStr, 
                        status, createdAt, completedAt;
            
            std::getline(iss, id, '|');
            std::getline(iss, propertyId, '|');
            std::getline(iss, propertyAddress, '|');
            std::getline(iss, startingPriceStr, '|');
            std::getline(iss, buyoutPriceStr, '|');
            std::getline(iss, status, '|');
            std::getline(iss, createdAt, '|');
            std::getline(iss, completedAt, '|');
            
            try {
                double startingPrice = std::stod(startingPriceStr);
                auto auction = std::make_shared<Auction>(id, propertyId, propertyAddress, startingPrice);
                // Восстанавливаем статус и даты
                if (status == "completed") {
                    auction->complete();
                } else if (status == "cancelled") {
                    auction->cancel();
                }
                auctions.push_back(auction);
                currentAuction = auction.get();
            } catch (const std::exception& e) {
                continue;
            }
        }
    }
    
    file.close();
}

