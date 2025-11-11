#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

class Client {
private:
  std::string id;
  std::string name;
  std::string phone;
  std::string email;
  std::string registrationDate;

public:
  Client(const std::string &id, const std::string &name,
         const std::string &phone, const std::string &email);

  // Friend operators
  friend std::ostream &operator<<(std::ostream &os, const Client &client);

  // Comparison operators
  bool operator==(const Client &other) const;
  bool operator<(const Client &other) const;

  // Getters
  std::string getId() const { return id; }
  std::string getName() const { return name; }
  std::string getPhone() const { return phone; }
  std::string getEmail() const { return email; }
  std::string getRegistrationDate() const { return registrationDate; }

  // Setters
  void setName(const std::string &name);
  void setPhone(const std::string &phone);
  void setEmail(const std::string &email);

  // Static validation methods
  static bool validateId(const std::string &id);
  static bool validatePhone(const std::string &phone);
  static bool validateEmail(const std::string &email);

  // Utility methods
  std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const Client &client);

#endif // CLIENT_H
