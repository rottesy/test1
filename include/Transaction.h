#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
  std::string id;
  std::string propertyId;
  std::string clientId;
  std::string date;
  double finalPrice;
  std::string status;
  std::string notes;

public:
  Transaction(const std::string &id, const std::string &propertyId,
              const std::string &clientId, double finalPrice,
              const std::string &status = "pending",
              const std::string &notes = "");

  // Friend operators
  friend std::ostream &operator<<(std::ostream &os, const Transaction &trans);

  // Comparison operators
  bool operator==(const Transaction &other) const;
  bool operator<(const Transaction &other) const;

  // Getters
  std::string getId() const { return id; }
  std::string getPropertyId() const { return propertyId; }
  std::string getClientId() const { return clientId; }
  std::string getDate() const { return date; }
  double getFinalPrice() const { return finalPrice; }
  std::string getStatus() const { return status; }
  std::string getNotes() const { return notes; }

  // Setters
  void setStatus(const std::string &status);
  void setFinalPrice(double price);
  void setNotes(const std::string &notes);

  // Static validation methods
  static bool validateId(const std::string &id);

  // Utility methods
  std::string toString() const;
  std::string toFileString() const;
};

std::ostream &operator<<(std::ostream &os, const Transaction &trans);

#endif // TRANSACTION_H
