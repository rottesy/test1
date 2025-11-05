#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <memory>
#include "Property.h"
#include "Client.h"

// Класс сделки
class Transaction {
private:
    std::string id;
    std::string propertyId;
    std::string clientId;
    std::string date;
    double finalPrice;
    std::string status; // "pending", "completed", "cancelled"
    std::string notes;

public:
    Transaction(const std::string& id, const std::string& propertyId, 
                const std::string& clientId, double finalPrice, 
                const std::string& status = "pending",
                const std::string& notes = "");

    // Дружественная функция для вывода
    friend std::ostream& operator<<(std::ostream& os, const Transaction& trans);

    // Перегрузка операций
    bool operator==(const Transaction& other) const;
    bool operator<(const Transaction& other) const;

    // Геттеры
    std::string getId() const { return id; }
    std::string getPropertyId() const { return propertyId; }
    std::string getClientId() const { return clientId; }
    std::string getDate() const { return date; }
    double getFinalPrice() const { return finalPrice; }
    std::string getStatus() const { return status; }
    std::string getNotes() const { return notes; }

    // Сеттеры
    void setStatus(const std::string& status);
    void setFinalPrice(double price);
    void setNotes(const std::string& notes);

    // Валидация
    static bool validateId(const std::string& id);

    std::string toString() const;
    std::string toFileString() const;
};

std::ostream& operator<<(std::ostream& os, const Transaction& trans);

#endif // TRANSACTION_H

