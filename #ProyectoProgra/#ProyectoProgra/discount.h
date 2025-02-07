#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <SFML/Graphics.hpp>
#include <string>
#include "List.h"

using namespace std;

class Discount {
private:
    float discountPercentage;
    int discountCount;
    List<string> codes;
    List<bool> used;
    List<float> percentages;

    string generateCode();

public:
    Discount();
    Discount(float percentage, int count);
    ~Discount();

    void configure(float percentage, int count);
    void configureDiscounts();
    bool verifyCode(string& code);
    float getDiscountPercentage();
    void showCodes();
    int getValidIntInput();
    void deleteDiscount();
    void showDiscountList();
    string getCodesString();
    bool deleteDiscountAtIndex(int index);


};

#endif