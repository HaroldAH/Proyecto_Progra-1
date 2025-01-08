#include "discount.h"
#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <string>

using namespace std;

Discount::Discount() {
    discountPercentage = 0;
    discountCount = 0;
    codes = NULL;
}

Discount::Discount(float percentage, int count) {
    this->discountPercentage = percentage;
    this->discountCount = count;
    this->codes = new DiscountCode[count];

    srand(time(0));

    for (int i = 0; i < count; i++) {
        codes[i].code = generateCode();
        codes[i].used = false;
    }
}

Discount::~Discount() {
    delete[] codes;
}

void Discount::configure(float percentage, int count) {
    if (codes != NULL) {
        delete[] codes;
    }

    this->discountPercentage = percentage;
    this->discountCount = count;
    this->codes = new DiscountCode[count];

    srand(time(0));

    for (int i = 0; i < count; i++) {
        codes[i].code = generateCode();
        codes[i].used = false;
    }
}

void Discount::configureDiscounts() {
    
    float discountPercentage;
    int discountCount;

    cout << "\n=== Configurar Descuentos ===" << endl;
    cout << "Ingrese el porcentaje de descuento: ";
    cin >> discountPercentage;

    cout << "Ingrese la cantidad de descuentos a generar: ";
    cin >> discountCount;

    configure(discountPercentage, discountCount);  

    showCodes();  
}

string Discount::generateCode() {
    string code;
    const char characters[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int length = 5;
    for (int i = 0; i < length; i++) {
        code += characters[rand() % (sizeof(characters) - 1)];
    }
    return code;
}

bool Discount::verifyCode(const std::string& code) {
    for (int i = 0; i < this->discountCount; i++) {
        if (codes[i].code == code) {
            if (!codes[i].used) {
                codes[i].used = true;
                return true;
            }
            break;
        }
    }
    return false;
}

float Discount::getDiscountPercentage() const {
    return this->discountPercentage;
}

void Discount::showCodes() const {
    cout << "Generated discount codes:\n";
    for (int i = 0; i < this->discountCount; i++) {
        cout << codes[i].code << " - "
             << (codes[i].used ? "Used" : "Available") << endl;
    }
}