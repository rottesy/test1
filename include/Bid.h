#ifndef BID_H
#define BID_H

#include "Client.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Класс ставки в аукционе
class Bid {
private:
  std::string clientId;
  std::string clientName;
  double amount;
  std::string timestamp;

public:
  Bid(const std::string &clientId, const std::string &clientName,
      double amount);

  // Дружественная функция для вывода
  friend std::ostream &operator<<(std::ostream &os, const Bid &bid);

  // Перегрузка операций
  bool operator==(const Bid &other) const;
  bool operator<(const Bid &other) const;

  // Геттеры
  std::string getClientId() const { return clientId; }
  std::string getClientName() const { return clientName; }
  double getAmount() const { return amount; }
  std::string getTimestamp() const { return timestamp; }

  std::string toString() const;
  std::string toFileString() const;
};

std::ostream &operator<<(std::ostream &os, const Bid &bid);

#endif // BID_H
