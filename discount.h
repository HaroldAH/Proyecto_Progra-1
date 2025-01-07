#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>

struct DiscountCode {
    std::string code;
    bool used;
};

class Discount {
private:
    float discountPercentage;
    int discountCount;
    DiscountCode* codes;

public:
    Discount();
    Discount(float percentage, int count);
    ~Discount();

    std::string generateCode();
    bool verifyCode(const std::string& code);
    void configure(float percentage, int count);
    float getDiscountPercentage() const;
    void showCodes() const;

    void configureDiscounts(); 
};

#endif
