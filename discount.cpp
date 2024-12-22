#include "Discount.h"
#include <cstdlib>
#include <ctime>

// Constructor
Descuento::Descuento(float porcentaje, int cantidad) {
    this->porcentajeDescuento = porcentaje;
    this->cantidadDescuentos = cantidad;
    this->codigos = new CodigoDescuento[cantidad]; 

    srand(time(0)); //

    for (int i = 0; i < cantidad; i++) {
        codigos[i].codigo = generarCodigo();
        codigos[i].usado = false; 
    }
}

// Destructor
Descuento::~Descuento() {
    delete[] codigos; 
}


string Descuento::generarCodigo() {
    string codigo;
    const char caracteres[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int longitud = 10; // Longitud del código
    for (int i = 0; i < longitud; i++) {
        codigo += caracteres[rand() % (sizeof(caracteres) - 1)];
    }
    return codigo;
}


bool Descuento::verificarCodigo(const string& codigo) {
    for (int i = 0; i < this->cantidadDescuentos; i++) {
        if (codigos[i].codigo == codigo) { 
            if (!codigos[i].usado) { 
                codigos[i].usado = true; // Marcar como usado
                return true;
            }
            break; 
        }
    }
    return false; 
}


float Descuento::obtenerPorcentajeDescuento() const {
    return this->porcentajeDescuento;
}


void Descuento::mostrarCodigos() const {
    cout << "Códigos de descuento generados:\n";
    for (int i = 0; i < this->cantidadDescuentos; i++) {
        cout << codigos[i].codigo << " - "
             << (codigos[i].usado ? "Usado" : "Disponible") << endl;
    }
}
