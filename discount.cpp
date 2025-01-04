#include "Discount.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// Constructor sin parámetros
Discount::Discount() : discountPercentage(0), discountCount(0), codes(NULL) {}

// Constructor con parámetros
Discount::Discount(float percentage, int count) {
    this->discountPercentage = percentage;
    this->discountCount = count;
    this->codes = new DiscountCode[count];

    srand(time(0)); // Seed for random number generation

    for (int i = 0; i < count; i++) {
        codes[i].code = generateCode();
        codes[i].used = false;
    }
}
// Destructor
Discount::~Discount() {
    delete[] codes;
}

// Configurar descuentos después de la inicialización
void Discount::configure(float percentage, int count) {
    // Liberar memoria previa si ya había códigos generados
    if (codes != NULL) {
        delete[] codes;
    }

    this->discountPercentage = percentage;
    this->discountCount = count;
    this->codes = new DiscountCode[count];

    srand(time(0)); // Seed for random number generation

    for (int i = 0; i < count; i++) {
        codes[i].code = generateCode();
        codes[i].used = false;
    }
}



// Generar un código aleatorio
string Discount::generateCode() {
    string code;
    const char characters[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int length = 10; // Longitud del código
    for (int i = 0; i < length; i++) {
        code += characters[rand() % (sizeof(characters) - 1)];
    }
    return code;
}

// Verificar si un código de descuento es válido y no está usado
bool Discount::verifyCode(const string& code) {
    for (int i = 0; i < this->discountCount; i++) {
        if (codes[i].code == code) {
            if (!codes[i].used) {
                codes[i].used = true; // Marcar como usado
                return true;
            }
            break;
        }
    }
    return false;
}

// Obtener el porcentaje de descuento
float Discount::getDiscountPercentage() const {
    return this->discountPercentage;
}

// Mostrar todos los códigos de descuento generados y su estado
void Discount::showCodes() const {
    cout << "Generated discount codes:\n";
    for (int i = 0; i < this->discountCount; i++) {
        cout << codes[i].code << " - "
             << (codes[i].used ? "Used" : "Available") << endl;
    }
}
