#include "Discount.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

Discount::Discount() {
    discountPercentage = 0.0f;
    discountCount = 0;
}

Discount::Discount(float percentage, int count) {
    discountPercentage = 0.0f;
    discountCount = 0;
    configure(percentage, count);
}

Discount::~Discount() {
  
}

void Discount::configure(float percentage, int count) {
    if (count <= 0) return;
    srand(time(0));

    for (int i = 0; i < count; i++) {
        codes.insertAtEnd(generateCode());
        used.insertAtEnd(false);
        percentages.insertAtEnd(percentage);
    }
    discountCount += count;
    discountPercentage = percentage;
}

void Discount::configureDiscounts() {
    float percentage;
    int countValue;
    cout << "\n=== Configurar Descuentos ===\n";
    cout << "Ingrese el porcentaje de descuento: ";
    percentage = getValidIntInput();
    cout << "Ingrese la cantidad de codigos a generar: ";
    countValue = getValidIntInput();
    configure(percentage, countValue);
    showCodes();
}

void Discount::showDiscountList() {
    if (codes.getHead() == nullptr) {
        cout << "No hay codigos de descuento disponibles." << endl;
        return;
    }

    cout << "\n=== LISTA DE CODIGOS ===\n";

    List<string>::NodePtr codeNode = codes.getHead();
    List<float>::NodePtr percentageNode = percentages.getHead();
    List<bool>::NodePtr usedNode = used.getHead();
    int index = 1;

    while (codeNode) {
        cout << index << ". "
            << codeNode->data << " - "
            << percentageNode->data << "% - "
            << (usedNode->data ? "Usado" : "Disponible") << "\n";
        codeNode = codeNode->next;
        percentageNode = percentageNode->next;
        usedNode = usedNode->next;
        index++;
    }

    cout << "=========================\n";
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

void Discount::deleteDiscount() {

    if (codes.getHead() == nullptr) {
        cout << "No hay codigos de descuento disponibles para eliminar." << endl;
        cout << "Presione Enter para continuar.." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    showDiscountList();  

    cout << "Ingrese el numero del codigo a eliminar (1-" << discountCount << "): ";
    int num = getValidIntInput();

    if (num < 1 || num > discountCount) {
        cout << "Numero invalido. Intente de nuevo." << endl;
        return;
    }

    
    List<bool>::NodePtr usedNode = used.getHead();
    for (int i = 1; i < num; i++) {
        usedNode = usedNode->next;
    }

    
    if (usedNode->data) {
        cout << "El codigo seleccionado ya ha sido usado y no puede eliminarse." << endl;
        cout << "Presione Enter para continuar..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    
    codes.deletePosition(num);
    percentages.deletePosition(num);
    used.deletePosition(num);

    discountCount--;  

    cout << "\nCodigo eliminado exitosamente.\n";

    showDiscountList();  

    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


bool Discount::verifyCode(string& code) {
    List<string>::NodePtr codeNode = codes.getHead();
    List<bool>::NodePtr usedNode = used.getHead();
    List<float>::NodePtr percentageNode = percentages.getHead();

    while (codeNode) {
        if (codeNode->data == code) {
            if (!usedNode->data) {
                usedNode->data = true;
                discountPercentage = percentageNode->data;
                return true;
            }
            break;
        }
        codeNode = codeNode->next;
        usedNode = usedNode->next;
        percentageNode = percentageNode->next;
    }
    return false;
}

float Discount::getDiscountPercentage() {
    return discountPercentage;
}

void Discount::showCodes() {
    cout << "\n=== LISTA DE CODIGOS ===\n";

    List<string>::NodePtr codeNode = codes.getHead();
    List<bool>::NodePtr usedNode = used.getHead();
    List<float>::NodePtr percentageNode = percentages.getHead();

    while (codeNode) {
        cout << codeNode->data << " - "
            << percentageNode->data << "% - "
            << (usedNode->data ? "Usado" : "Disponible") << "\n";
        codeNode = codeNode->next;
        usedNode = usedNode->next;
        percentageNode = percentageNode->next;
    }

    cout << "=========================\n";
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int Discount::getValidIntInput() {
    int input;
    while (!(cin >> input) || input <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
    return input;
}

string Discount::getCodesString() {
    std::string result;
    List<std::string>::NodePtr codeNode = codes.getHead();
    List<float>::NodePtr percentageNode = percentages.getHead();
    List<bool>::NodePtr usedNode = used.getHead();
    int index = 1;
    while (codeNode) {
        result += std::to_string(index) + ". " +
            codeNode->data + " - " +
            std::to_string(static_cast<int>(percentageNode->data)) + "% - " +
            (usedNode->data ? "Usado" : "Disponible") + "\n";
        codeNode = codeNode->next;
        percentageNode = percentageNode->next;
        usedNode = usedNode->next;
        index++;
    }
    return result;
}


bool Discount::deleteDiscountAtIndex(int index) {
    
    if (codes.getHead() == nullptr) {
        return false;
    }
   
    if (index < 1 || index > discountCount) {
        return false;
    }
    
    List<bool>::NodePtr usedNode = used.getHead();
    for (int i = 1; i < index; i++) {
        usedNode = usedNode->next;
    }
    if (usedNode->data) {
        
        return false;
    }
   
    codes.deletePosition(index);
    percentages.deletePosition(index);
    used.deletePosition(index);
    discountCount--;
    return true;
}
