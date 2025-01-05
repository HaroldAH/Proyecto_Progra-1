#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
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

public:
    Discount();
    Discount(float percentage, int count);
    ~Discount();

    string generateCode();
    bool verifyCode(const string& code);
    void configure(float percentage, int count);
    float getDiscountPercentage() const;
    void showCodes() const;
};

