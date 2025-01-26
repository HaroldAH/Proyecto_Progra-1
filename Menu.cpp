
#include "Menu.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

void Menu::showMenu()
{
    while (true)
    {
        system("cls");
        int option, size = 7;

        cout << "\033[31m";
        cout << "  __| |______________________________________________| |__\n";
        cout << "  __   ______________________________________________   __\n";
        cout << "    | |                                              | |\n";
        cout << "    | | _____     _            _ _                   | |\n";
        cout << "    | || ____|___| |_ __ _  __| (_) ___              | |\n";
        cout << "    | ||  _| / __| __/ _` |/ _` | |/ _ \\             | |\n";
        cout << "    | || |___\\__ \\ || (_| | (_| | | (_) |            | |\n";
        cout << "    | ||_____|___/\\__\\__,_|\\__,_|_|\\___/             | |\n";
        cout << "    | | ____              _   _                      | |\n";
        cout << "    | |/ ___|  __ _ _ __ | |_(_) __ _  __ _  ___     | |\n";
        cout << "    | |\\___ \\ / _` | '_ \\| __| |/ _` |/ _` |/ _ \\    | |\n";
        cout << "    | | ___) | (_| | | | | |_| | (_| | (_| | (_) |   | |\n";
        cout << "    | ||____/ \\__,_|_| |_|\\__|_|\\__,_|\\__, |\\___/    | |\n";
        cout << "    | || __ )  ___ _ __ _ __   __ _| ||___//_/ _   _ | |\n";
        cout << "    | ||  _ \\ / _ \\ '__| '_ \\ / _` | '_ \\ / _ \\ | | || |\n";
        cout << "    | || |_) |  __/ |  | | | | (_| | |_) |  __/ |_| || |\n";
        cout << "    | ||____/ \\___|_|  |_| |_|\\__,_|_.__/ \\___|\\__,_|| |\n";
        cout << "  _ | |______________________________________________| |__\n";
        cout << "  __   ______________________________________________   __\n";
        cout << "    | |                                              | |\n";
        cout << "\033[37m";

        cout << "               +==========================+\n";
        cout << "                |     Menu Principal      |\n";
        cout << "                +==========================+\n";
        cout << "                | 1. Configurar Evento    |\n";
        cout << "                | 2. Configurar Descuentos|\n";
        cout << "                | 3. Vender Entrada       |\n";
        cout << "                | 4. Consultar Ventas     |\n";
        cout << "                | 5. Acerca de            |\n";
        cout << "                | 6. Pregunta y Respuestas|\n";
        cout << "                | 7. Salir                |\n";
        cout << "                +==========================+\n";

        validateChoice(option, size);

        if (option == 1)
        {
            configureEvent(event, segment);
            continue;
        }

        if (option == 2)
        {
            manageCodes(discount);
            continue;
        }

        if (option == 3)
        {
            sellTicket(user);
            continue;
        }

        if (option == 4)
        {
            seating.checkSales(event, segment, seatingMap);
            continue;
        }

        if (option == 5)
        {
            showAbout();
            continue;
        }

        if (option == 6)
        {
            showFaq();
            continue;
        }

        if (option == 7)
        {
            cout << "Saliendo del programa. Gracias!" << endl;
            cin.get();
            return;
        }
    }
}

