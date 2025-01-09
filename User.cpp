#include "User.h"

void User::expandAndAssignUsers(User &usersObj, int quantity) {
    
    if (usersObj.userCount + quantity > usersObj.capacity) {
        
        int newCap = (usersObj.capacity == 0) ? quantity : usersObj.capacity + quantity;

        UserData* newArr = new UserData[newCap];
        
        for (int i = 0; i < usersObj.userCount; i++) {
            newArr[i] = usersObj.users[i];
        }
        
        delete[] usersObj.users;
        usersObj.users = newArr;
        usersObj.capacity = newCap;
    }
}

void User::createUser(User &usersObj, const string &idNumber) {
    
    expandAndAssignUsers(usersObj, 1);

    
    usersObj.users[usersObj.userCount].setIdNumber(idNumber);

    
    string name, birthDate;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese el nombre del usuario " << usersObj.userCount + 1 << ": ";
    getline(cin, name);
    usersObj.users[usersObj.userCount].setName(name);

    cout << "Ingrese la fecha de nacimiento del usuario " 
              << usersObj.userCount + 1 << " (DD/MM/AAAA): ";
    getline(cin, birthDate);
    usersObj.users[usersObj.userCount].setBirthDate(birthDate);

    
    usersObj.userCount++;
}

UserData* User::searchUserById(string &idNumber) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].getIdNumber() == idNumber) {
            return &users[i];
        }
    }
    return nullptr;
}
