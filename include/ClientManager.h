#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "Client.h"
#include <memory>
#include <string>
#include <vector>

class ClientManagerException : public std::exception {
private:
  std::string message;

public:
  explicit ClientManagerException(const std::string &msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};

class ClientManager {
private:
  std::vector<std::shared_ptr<Client>> clients;
  std::string dataFilePath;

public:
  ClientManager();

  // Client management methods
  void addClient(std::shared_ptr<Client> client);
  bool removeClient(const std::string &id);
  Client *findClient(const std::string &id) const;

  // Query methods
  std::vector<Client *> getAllClients() const;
  std::vector<Client *> searchByName(const std::string &name) const;
  std::vector<Client *> searchByPhone(const std::string &phone) const;

  // File operations
  void saveToFile(const std::string &filename) const;
  void loadFromFile(const std::string &filename);

  // Utility methods
  size_t getCount() const { return clients.size(); }
};

#endif // CLIENT_MANAGER_H
