#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <iostream>
#include <string>

using namespace std;

struct DiscountCode {
    string code;
    bool used;
};

class Discount {
private:
    float discountPercentage;       
    int discountCount;              
    DiscountCode* codes;            

    string generateCode();

public:
    
    Discount(float percentage, int count);

    ~Discount();

    bool verifyCode(const string& code);

    float getDiscountPercentage() const;

    void showCodes() const;
};

#endif 
