#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "Client.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>

class ClientManagerException : public std::exception {
private:
  std::string message;

public:
  ClientManagerException(const std::string &msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};

class ClientManager {
private:
  std::vector<std::shared_ptr<Client>> clients;
  std::string dataFilePath;

public:
  ClientManager();

  void addClient(std::shared_ptr<Client> client);
  bool removeClient(const std::string &id);
  Client *findClient(const std::string &id) const;

  std::vector<Client *> getAllClients() const;
  std::vector<Client *> searchByName(const std::string &name) const;
  std::vector<Client *> searchByPhone(const std::string &phone) const;

  void saveToFile(const std::string &filename) const;
  void loadFromFile(const std::string &filename);

  size_t getCount() const { return clients.size(); }
};

#endif // CLIENT_MANAGER_H
