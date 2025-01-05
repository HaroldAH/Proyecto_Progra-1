#include "User.h"
#include <iostream>

using namespace std;

// Constructores
User::User(string idNumber, string name, string birthDate, int ticketsPurchased)
    : idNumber(idNumber), name(name), birthDate(birthDate), ticketsPurchased(ticketsPurchased) {}

User::User() : idNumber(""), name(""), birthDate(""), ticketsPurchased(0) {}

// Getters
string User::getIdNumber() const {
    return idNumber;
}

string User::getName() const {
    return name;
}

string User::getBirthDate() const {
    return birthDate;
}

int User::getTicketsPurchased() const {
    return ticketsPurchased;
}

// Setters
void User::setIdNumber(const string& idNumber) {
    this->idNumber = idNumber;
}

void User::setName(const string& name) {
    this->name = name;
}

void User::setBirthDate(const string& birthDate) {
    this->birthDate = birthDate;
}

void User::setTicketsPurchased(int quantity) {
    this->ticketsPurchased = quantity;
}

// Método para mostrar los datos del usuario
void User::displayData() const {
    cout << "Cedula: " << idNumber << endl;
    cout << "Nombre: " << name << endl;
    cout << "Fecha de Nacimiento: " << birthDate << endl;
    cout << "Entradas Compradas: " << ticketsPurchased << endl;
}

// Método para comprar entradas con verificación
bool User::purchaseTickets(int quantity) {
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
