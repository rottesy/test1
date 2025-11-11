#include "../include/ClientManager.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace
{
constexpr char FILE_DELIMITER = '|';
}

ClientManager::ClientManager() : dataFilePath("data/clients.txt") {}

void ClientManager::addClient(std::shared_ptr<Client> client)
{
    if (!client)
    {
        throw ClientManagerException("Cannot add null client");
    }
    if (findClient(client->getId()) != nullptr)
    {
        throw ClientManagerException("Client with ID " + client->getId() + " already exists");
    }
    clients.push_back(client);
}

bool ClientManager::removeClient(const std::string &id)
{
    auto it = std::remove_if(clients.begin(), clients.end(),
                             [&id](const std::shared_ptr<Client> &client) { return client->getId() == id; });

    if (it != clients.end())
    {
        clients.erase(it, clients.end());
        return true;
    }
    return false;
}

Client *ClientManager::findClient(const std::string &id) const
{
    auto it = std::find_if(clients.begin(), clients.end(),
                           [&id](const std::shared_ptr<Client> &client) { return client->getId() == id; });

    return (it != clients.end()) ? it->get() : nullptr;
}

std::vector<Client *> ClientManager::getAllClients() const
{
    std::vector<Client *> result;
    result.reserve(clients.size());
    for (const auto &client : clients)
    {
        result.push_back(client.get());
    }
    return result;
}

std::vector<Client *> ClientManager::searchByName(const std::string &name) const
{
    std::vector<Client *> result;
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    for (const auto &client : clients)
    {
        std::string clientName = client->getName();
        std::transform(clientName.begin(), clientName.end(), clientName.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (clientName.find(lowerName) != std::string::npos)
        {
            result.push_back(client.get());
        }
    }
    return result;
}

std::vector<Client *> ClientManager::searchByPhone(const std::string &phone) const
{
    std::vector<Client *> result;
    for (const auto &client : clients)
    {
        if (client->getPhone() == phone)
        {
            result.push_back(client.get());
        }
    }
    return result;
}

void ClientManager::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw ClientManagerException("Cannot open file for writing: " + filename);
    }

    for (const auto &client : clients)
    {
        file << client->getId() << FILE_DELIMITER << client->getName() << FILE_DELIMITER << client->getPhone()
             << FILE_DELIMITER << client->getEmail() << FILE_DELIMITER << client->getRegistrationDate() << "\n";
    }
    file.close();
}

void ClientManager::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    clients.clear();
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream iss(line);
        std::string id, name, phone, email, regDate;

        std::getline(iss, id, FILE_DELIMITER);
        std::getline(iss, name, FILE_DELIMITER);
        std::getline(iss, phone, FILE_DELIMITER);
        std::getline(iss, email, FILE_DELIMITER);
        std::getline(iss, regDate, FILE_DELIMITER);

        try
        {
            auto client = std::make_shared<Client>(id, name, phone, email);
            clients.push_back(client);
        }
        catch (const std::exception &e)
        {
            continue;
        }
    }

    file.close();
}
