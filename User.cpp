// User.cpp
#include "User.h"
#include <iostream>

using namespace std;

// Constructores
User::User(string cedula, string nombre, string fechaNacimiento, int cantidadEntradasCompradas)
    : cedula(cedula), nombre(nombre), fechaNacimiento(fechaNacimiento), cantidadEntradasCompradas(cantidadEntradasCompradas) {}

User::User() : cedula(""), nombre(""), fechaNacimiento(""), cantidadEntradasCompradas(0) {}

// Getters
string User::getCedula() const {
    return cedula;
}

string User::getNombre() const {
    return nombre;
}

string User::getFechaNacimiento() const {
    return fechaNacimiento;
}

int User::getCantidadEntradasCompradas() const {
    return cantidadEntradasCompradas;
}

// Setters
void User::setCedula(const string& cedula) {
    this->cedula = cedula;
}

void User::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void User::setFechaNacimiento(const string& fechaNacimiento) {
    this->fechaNacimiento = fechaNacimiento;
}

void User::setCantidadEntradasCompradas(int cantidad) {
    this->cantidadEntradasCompradas = cantidad;
}

// Método para mostrar los datos del usuario
void User::obtenerDatos() const {
    cout << "Cédula: " << cedula << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Fecha de Nacimiento: " << fechaNacimiento << endl;
    cout << "Entradas Compradas: " << cantidadEntradasCompradas << endl;
}

// Método para comprar entradas con verificación
bool User::comprarEntradas(int cantidad) {
    if (cantidad <= 0) {
        cout << "La cantidad de entradas a comprar debe ser positiva." << endl;
        return false;
    }

    if (cantidadEntradasCompradas + cantidad > 5) {
        cout << "Error: No puedes comprar más de 5 entradas en total. Ya has comprado " 
             << cantidadEntradasCompradas << " entradas." << endl;
        return false;
    }

    cantidadEntradasCompradas += cantidad;
    cout << "Entradas compradas exitosamente. Total ahora: " << cantidadEntradasCompradas << endl;
    return true;
}
