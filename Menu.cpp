#include "Menu.h"
#include <iostream>
using namespace std;

void Menu::showMenu()
{
    int option;

    do
    {
        cout << "\033[31m";
        cout << " ______  ______  ______  ______  ______  ______  ______  ______ \n";
        cout << "| |__| || |__| || |__| || |__| || |__| || |__| || |__| || |__| |\n";
        cout << "|  ()  ||  ()  ||  ()  ||  ()  ||  ()  ||  ()  ||  ()  ||  ()  |\n";
        cout << "|______||______||______||______||______||______||______||______|\n";
        cout << " ______                                                  ______ \n";
        cout << "| |__| |  _____     _            _ _                    | |__| |\n";
        cout << "|  ()  | | ____|___| |_ __ _  __| (_) ___               |  ()  |\n";
        cout << "|______| |  _| / __| __/ _` |/ _` | |/ _ \\              |______|\n";
        cout << " ______  | |___\\__ \\ || (_| | (_| | | (_) |              ______ \n";
        cout << "| |__| | |_____|___/\\__\\__,_|\\__,_|_|\\___/              | |__| |\n";
        cout << "|  ()  |  ____              _   _                       |  ()  |\n";
        cout << "|______| / ___|  __ _ _ __ | |_(_) __ _  __ _  ___      |______|\n";
        cout << " ______  \\\___ \\\ / _` | '_ \\\\| __| |/ _` |/ _` |/ _ \\      ______ \n";
        cout << "| |__| |  ___) | (_| | | | | |_| | (_| | (_| | (_) |    | |__| |\n";
        cout << "|  ()  | |____/ \\\__,_|_| |_|\\__|_|\\__,_|\\__, |\\___/     |  ()  |\n";
        cout << "|______| | __ )  ___ _ __ _ __   __ _| ||___//_/ _   _  |______|\n";
        cout << " ______  |  _ \\\\ / _ \\\\ '__| '_ \\\\ / _` | '_ \\\\ / _ \\\\ | | |  ______ \n";
        cout << "| |__| | | |_) |  __/ |  | | | | (_| | |_) |  __/ |_| | | |__| |\n";
        cout << "|  ()  | |____/ \\\___|_|  |_| |_|\\__,_|_.__/ \\\___|\\__,_| |  ()  |\n";
        cout << "|______|                                                |______|\n";
        cout << " ______  ______  ______  ______  ______  ______  ______  ______ \n";
        cout << "| |__| || |__| || |__| || |__| || |__| || |__| || |__| || |__| |\n";
        cout << "|  ()  ||  ()  ||  ()  ||  ()  ||  ()  ||  ()  ||  ()  ||  ()  |\n";
        cout << "|______||______||______||______||______||______||______||______|\n";

        cout << "\033[37m";

        cout << "               +==========================+\n";
        cout << "                |     Menu Principal      |\n";
        cout << "                +==========================+\n";
        cout << "                | 1. Configurar Evento    |\n";
        cout << "                | 2. Configurar Descuentos|\n";
        cout << "                | 3. Vender Entrada       |\n";
        cout << "                | 4. Consultar Ventas     |\n";
        cout << "                | 5. Acerca de            |\n";
        cout << "                | 6. Salir                |\n";
        cout << "                +==========================+\n";
        cout << "                | Seleccione una opcion:  |\n";
        cout << "                +--------------------------+\n";
        cout << "                >> ";
        cin >> option;

        switch (option)
        {
        case 1:
            cout << "Opción 1: Configurar Evento seleccionada." << endl;
            break;
        case 2:
            configureDiscounts();
            break;
        case 3:
            cout << "Opcion 3: Vender Entrada seleccionada." << endl;
            break;
        case 4:
            cout << "Opcion 4: Consultar Ventas seleccionada." << endl;
            break;
        case 5:
            showAbout();
            break;
        case 6:
            cout << "Saliendo del programa. ¡Gracias!" << endl;
            break;
        default:
            cout << "Opcion inválida. Intente nuevamente." << endl;
        }
    } while (option != 6);
}

void Menu::configureDiscounts()
{
    float discountPercentage;
    int discountCount;

    cout << "\n=== Configurar Descuentos ===" << endl;
    cout << "Ingrese el porcentaje de descuento: ";
    cin >> discountPercentage;

    cout << "Ingrese la cantidad de descuentos a generar: ";
    cin >> discountCount;

    Discount discount(discountPercentage, discountCount);

    discount.showCodes(); 
    cout << "RECUERDE EL CODIGO";
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
    system("cls");
}

void showAbout() {
    cout << "\n=========================================\n";
    cout << "               ACERCA DE                 \n";
    cout << "=========================================\n";
    cout << "Integrantes del Proyecto:\n";
    cout << "  - Harold Avila Hernandez\n";
    cout << "  - Cristhian Cordero Varela\n";
    cout << "=========================================\n";
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
    system("cls");
}
