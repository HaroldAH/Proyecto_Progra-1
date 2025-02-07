#include "Faq.h"
#include <iostream>
#include <string>

using namespace std;


string Faq::getRulesContent() {
    return "- Llegar con minimo 30 minutos de anticipacion.\n"
        "- Mantener el orden.\n"
        "- No ingresar con objetos prohibidos.\n"
        "- Respetar normas de seguridad.\n"
        "- No nos hacemos responsables por objetos perdidos.\n";
}

string Faq::getHowToBuyContent() {
    return "- Seleccione 'Vender Entrada'.\n"
        "- Registre un usuario si no existe.\n"
        "- Elija evento, segmento y asientos.\n"
        "- Complete el proceso de pago.\n"
        "- Reciba confirmacion.\n";
}

string Faq::getDiscountsContent() {
    return "- Se generan codigos en 'Configurar Descuentos'.\n"
        "- Al vender entrada, ingrese el codigo.\n"
        "- Si es valido, se aplica al subtotal.\n"
        "- Cada codigo se puede usar solo una vez.\n";
}

string Faq::getViewSalesContent() {
    return "- Seleccione 'Consultar Ventas'.\n"
        "- Podra ver la ocupacion de asientos.\n"
        "- Cuantas entradas se han vendido.\n"
        "- Estado de cada asiento.\n";
}

    

