#include "Menu.h"
#include <iostream>
using namespace std;

void Menu::mostrarMenu()
{
    int opcion;

    do
    {
        cout << "\n=== Menu Principal ===" << endl;
        cout << "1. Configurar evento" << endl;
        cout << "2. Configurar descuentos" << endl;
        cout << "3. Vender entrada" << endl;
        cout << "4. Consultar estado de las ventas" << endl;
        cout << "5. Acerca de" << endl;
        cout << "6. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            cout << "Opcion 1: Configurar evento seleccionada." << endl;
            break;
        case 2:
            configurarDescuentos();
            break;
        case 3:
            cout << "Opcion 3: Vender entrada seleccionada." << endl;
            break;
        case 4:
            cout << "Opcion 4: Consultar estado de las ventas seleccionada." << endl;
            break;
        case 5:
            cout << "Opcion 5: Acerca de seleccionada." << endl;
            break;
        case 6:
            cout << "Saliendo del programa. ¡Gracias!" << endl;
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente." << endl;
        }
    } while (opcion != 6);
} // <- Cierra correctamente la función mostrarMenu

void Menu::configurarDescuentos()
{
    float porcentaje;
    int cantidad;

    cout << "\n=== Configurar Descuentos ===" << endl;
    cout << "Ingrese el porcentaje de descuento: ";
    cin >> porcentaje;

    cout << "Ingrese la cantidad de descuentos a generar: ";
    cin >> cantidad;

    // Crea una instancia local de Descuento
    Descuento descuento(porcentaje, cantidad);

    cout << "\nCódigos de descuento generados:\n";
    descuento.mostrarCodigos(); // Muestra los códigos generados
}
