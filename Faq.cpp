#include "Faq.h"
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

int Faq::validateChoice(int &choice, int &size) {
    while (true) {
        cout << "\nSeleccione una opcion: ";
        if (cin >> choice && choice >= 1 && choice <= size) {
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente de nuevo." << endl;
    }
}

void Faq::showFaq() {
    while (true) {
        system("cls");

        int opcion, size = 5;

        cout << "\n=========================================\n";
        cout << "         PREGUNTAS FRECUENTES (FAQ)     \n";
        cout << "=========================================\n\n";
        cout << "  1) Cuales son las reglas del evento?\n";
        cout << "  2) Como puedo comprar entradas?\n";
        cout << "  3) Se pueden aplicar descuentos?\n";
        cout << "  4) Como veo las ventas realizadas?\n";
        cout << "  5) Volver al menu principal\n";

        validateChoice(opcion, size);

        if (opcion == 1) {
            system("cls");
            cout << "\n=========================================\n";
            cout << "  REGLAS DEL EVENTO\n";
            cout << "=========================================\n\n";
            cout << " - Llegar con al menos 30 minutos de anticipacion.\n";
            cout << " - Mantener el orden y seguir las indicaciones del personal.\n";
            cout << " - No ingresar con objetos prohibidos (armas, bebidas alcoholicas, etc.).\n";
            cout << " - Respetar las normas de seguridad y, si aplica, medidas sanitarias.\n";
            cout << " - Los organizadores no se hacen responsables por objetos perdidos.\n\n";
        }
        else if (opcion == 2) {
            system("cls");
            cout << "\n=========================================\n";
            cout << "  COMO COMPRAR ENTRADAS?\n";
            cout << "=========================================\n\n";
            cout << " - Seleccionar la opcion 'Vender Entrada' en el menu principal.\n";
            cout << " - Registrar un usuario (si no existe).\n";
            cout << " - Elegir el evento, segmento y asientos.\n";
            cout << " - Completar el proceso de pago segun corresponda.\n";
            cout << " - Recibir confirmacion del sistema.\n\n";
        }
        else if (opcion == 3) {
            system("cls");
            cout << "\n=========================================\n";
            cout << "  APLICACION DE DESCUENTOS\n";
            cout << "=========================================\n\n";
            cout << " - Se generan codigos de descuento en la opcion 'Configurar Descuentos'.\n";
            cout << " - Al vender una entrada, se ingresa el codigo (si aplica).\n";
            cout << " - El descuento se aplica al subtotal si el codigo es valido.\n";
            cout << " - Cada codigo de descuento tiene un uso limitado (generalmente una vez).\n\n";
        }
        else if (opcion == 4) {
            system("cls");
            cout << "\n=========================================\n";
            cout << "  COMO VEO LAS VENTAS REALIZADAS?\n";
            cout << "=========================================\n\n";
            cout << " - Seleccionar la opcion 'Consultar Ventas'.\n";
            cout << " - Se muestra la ocupacion de asientos por evento y segmento.\n";
            cout << " - Alli puedes ver cuantos asientos han sido vendidos y cuales quedan disponibles.\n";
            cout << " - Tambien puedes consultar el estado de cada asiento en tiempo real.\n\n";
        }
        else if (opcion == 5) {
            return;
        }

        cout << "Presione Enter para continuar... ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}
