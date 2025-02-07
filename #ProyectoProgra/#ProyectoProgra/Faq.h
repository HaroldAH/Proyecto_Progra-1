#ifndef FAQ_H
#define FAQ_H

#include <string>

class Faq {
public:
    // Devuelve el contenido de la secci�n "Reglas del evento"
    std::string getRulesContent();

    // Devuelve el contenido de la secci�n "C�mo comprar entradas"
    std::string getHowToBuyContent();

    // Devuelve el contenido de la secci�n "Aplicaci�n de descuentos"
    std::string getDiscountsContent();

    // Devuelve el contenido de la secci�n "C�mo ver las ventas realizadas"
    std::string getViewSalesContent();
};

#endif // FAQ_H
