#include "User.h"


User::User() {
    users = nullptr;
    userCount = 0;
    capacity = 0;
}

User::~User() {
    delete[] users;
}

void User::expandAndAssignUsers(User &users, int quantity) {
    
    if (users.userCount + quantity > users.capacity) {
        
        int newCap = (users.capacity == 0) ? quantity : users.capacity + quantity;

        UserData* newArr = new UserData[newCap];
        
        for (int i = 0; i < users.userCount; i++) {
            newArr[i] = users.users[i];
        }
        
        delete[] users.users;
        users.users = newArr;
        users.capacity = newCap;
    }
}

void User::createUser(User &users, const string &idNumber) {
    
    expandAndAssignUsers(users, 1);

    
    users.users[users.userCount].setIdNumber(idNumber);

    
    string name, birthDate;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese el nombre del usuario " << users.userCount + 1 << ": ";
    getline(cin, name);
    users.users[users.userCount].setName(name);

    do {
        cout << "Ingrese la fecha de nacimiento del usuario "<< users.userCount + 1 << " (DD/MM/AAAA): ";
        getline(cin, birthDate);
            if (!isValidDate(birthDate)) {
                cout << "Fecha invalida. Intente nuevamente." << endl;
            }
    } while (!isValidDate(birthDate));

    
    users.users[users.userCount].setBirthDate(birthDate);

    
    users.userCount++;
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
