#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User {
private:
    string idNumber;
    string name;
    string birthDate;
    int ticketsPurchased;
    int userCount;
    User* users;

public:
    User();
    User(string aIdNumber, string aName, string aBirthDate, int aTicketsPurchased);
    

    string getIdNumber();
    string getName();
    string getBirthDate();
    int getTicketsPurchased();

    void setIdNumber(string& aIdNumber);
    void setName(string& aName);
    void setBirthDate(string& aBirthDate);
    void setTicketsPurchased(int& quantity);

    void displayData();
    bool purchaseTickets(int& quantity);
    int getValidIntInput();
    void expandAndAssignUsers(User& users, int& quantity);
    void createUser(User& users);

    int getUserCount()  { return userCount; }
    User* getUsers()  { return users; }
    User* searchUserById(string& id);
};

#endif
