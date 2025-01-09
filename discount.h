#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>

class Discount {
private:
    // Almacena el "último" porcentaje verificado o configurado
    float discountPercentage;

    // Número total de códigos en este objeto
    int discountCount;

    // Arreglos paralelos:
    //  - codes[i] -> texto del código
    //  - used[i]  -> si está usado
    //  - percentages[i] -> el % individual para ese código
    std::string* codes;
    bool* used;
    float* percentages;

public:
    // Constructores y destructor
    Discount();
    Discount(float percentage, int count);
    ~Discount();

    // "Acumula" nuevos códigos de un porcentaje dado
    void configure(float percentage, int count);

    // Pide porcentaje y número de códigos por consola, llama a configure
    void configureDiscounts();

    // Verifica si un código está disponible; si lo encuentra sin usar, lo marca
    // y actualiza discountPercentage con el % de ese código
    bool verifyCode(const std::string &code);

    // Devuelve el discountPercentage "actual" (el del último código verificado)
    float getDiscountPercentage() const;

    // Muestra todos los códigos con sus porcentajes y estado (Used / Available)
    void showCodes() const;

private:
    // Genera un código aleatorio de 5 caracteres [A-Za-z0-9]
    std::string generateCode();
};

#endif
