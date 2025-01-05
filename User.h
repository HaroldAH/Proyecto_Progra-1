#pragma once
#include <string>

class User {
private:
    std::string idNumber;
    std::string name;
    std::string birthDate;
    int ticketsPurchased;

public:
    User(std::string idNumber, std::string name, std::string birthDate, int ticketsPurchased);
    User();

    std::string getIdNumber() const;
    std::string getName() const;
    std::string getBirthDate() const;
    int getTicketsPurchased() const;

    void setIdNumber(const std::string& idNumber);
    void setName(const std::string& name);
    void setBirthDate(const std::string& birthDate);
    void setTicketsPurchased(int quantity);

    void displayData() const;
    bool purchaseTickets(int quantity);
};
