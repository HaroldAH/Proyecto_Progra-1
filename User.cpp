#include "User.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>


using namespace std;


User::User() {
    idNumber = "";
    name = "";
    birthDate = "";
    ticketsPurchased = 0;
    userCount = 0;
    users = nullptr;
}
User::User(string aIdNumber, string aName, string aBirthDate, int aTicketsPurchased) {
    idNumber = aIdNumber;
    name = aName;
    birthDate = aBirthDate;
    ticketsPurchased = aTicketsPurchased;
}

string User::getIdNumber(){
    return idNumber;
}

string User::getName() {
    return name;
}

string User::getBirthDate(){
    return birthDate;
}

int User::getTicketsPurchased(){
    return ticketsPurchased;
}

void User::setIdNumber(string& aIdNumber) {
    idNumber = aIdNumber;
}

void User::setName(string& aName) {
    name = aName;
}

void User::setBirthDate(string& aBirthDate) {
    birthDate = aBirthDate;
}

void User::setTicketsPurchased(int& quantity) {
    ticketsPurchased = quantity;
}

void User::expandAndAssignUsers(User& users, int& quantity) {
    
    if (users.userCount + quantity > 0) {
        int newCapacity = users.userCount + quantity;

        User* newUsers = new User[newCapacity];
        for (int j = 0; j < users.userCount; j++) {
            newUsers[j] = users.users[j];  
        }

        delete[] users.users;  
        users.users = newUsers;  
    }
}

void User::createUser(User& users) {

    int quantity = 0;

    cout << "Cuantos usuarios desea agregar?" << endl;
    quantity = getValidIntInput();   

    expandAndAssignUsers(users, quantity);  


    for (int i = 0; i < quantity; i++) {
        string idNumber, name, birthDate;

        cout << "\nIngrese la cedula del usuario " << users.userCount + 1 << ": ";
        cin.ignore();
        getline(cin, idNumber);

        cout << "Ingrese el nombre del usuario " << users.userCount + 1 << ": ";
        getline(cin, name);

        cout << "Ingrese la fecha de nacimiento del usuario " << users.userCount + 1 << " (DD/MM/AAAA): ";
        getline(cin, birthDate);

        users.users[users.userCount].setIdNumber(idNumber); 
        users.users[users.userCount].setName(name); 
        users.users[users.userCount].setBirthDate(birthDate); 
        
        users.userCount++;  

        cout << "Usuario registrado exitosamente.\n";
    }

    cout << "Se han registrado " << quantity << " usuarios correctamente.\n";
}

User* User::searchUserById(string& idNumber) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].getIdNumber() == idNumber) {
            return &users[i];  
        }
    }
    return nullptr; 
}

void User::displayData() {
    cout << "Cedula: " << idNumber << endl;
    cout << "Nombre: " << name << endl;
    cout << "Fecha de Nacimiento: " << birthDate << endl;
    cout << "Entradas Compradas: " << ticketsPurchased << endl;
}

bool User::purchaseTickets(int& quantity) {
    if (quantity <= 0) {
        cout << "La cantidad de entradas a comprar debe ser positiva." << endl;
        return false;
    }

    if (ticketsPurchased + quantity > 5) {
        cout << "Error: No puedes comprar mas de 5 entradas en total. Ya has comprado " 
             << ticketsPurchased << " entradas." << endl;
        return false;
    }

    ticketsPurchased += quantity;
    cout << "Entradas compradas exitosamente. Total ahora: " << ticketsPurchased << endl;
    return true;
}

int User::getValidIntInput() 
{
    int input;
    while (!(cin >> input)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
    return input;
}
