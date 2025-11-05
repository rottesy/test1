#ifndef AUCTION_H
#define AUCTION_H

#include "Bid.h"
#include "Property.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Класс аукциона
class Auction {
private:
  std::string id;
  std::string propertyId;
  std::string propertyAddress;
  double startingPrice;
  double buyoutPrice; // Цена автоматической покупки (startPrice * 1.7)
  std::vector<std::shared_ptr<Bid>> bids;
  std::string status; // "active", "completed", "cancelled"
  std::string createdAt;
  std::string completedAt;

public:
  Auction(const std::string &id, const std::string &propertyId,
          const std::string &propertyAddress, double startingPrice);

  // Дружественная функция для вывода
  friend std::ostream &operator<<(std::ostream &os, const Auction &auction);

  // Перегрузка операций
  bool operator==(const Auction &other) const;
  bool operator<(const Auction &other) const;

  // Добавление ставки (проверяет минимальный шаг и автоматическую покупку)
  bool addBid(std::shared_ptr<Bid> bid);

  // Прямое добавление ставки при загрузке (без проверок)
  void addBidDirect(std::shared_ptr<Bid> bid);

  // Получение текущей максимальной ставки
  double getCurrentHighestBid() const;
  Bid *getHighestBid() const;

  // Завершение аукциона
  void complete();
  void cancel();

  // Геттеры
  std::string getId() const { return id; }
  std::string getPropertyId() const { return propertyId; }
  std::string getPropertyAddress() const { return propertyAddress; }
  double getStartingPrice() const { return startingPrice; }
  double getBuyoutPrice() const { return buyoutPrice; }
  std::vector<std::shared_ptr<Bid>> getBids() const { return bids; }
  std::string getStatus() const { return status; }
  std::string getCreatedAt() const { return createdAt; }
  std::string getCompletedAt() const { return completedAt; }
  bool isActive() const { return status == "active"; }
  bool isCompleted() const { return status == "completed"; }

  // Проверка, была ли автоматическая покупка
  bool wasBuyout() const;

  std::string toString() const;
  std::string toFileString() const;
};

std::ostream &operator<<(std::ostream &os, const Auction &auction);

#endif // AUCTION_H
