#include "Discount.h"
#include <cstdlib>
#include <ctime>

// Constructor
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

// Generate a random discount code
string Discount::generateCode() {
    string code;
    const char characters[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int length = 10; // Length of the code
    for (int i = 0; i < length; i++) {
        code += characters[rand() % (sizeof(characters) - 1)];
    }
    return code;
}

// Verify if a discount code is valid and unused
bool Discount::verifyCode(const string& code) {
    for (int i = 0; i < this->discountCount; i++) {
        if (codes[i].code == code) { 
            if (!codes[i].used) { 
                codes[i].used = true; // Mark as used
                return true;
            }
            break; 
        }
    }
    return false; 
}

// Get the discount percentage
float Discount::getDiscountPercentage() const {
    return this->discountPercentage;
}

// Display all generated discount codes and their status
void Discount::showCodes() const {
    cout << "Generated discount codes:\n";
    for (int i = 0; i < this->discountCount; i++) {
        cout << codes[i].code << " - "
             << (codes[i].used ? "Used" : "Available") << endl;
    }
}
