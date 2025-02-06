#include "User.h"

User::User() {
    userCount = 0;
}

User::~User() {}

void User::expandAndAssignUsers(User& usersObj, int quantity) {
    for (int i = 0; i < quantity; i++) {
        usersObj.users.insertAtEnd(UserData());
    }
}

void User::createUser(User& usersObj, const string& idNumber) {

    expandAndAssignUsers(usersObj, 1);

    List<UserData>::NodePtr userNode = usersObj.users.getHead();
    while (userNode->next) {
        userNode = userNode->next;
    }

    userNode->data.setIdNumber(idNumber);

    string name, birthDate;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese el nombre del usuario " << userCount + 1 << ": ";
    getline(cin, name);
    userNode->data.setName(name);

    do {
        cout << "Ingrese la fecha de nacimiento del usuario " << userCount + 1 << " (DD/MM/AAAA): ";
        getline(cin, birthDate);
        if (!isValidDate(birthDate)) {
            cout << "Fecha invalida. Intente nuevamente." << endl;
        }
    } while (!isValidDate(birthDate));

    userNode->data.setBirthDate(birthDate);

    userCount++;
}

bool User::isValidDate(string& birthDate) {

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

UserData* User::searchUserById(string& idNumber) {
    List<UserData>::NodePtr userNode = users.getHead();

    while (userNode) {
        if (userNode->data.getIdNumber() == idNumber) {
            return &(userNode->data);
        }
        userNode = userNode->next;
    }
    return nullptr;
}

List<UserData>& User::getUsers() {
    return users;
}

int User::getValidIntInput() {
    int val;
    while (true) {
        if (cin >> val) {
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente nuevamente: ";
    }
}
