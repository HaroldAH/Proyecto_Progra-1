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

    do {
        cout << "Ingrese la fecha de nacimiento del usuario "<< usersObj.userCount + 1 << " (DD/MM/AAAA): ";
        getline(cin, birthDate);
            if (!isValidDate(birthDate)) {
                cout << "Fecha invalida. Intente nuevamente." << endl;
            }
    } while (!isValidDate(birthDate));

    
    usersObj.users[usersObj.userCount].setBirthDate(birthDate);

    
    usersObj.userCount++;
}

bool User::isValidDate(string &birthDate) {
    
    if (birthDate.length() != 10) return false;

    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5) && birthDate[i] != '/') return false;
        if (i != 2 && i != 5 && (birthDate[i] < '0' || birthDate[i] > '9')) return false;
    }

    int day = (birthDate[0] - '0') * 10 + (birthDate[1] - '0');
    int month = (birthDate[3] - '0') * 10 + (birthDate[4] - '0');

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}

UserData* User::searchUserById(string &idNumber) {
    
    for (int i = 0; i < userCount; i++) {
        if (users[i].getIdNumber() == idNumber) {
            return &users[i];
        }
    }
    return nullptr;
}
