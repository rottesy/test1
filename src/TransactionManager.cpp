#include "../include/TransactionManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace {
constexpr char FILE_DELIMITER = '|';
}

TransactionManager::TransactionManager()
    : dataFilePath("data/transactions.txt") {}

void TransactionManager::addTransaction(
    std::shared_ptr<Transaction> transaction) {
  if (!transaction) {
    throw TransactionManagerException("Cannot add null transaction");
  }
  if (findTransaction(transaction->getId()) != nullptr) {
    throw TransactionManagerException("Transaction with ID " +
                                      transaction->getId() + " already exists");
  }
  transactions.push_back(transaction);
}

bool TransactionManager::removeTransaction(const std::string &id) {
  auto it = std::remove_if(transactions.begin(), transactions.end(),
                           [&id](const std::shared_ptr<Transaction> &trans) {
                             return trans->getId() == id;
                           });

  if (it != transactions.end()) {
    transactions.erase(it, transactions.end());
    return true;
  }
  return false;
}

Transaction *TransactionManager::findTransaction(const std::string &id) const {
  auto it = std::find_if(transactions.begin(), transactions.end(),
                         [&id](const std::shared_ptr<Transaction> &trans) {
                           return trans->getId() == id;
                         });

  return (it != transactions.end()) ? it->get() : nullptr;
}

std::vector<Transaction *> TransactionManager::getAllTransactions() const {
  std::vector<Transaction *> result;
  result.reserve(transactions.size());
  for (const auto &trans : transactions) {
    result.push_back(trans.get());
  }
  return result;
}

std::vector<Transaction *>
TransactionManager::getTransactionsByClient(const std::string &clientId) const {
  std::vector<Transaction *> result;
  for (const auto &trans : transactions) {
    if (trans->getClientId() == clientId) {
      result.push_back(trans.get());
    }
  }
  return result;
}

std::vector<Transaction *> TransactionManager::getTransactionsByProperty(
    const std::string &propertyId) const {
  std::vector<Transaction *> result;
  for (const auto &trans : transactions) {
    if (trans->getPropertyId() == propertyId) {
      result.push_back(trans.get());
    }
  }
  return result;
}

std::vector<Transaction *>
TransactionManager::getTransactionsByStatus(const std::string &status) const {
  std::vector<Transaction *> result;
  for (const auto &trans : transactions) {
    if (trans->getStatus() == status) {
      result.push_back(trans.get());
    }
  }
  return result;
}

void TransactionManager::saveToFile(const std::string &filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw TransactionManagerException("Cannot open file for writing: " +
                                      filename);
  }

  for (const auto &trans : transactions) {
    file << trans->toFileString() << "\n";
  }
  file.close();
}

void TransactionManager::loadFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return;
  }

  transactions.clear();
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string id, propertyId, clientId, date, status, notes;
    double finalPrice;

    std::getline(iss, id, FILE_DELIMITER);
    std::getline(iss, propertyId, FILE_DELIMITER);
    std::getline(iss, clientId, FILE_DELIMITER);
    std::getline(iss, date, FILE_DELIMITER);
    iss >> finalPrice;
    iss.ignore();
    std::getline(iss, status, FILE_DELIMITER);
    std::getline(iss, notes, FILE_DELIMITER);

    try {
      auto transaction = std::make_shared<Transaction>(
          id, propertyId, clientId, finalPrice, status, notes);
      transactions.push_back(transaction);
    } catch (const std::exception &e) {
      continue;
    }
  }

  file.close();
}