int Menu::validateChoice(int &choice, int &size)
{
    while (true)
    {
        cout << "Seleccione una opcion: " << endl;
        if (cin >> choice && choice >= 1 && choice <= size)
        {
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida." << endl;
    }
}

void Menu::configureDiscounts(Discount &discount)
{
    discount.configureDiscounts();
}

void Menu::configureEvent(Event &event, Segment &segment)
{
    while (true)
    {
        system("cls");
        int choice, size = 4;
        cout << "\n+=======================+" << endl;
        cout << "|   CONFIGURAR EVENTOS  |" << endl;
        cout << "+=======================+" << endl;
        cout << "1. Agregar nuevo evento" << endl;
        cout << "2. Modificar segmentos" << endl;
        cout << "3. Mostrar eventos actuales" << endl;
        cout << "4. Volver al menu principal" << endl;

        validateChoice(choice, size);

        if (choice == 1)
        {
            saveEvent(event, segment);
            continue;
        }

        if (choice == 2)
        {
            segment.saveSegments(segment, updateSegmentEventCount(event));
            continue;
        }

        if (choice == 3)
        {
            listEventAndSegments(event, segment);
            continue;
        }

        if (choice == 4)
        {
            cin.get();
            return;
        }
    }
}

void Menu::listEventAndSegments(Event &event, Segment &segment)
{
    system("cls");

    if (event.getEventCount() == 0)
    {
        cout << "No hay eventos guardados." << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    cout << "\nLista de eventos y segmentos:\n" << endl;

    List<List<Segment>>& segmentsByEvent = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();

    for (int i = 1; i <= event.getEventCount(); i++)
    {
        cout << "Evento #" << i << ":\n";  
        cout << "Nombre: " << event.getEvents().getAt(i).getName() << "\n";
        cout << "Fecha: " << event.getEvents().getAt(i).getDate() << "\n";
        cout << "Descripcion: " << event.getEvents().getAt(i).getDescription() << "\n";

        cout << "Segmentos asociados:\n";

        if (segmentCounts.getAt(i) == 0 || segmentsByEvent.getAt(i).getHead() == nullptr)
        {
            cout << " No tiene segmentos asignados.\n";
            cout << "--------------------------\n";
            continue;
        }

        for (int j = 1; j <= segmentCounts.getAt(i); j++)
        {
            cout << "  Segmento #" << j << ":\n";
            cout << "    Nombre: " 
                 << segmentsByEvent.getAt(i).getAt(j).getName() << "\n";
            cout << "    Filas: " 
                 << segmentsByEvent.getAt(i).getAt(j).getRows() << "\n";
            cout << "    Asientos por fila: " 
                 << segmentsByEvent.getAt(i).getAt(j).getSeats() << "\n";
            cout << "    Precio: " 
                 << segmentsByEvent.getAt(i).getAt(j).getPrice() << "\n";
        }

        cout << "--------------------------\n";
    }

    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


void Menu::manageCodes(Discount &discount)
{
    while (true)
    {
        system("cls");
        int choice, size = 4;
        cout << "\n+=======================+" << endl;
        cout << "|     GESTION DE CODIGOS|" << endl;
        cout << "+=======================+" << endl;
        cout << "1. Generar codigos" << endl;
        cout << "2. Mostrar registro" << endl;
        cout << "3. Eliminar un codigo" << endl;  // Nueva opción para eliminar descuentos
        cout << "4. Volver al menu principal" << endl;

        validateChoice(choice, size);

        if (choice == 1)
        {
            configureDiscounts(discount);
            continue;
        }

        if (choice == 2)
        {
            discount.showCodes();
            continue;
        }

        if (choice == 3)
        {
            discount.deleteDiscount();
            continue;
        }

        if (choice == 4)
        {
            return;
        }
    }
}


void Menu::sellTicket(User &user)
{
    sale.sell(user, event, segment, seatingMap, discount);
}

int Menu::updateSegmentEventCount(Event &event)
{
    int events = event.getEventCount();
    return events;
}

void Menu::saveEvent(Event &event, Segment &segment)
{
    int events = event.getEventCount();
    event.saveEvent(event);
}

void Menu::showAbout()
{
    system("cls");
    cout << "\n=========================================\n";
    cout << "               ACERCA DE                 \n";
    cout << "=========================================\n";
    cout << "Integrantes del Proyecto:\n";
    cout << "  - Harold Avila Hernandez\n";
    cout << "  - Cristhian Cordero Varela\n";
    cout << "=========================================\n";
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void Menu::showFaq()
{

    faq.showFaq();
}