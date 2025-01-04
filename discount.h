#include <iostream>
#include <string>
using namespace std;

struct DiscountCode
{
    string code;
    bool used;
};

class Discount
{
private:
    float discountPercentage;
    int discountCount;
    DiscountCode *codes;

    string generateCode();

public:
    // Constructor sin parámetros
    Discount();
    Discount(float percentage, int count);
    ~Discount();

    // Métodos
    string generateCode();
    bool verifyCode(const string &code);
    void configure(float percentage, int count); // Configurar descuentos después de la inicialización
    float getDiscountPercentage() const;
    void showCodes() const;
};
