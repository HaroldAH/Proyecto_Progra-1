#include "Discount.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

Discount::Discount() {
    discountPercentage = 0.0f;
    discountCount = 0;
    codes = nullptr;
    used = nullptr;
    percentages = nullptr;
}

Discount::Discount(float percentage, int count) {
    discountPercentage = 0.0f;
    discountCount = 0;
    codes = nullptr;
    used = nullptr;
    percentages = nullptr;
    configure(percentage, count);
}

Discount::~Discount() {
    delete[] codes;
    delete[] used;
    delete[] percentages;
    discountCount = 0;
}

void Discount::configure(float percentage, int count) {
    srand(time(0));
    int oldCount = discountCount;
    int newCount = oldCount + count;
    string* newCodes = new string[newCount];
    bool* newUsed = new bool[newCount];
    float* newPercentages = new float[newCount];
    for (int i = 0; i < oldCount; i++) {
        newCodes[i] = codes[i];
        newUsed[i] = used[i];
        newPercentages[i] = percentages[i];
    }
    for (int i = oldCount; i < newCount; i++) {
        newCodes[i] = generateCode();
        newUsed[i] = false;
        newPercentages[i] = percentage;
    }
    if (codes) delete[] codes;
    if (used) delete[] used;
    if (percentages) delete[] percentages;
    codes = newCodes;
    used = newUsed;
    percentages = newPercentages;
    discountCount = newCount;
    discountPercentage = percentage;
}

void Discount::configureDiscounts() {
    float perc;
    int countValue;
    cout << "\n=== Configurar Descuentos ===\n";
    cout << "Ingrese el porcentaje de descuento: ";
    cin >> perc;
    cout << "Ingrese la cantidad de codigos a generar: ";
    cin >> countValue;
    configure(perc, countValue);
    showCodes();
    cout << "\nRECUERDE EL CODIGO\n";
    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string Discount::generateCode() {
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int length = 5;
    string code;
    for (int i = 0; i < length; i++) {
        code += chars[rand() % (sizeof(chars) - 1)];
    }
    return code;
}

bool Discount::verifyCode(const string &code) {
    for (int i = 0; i < discountCount; i++) {
        if (codes[i] == code) {
            if (!used[i]) {
                used[i] = true;
                discountPercentage = percentages[i];
                return true;
            }
            break;
        }
    }
    return false;
}

float Discount::getDiscountPercentage() const {
    return discountPercentage;
}

void Discount::showCodes() const {
    cout << "\n=== LISTA DE CODIGOS ===\n";
    for (int i = 0; i < discountCount; i++) {
        cout << codes[i] << " - "
             << percentages[i] << "% - "
             << (used[i] ? "Used" : "Available") << "\n";
    }
    cout << "=========================\n";
}
