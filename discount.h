#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>
using namespace std;

class Discount {
private:

    float discountPercentage;
    int discountCount;
    string* codes;
    bool* used;
    float* percentages;
    string generateCode();

public:
    
    Discount();

    Discount(float percentage, int count);

    ~Discount();

    void configure(float percentage, int count);

    void configureDiscounts();

    bool verifyCode(string &code);

    float getDiscountPercentage() ;

    void showCodes() ;
    
    int getValidIntInput();
    
        
    };

    #endif
