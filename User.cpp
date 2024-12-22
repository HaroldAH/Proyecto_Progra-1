#include "User.h"

User::User(string cedula, string nombre, string fechaNacimiento, int cantidadEntradasCompradas){
    this->cedula = cedula;
    this->nombre = nombre;
    this->fechaNacimiento = fechaNacimiento;
    this->cantidadEntradasCompradas = cantidadEntradasCompradas;
    }

void User::comprarEntradas(int cantidad) {
    if (cantidadEntradasCompradas + cantidad > 5) {
        cout << "No puedes comprar más de 5 entradas para el mismo evento." << endl;
        return;
    }
    cantidadEntradasCompradas += cantidad;
    cout << "Compra realizada. Entradas totales compradas: " << cantidadEntradasCompradas << endl;
}

void User::obtenerDatos() {
    cout << "Cédula: " << cedula << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Fecha de Nacimiento: " << fechaNacimiento << endl;
    cout << "Entradas Compradas: " << cantidadEntradasCompradas << endl;
}
