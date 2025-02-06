#ifndef USER_H
#define USER_H

#include <string>
#include <limits>
#include <iostream>
#include "List.h"

using namespace std;

class UserData {
private:
    string idNumber;
    string name;
    string birthDate;
    int ticketsPurchased = 0;

public:
    void setIdNumber(const string& id) { idNumber = id; }
    void setName(const string& n) { name = n; }
    void setBirthDate(const string& bd) { birthDate = bd; }

    string getIdNumber() const { return idNumber; }
    string getName() const { return name; }
    string getBirthDate() const { return birthDate; }

    bool purchaseTickets(int num) {
        ticketsPurchased += num;
        return true;
    }
    int getTicketsPurchased() const { return ticketsPurchased; }
};

class User {
private:
    List<UserData> users;
    int userCount;

    int getValidIntInput();

    void expandAndAssignUsers(User& usersObj, int quantity);

public:
    User();
    ~User();

    void createUser(User& usersObj);
    bool isValidDate(string& date);
    void createUser(User& usersObj, const string& idNumber);

    UserData* searchUserById(string& idNumber);

    List<UserData>& getUsers();
    int getUserCount() const { return userCount; }
};

#endif
