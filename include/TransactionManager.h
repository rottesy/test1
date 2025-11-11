#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include "Transaction.h"
#include <memory>
#include <string>
#include <vector>

class TransactionManagerException : public std::exception {
private:
  std::string message;

public:
  explicit TransactionManagerException(const std::string &msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};

class TransactionManager {
private:
  std::vector<std::shared_ptr<Transaction>> transactions;
  std::string dataFilePath;

public:
  TransactionManager();

  // Transaction management methods
  void addTransaction(std::shared_ptr<Transaction> transaction);
  bool removeTransaction(const std::string &id);
  Transaction *findTransaction(const std::string &id) const;

  // Query methods
  std::vector<Transaction *> getAllTransactions() const;
  std::vector<Transaction *>
  getTransactionsByClient(const std::string &clientId) const;
  std::vector<Transaction *>
  getTransactionsByProperty(const std::string &propertyId) const;
  std::vector<Transaction *>
  getTransactionsByStatus(const std::string &status) const;

  // File operations
  void saveToFile(const std::string &filename) const;
  void loadFromFile(const std::string &filename);

  // Utility methods
  size_t getCount() const { return transactions.size(); }
};

#endif // TRANSACTION_MANAGER_H
