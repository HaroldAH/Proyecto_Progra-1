#ifndef USER_H
#define USER_H

#include <string>
#include <limits>
#include <iostream>


class UserData {
private:
    std::string idNumber;
    std::string name;
    std::string birthDate;
    int ticketsPurchased = 0;

public:
    
    void setIdNumber(const std::string &id) { idNumber = id; }
    void setName(const std::string &n) { name = n; }
    void setBirthDate(const std::string &bd) { birthDate = bd; }

    
    std::string getIdNumber() const { return idNumber; }
    std::string getName() const { return name; }
    std::string getBirthDate() const { return birthDate; }

    
    bool purchaseTickets(int num) {
        ticketsPurchased += num;
        return true; 
    }
    int getTicketsPurchased() const { return ticketsPurchased; }
};


class User {
private:
    UserData* users;     
    int userCount;       
    int capacity;        

    
    int getValidIntInput() {
        int val;
        while (true) {
            if (std::cin >> val) {
                return val;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida. Intente nuevamente: ";
        }
    }

    
    void expandAndAssignUsers(User &usersObj, int quantity);

public:
    User() : users(nullptr), userCount(0), capacity(0) {}
    ~User() { delete[] users; }

    
    void createUser(User &usersObj);

    
    void createUser(User &usersObj, const std::string &idNumber);

    
    UserData* searchUserById(std::string &idNumber);

    
    UserData* getUsers() const { return users; }
    int getUserCount() const { return userCount; }
};

#endif
