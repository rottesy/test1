#include "../include/Auction.h"
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

Auction::Auction(const std::string& id, const std::string& propertyId, 
                 const std::string& propertyAddress, double startingPrice)
    : id(id), propertyId(propertyId), propertyAddress(propertyAddress), 
      startingPrice(startingPrice), status("active") {
    if (startingPrice <= 0) {
        throw std::invalid_argument("Starting price must be positive");
    }
    
    buyoutPrice = startingPrice * 1.7;
    
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    createdAt = oss.str();
    completedAt = "";
}

std::ostream& operator<<(std::ostream& os, const Auction& auction) {
    os << "Auction ID: " << auction.id << "\n"
       << "Property: " << auction.propertyAddress << " (ID: " << auction.propertyId << ")\n"
       << "Starting Price: " << std::fixed << std::setprecision(2) << auction.startingPrice << " руб.\n"
       << "Buyout Price: " << std::fixed << std::setprecision(2) << auction.buyoutPrice << " руб.\n"
       << "Status: " << auction.status << "\n"
       << "Bids Count: " << auction.bids.size() << "\n"
       << "Created: " << auction.createdAt;
    return os;
}

bool Auction::operator==(const Auction& other) const {
    return id == other.id;
}

bool Auction::operator<(const Auction& other) const {
    return createdAt < other.createdAt;
}

bool Auction::addBid(std::shared_ptr<Bid> bid) {
    if (status != "active") {
        return false; 
    }
    
    double currentHighest = getCurrentHighestBid();
    
  
    if (bid->getAmount() >= buyoutPrice) {
        bids.push_back(bid);
        complete(); 
        return true;
    }
    
    double minBid = (currentHighest > 0) ? currentHighest + 0.01 : startingPrice;
    if (bid->getAmount() < minBid) {
        return false; 
    }
    
    bids.push_back(bid);
    return true;
}

void Auction::addBidDirect(std::shared_ptr<Bid> bid) {
    bids.push_back(bid);
}

double Auction::getCurrentHighestBid() const {
    if (bids.empty()) {
        return 0.0;
    }
    
    double maxBid = 0.0;
    for (const auto& bid : bids) {
        if (bid->getAmount() > maxBid) {
            maxBid = bid->getAmount();
        }
    }
    return maxBid;
}

Bid* Auction::getHighestBid() const {
    if (bids.empty()) {
        return nullptr;
    }
    
    Bid* highest = nullptr;
    double maxBid = 0.0;
    for (const auto& bid : bids) {
        if (bid->getAmount() > maxBid) {
            maxBid = bid->getAmount();
            highest = bid.get();
        }
    }
    return highest;
}

void Auction::complete() {
    if (status == "active") {
        status = "completed";
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        completedAt = oss.str();
    }
}

void Auction::cancel() {
    if (status == "active") {
        status = "cancelled";
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        completedAt = oss.str();
    }
}

bool Auction::wasBuyout() const {
    if (!isCompleted() || bids.empty()) {
        return false;
    }
    
    Bid* highest = getHighestBid();
    return highest != nullptr && highest->getAmount() >= buyoutPrice;
}

std::string Auction::toString() const {
    std::ostringstream oss;
    oss << "Auction ID: " << id << ", "
        << "Property: " << propertyAddress << ", "
        << "Starting: " << std::fixed << std::setprecision(2) << startingPrice << " руб., "
        << "Status: " << status << ", "
        << "Bids: " << bids.size();
    return oss.str();
}

std::string Auction::toFileString() const {
    std::ostringstream oss;
    oss << id << "|" << propertyId << "|" << propertyAddress << "|"
        << std::fixed << std::setprecision(2) << startingPrice << "|"
        << buyoutPrice << "|" << status << "|" << createdAt << "|" << completedAt;
    return oss.str();
}

