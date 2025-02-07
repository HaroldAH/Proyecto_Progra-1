#ifndef FAQ_H
#define FAQ_H

#include <string>

class Faq {
public:
    // Devuelve el contenido de la sección "Reglas del evento"
    std::string getRulesContent();

    // Devuelve el contenido de la sección "Cómo comprar entradas"
    std::string getHowToBuyContent();

    // Devuelve el contenido de la sección "Aplicación de descuentos"
    std::string getDiscountsContent();

    // Devuelve el contenido de la sección "Cómo ver las ventas realizadas"
    std::string getViewSalesContent();
};

#endif // FAQ_H
