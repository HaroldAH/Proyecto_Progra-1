#include "Menu.h"
#include <iostream>
#include <string>
#include <cstdlib>1

#include <limits>

using namespace std;

void Menu::showMenu()
{
    int option, size = 6;

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
        cout << " ______  \\___ \\ / _` | '_ \\| __| |/ _` |/ _` |/ _ \\      ______ \n";
        cout << "| |__| |  ___) | (_| | | | | |_| | (_| | (_| | (_) |    | |__| |\n";
        cout << "|  ()  | |____/ \\__,_|_| |_|\\__|_|\\__,_|\\__, |\\___/     |  ()  |\n";
        cout << "|______| | __ )  ___ _ __ _ __   __ _| ||___//_/ _   _  |______|\n";
        cout << " ______  |  _ \\ / _ \\ '__| '_ \\ / _` | '_ \\ / _ \\ | | |  ______ \n";
        cout << "| |__| | | |_) |  __/ |  | | | | (_| | |_) |  __/ |_| | | |__| |\n";
        cout << "|  ()  | |____/ \\___|_|  |_| |_|\\__,_|_.__/ \\___|\\__,_| |  ()  |\n";
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

        validateChoice(option, size);

        switch (option)
        {
        case 1:
            showEventMenu(event);
            break;
        case 2:
            configureDiscounts();
            break;
        case 3:
            sellTicket();
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

void showAbout()
{
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

void Menu::showEventMenu(Event &event)
{

    while (true)
    {

        int choice, size = 4;
        cout << "\n=== Configurar Eventos ===" << endl;
        cout << "1. Agregar nuevo evento" << endl;
        cout << "2. Modificar segmentos" << endl;
        cout << "3. Mostrar eventos actuales" << endl;
        cout << "4. Regresar al menu principal" << endl;

        validateChoice(choice, size);

        if (choice == 1)
        {
            event.saveEvent(event);
            continue;
        }

        if (choice == 2)
        {
            segment.saveSegments(event);
            continue;
        }

        if (choice == 3)
        {
            listEventAndSegments(event, segment);
            continue;
        }

        if (choice == 4)
        {
            return;
        }
        cout << endl
             << " Entrada invalida. Intente de nuevo." << endl
             << endl;
    }
}

void Menu::listEventAndSegments(Event &event, Segment &segment)
{

    if (event.getEventCount() == 0)
    {
        cout << "No hay eventos guardados." << endl;
        return;
    }

    cout << endl
         << "Lista de eventos y segmentos:" << endl
         << endl;

    Segment **segmentsByEvent = segment.getSegmentsByEvent();

    for (int i = 0; i < event.getEventCount(); i++)
    {
        cout << "Evento #" << i + 1 << ":" << endl;
        cout << "Nombre: " << event.getEvents()[i].getName() << endl;
        cout << "Fecha: " << event.getEvents()[i].getDate() << endl;
        cout << "Descripcion: " << event.getEvents()[i].getDescription() << endl;

        cout << "Segmentos asociados:" << endl
             << endl;

        if (segmentsByEvent[i] == NULL)
        {
            cout << "  No hay segmentos asociados para este evento." << endl;
            cout << "--------------------------" << endl;
            continue;
        }

        int *segmentCounts = new int[event.getEventCount()];
        segment.getSegmentCount(event, segmentCounts);
        int numSegments = segmentCounts[i];

        for (int j = 0; j < numSegments; j++)
        {
            cout << "  Segmento #" << j + 1 << ":" << endl;
            cout << "    Nombre: " << segmentsByEvent[i][j].getName() << endl;
            cout << "    Filas: " << segmentsByEvent[i][j].getRows() << endl;
            cout << "    Asientos por fila: " << segmentsByEvent[i][j].getSeats() << endl;
            cout << "    Precio: " << segmentsByEvent[i][j].getPrice() << endl;
        }

        cout << "--------------------------" << endl;

        delete[] segmentCounts;
    }
}

int Menu::validateChoice(int &choice, int size)
{
    std::cout << "Seleccione una opcion: ";
    if (!(std::cin >> choice) || choice < 1 || choice > size)
    {
        //std::cin.clear();
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Opcion invalida. Intente nuevamente.\n";
        return -1; // Valor inválido
    }
    return choice;
}

void Menu::sellTicket()
{
    int choice, size = 2; // Número total de opciones

    cout << "\n+=======================+" << endl;
    cout << "|      VENTA DE BOLETOS |" << endl;
    cout << "+=======================+" << endl;

    cout << "1. Agregar usuario" << endl;
    cout << "2. Hacer compra" << endl;

    cout << "Seleccione una opcion: ";
    cin >> choice;

    // Validación de la opción
    while (choice < 1 || choice > size)
    {
        cout << "Opcion invalida. Intente nuevamente: ";
        cin >> choice;
    }

    // Procesar la opción seleccionada
    switch (choice)
    {
    case 1:
        crearUsuario();
        break;
    case 2:
        venta();
        break;
    default:
        cout << "Error inesperado. Intente nuevamente." << endl;
    }
}
void Menu::crearUsuario()
{

    string cedula, nombre, fechaNacimiento;

    cout << "\n+=======================+" << endl;
    cout << "|    REGISTRO DE USUARIO|" << endl;
    cout << "+=======================+" << endl;

    // Solicitar datos del usuario
    cout << "Ingrese la cedula del usuario: ";
    cin >> cedula;

    cout << "Ingrese el nombre del usuario: ";
    cin.ignore();
    getline(cin, nombre);

    cout << "Ingrese la fecha de nacimiento del usuario (DD/MM/AAAA): ";
    cin >> fechaNacimiento;

    // Redimensionar el arreglo si es necesario
    if (numUsuarios == capacidadUsuarios)
    {
        int nuevaCapacidad = (capacidadUsuarios == 0) ? 1 : capacidadUsuarios * 2;
        User *nuevoArreglo = new User[nuevaCapacidad];

        // Copiar los usuarios existentes al nuevo arreglo
        for (int i = 0; i < numUsuarios; i++)
        {
            nuevoArreglo[i] = usuarios[i];
        }

        // Liberar memoria del arreglo anterior
        delete[] usuarios;

        // Actualizar puntero y capacidad
        usuarios = nuevoArreglo;
        capacidadUsuarios = nuevaCapacidad;
    }

    // Agregar el nuevo usuario al arreglo
    usuarios[numUsuarios] = User(cedula, nombre, fechaNacimiento, 0);
    numUsuarios++;

    cout << "Usuario registrado exitosamente.\n";
}

void Menu::venta()
{
    if (event.getEventCount() == 0)
    {
        cout << "No hay eventos disponibles.\n";
        return;
    }

    // Mostrar eventos disponibles
    cout << "Seleccione un evento:\n";
    for (int i = 0; i < event.getEventCount(); i++)
    {
        cout << i + 1 << ". " << event.getEvents()[i].getName() << "\n";
    }

    int eventoSeleccionado = -1;
    int limiteEventos = event.getEventCount(); // getEven tiene in cin para gregar el evento
   
    eventoSeleccionado = validateChoice(eventoSeleccionado, limiteEventos); // revisa si el segmento esta ingresado o no de lo mcontraio manda error
    
    // Solicitar segmento
    // crea una variable para solicitar segmento
    Segment **segmentos = segment.getSegmentsByEvent();

    // Verificar que los segmentos no sean nulos
    if (segmentos == NULL)
    {
        cout << "Error: No se pudieron obtener los segmentos.\n";
        return;
    }

    // Obtener cantidad de segmentos
    int *segmentCounts = new int[event.getEventCount()];
    segment.getSegmentCount(event.getEvents()[eventoSeleccionado - 1], segmentCounts); // llena nunm segmentos con la cantidad de segmentos por

    int numSegmentos = segmentCounts[eventoSeleccionado - 1];
    delete[] segmentCounts; // Liberar memoria después de usar

    if (numSegmentos <= 0)
    {
        cout << "No hay segmentos disponibles para este evento.\n";
        return;
    }
    // Verificar y mostrar segmentos disponibles
    cout << "Segmentos disponibles:\n";
    for (int i = 0; i < numSegmentos; i++)
    {
        if (segmentos[eventoSeleccionado - 1][i].getName().empty())
        {
            cout << "Error: El segmento " << i + 1 << " no tiene un nombre válido.\n";
            return;
        }

        cout << i + 1 << ". " << segmentos[eventoSeleccionado - 1][i].getName()
             << " - Precio: " << segmentos[eventoSeleccionado - 1][i].getPrice() << "\n";
    }

    cout << "Linea 362-------------" << endl;
    // Validar la selección del segmento
    int segmentoSeleccionado = validateChoice(segmentoSeleccionado, numSegmentos);
    cout << "Linea 365-------------" << endl;
    if (segmentoSeleccionado == -1)
    {
        cout << "Selección inválida. Regresando al menú principal.\n";
        return;
    }

    // Obtener precio del segmento seleccionado
    float precioBase = segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getPrice();
    cout << "Precio del segmento seleccionado: " << precioBase << "\n";
}

User *Menu::buscarUsuarioPorCedula(const string &cedula)
{
    for (int i = 0; i < numUsuarios; i++)
    {
        if (usuarios[i].getCedula() == cedula)
        {                        // Comparar cédulas
            return &usuarios[i]; // Retorna un puntero al usuario encontrado
        }
    }
    return NULL; // No encontrado
}
