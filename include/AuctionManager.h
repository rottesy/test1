#ifndef AUCTION_MANAGER_H
#define AUCTION_MANAGER_H

#include "Auction.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>

// Исключение для менеджера аукционов
class AuctionManagerException : public std::exception {
private:
  std::string message;

public:
  AuctionManagerException(const std::string &msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};

// Менеджер аукционов
class AuctionManager {
private:
  std::vector<std::shared_ptr<Auction>> auctions;
  std::string dataFilePath;

public:
  AuctionManager();

  void addAuction(std::shared_ptr<Auction> auction);
  bool removeAuction(const std::string &id);
  Auction *findAuction(const std::string &id) const;

  std::vector<Auction *> getAllAuctions() const;
  std::vector<Auction *> getActiveAuctions() const;
  std::vector<Auction *> getCompletedAuctions() const;
  std::vector<Auction *>
  getAuctionsByProperty(const std::string &propertyId) const;

  void saveToFile(const std::string &filename) const;
  void loadFromFile(const std::string &filename);

  size_t getCount() const { return auctions.size(); }
};

#endif // AUCTION_MANAGER_H
