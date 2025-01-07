
#include "Menu.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include "Utilities.h" 

using namespace std;

void Menu::showMenu() {
    int option = 0, size = 6;

    do {
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
        cout << "                | 6. Salir                |\n";
        cout << "                +==========================+\n";

        option = validateChoice(option, size);

        switch (option) {
        case 1:
            configureEvent(event,segment);
            break;
        case 2:
            configureDiscounts();
            break;
        case 3:
            sellTicket(user);
            break;
        case 4:
            checkSales(event);
            break;
        case 5:
            showAbout();
            break;
        case 6:
            cout << "Saliendo del programa. Gracias!" << endl;
            break;
        }
    } while (option != 6);
}

int Menu::validateChoice(int& choice, int& size) {
    while (true) {
        cout << "Seleccione una opcion: " << endl;
        if (cin >> choice && choice >= 1 && choice <= size) {
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente de nuevo." << endl;
    }
}

void Menu::configureDiscounts() {
    float discountPercentage;
    int discountCount;

    cout << "\n=== Configurar Descuentos ===" << endl;
    cout << "Ingrese el porcentaje de descuento: ";
    cin >> discountPercentage;

    cout << "Ingrese la cantidad de descuentos a generar: ";
    cin >> discountCount;

    discountManager.configure(discountPercentage, discountCount);

    discountManager.showCodes();
    cout << "RECUERDE EL CODIGO\n";
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Menu::configureEvent(Event& event, Segment& segment) {

    while (true) {

        int choice, size = 4;
        cout << "\n=== Configurar Eventos ===" << endl;
        cout << "1. Agregar nuevo evento" << endl;
        cout << "2. Modificar segmentos" << endl;
        cout << "3. Mostrar eventos actuales" << endl;
        cout << "4. Regresar al menu principal" << endl;
        
        validateChoice(choice, size);

        if (choice == 1) {
            saveEvent(event, segment); 
            continue;
        }

        if (choice == 2) {
            int events = updateSegmentEventCount(event);
            segment.saveSegments(segment, events);
            continue;
        }

        if (choice == 3) {
           listEventAndSegments(event, segment);
            continue;
        }

        if (choice == 4) {
            return;  
        }
        cout << endl << " Entrada invalida. Intente de nuevo." << endl << endl;
    }
}

void Menu::listEventAndSegments(Event& event, Segment& segment) {

    if (event.getEventCount() == 0) {
        cout << "No hay eventos guardados." << endl;
        return;  
    }

    cout << endl << "Lista de eventos y segmentos:" << endl << endl;

    Segment** segmentsByEvent = segment.getSegmentsByEvent();

    for (int i = 0; i < event.getEventCount(); i++) {
        cout << "Evento #" << i + 1 << ":" << endl;
        cout << "Nombre: " << event.getEvents()[i].getName() << endl;  
        cout << "Fecha: " << event.getEvents()[i].getDate() << endl;
        cout << "Descripcion: " << event.getEvents()[i].getDescription() << endl;

        cout << "Segmentos asociados:" << endl << endl;
        int* segmentCounts = segment.getSegmentCount();

         if (segmentCounts == nullptr || segmentsByEvent[i] == nullptr || segmentCounts[i] == 0) {
            cout << "  No hay segmentos asociados para este evento." << endl;
            cout << "--------------------------" << endl;
            continue;
        }
        int numSegments = segmentCounts[i];

        for (int j = 0; j < numSegments; j++) {
            cout << "  Segmento #" << j + 1 << ":" << endl;
            cout << "    Nombre: " << segmentsByEvent[i][j].getName() << endl;
            cout << "    Filas: " << segmentsByEvent[i][j].getRows() << endl;
            cout << "    Asientos por fila: " << segmentsByEvent[i][j].getSeats() << endl;
            cout << "    Precio: " << segmentsByEvent[i][j].getPrice() << endl;
        }

        cout << "--------------------------" << endl;
    }
}

void Menu::sellTicket(User& user) {

    int choice, totalOptions = 2;

    cout << "\n+=======================+" << endl;
    cout << "|      VENTA DE BOLETOS |" << endl;
    cout << "+=======================+" << endl;

    cout << "1. Agregar usuario" << endl;
    cout << "2. Hacer compra" << endl;

    validateChoice(choice,totalOptions);

    while (choice < 1 || choice > totalOptions) {
        cout << "Opcion invalida. Intente nuevamente: ";
        cin >> choice;
    }

    switch (choice) {
    case 1:
        user.createUser(user);
        break;
    case 2:
        sell();
        break;
    default:
        cout << "Error inesperado. Intente nuevamente." << endl;
    }
}


void Menu::sell() {

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return;
    }

    string idNumber;
    User* currentUser = nullptr;

    while (true) {
        cout << "Ingrese su numero de cedula: ";
        cin >> idNumber;

        currentUser = user.searchUserById(idNumber);

        if (currentUser != nullptr) { 
            cout << "Usuario encontrado. Bienvenido, " << currentUser->getName() << ".\n";
            break;
        }

        cout << "Usuario no encontrado. Desea registrarse? (S/N): ";
        char option;
        cin >> option;

        if (tolower(option) == 's') {
            user.createUser(user); 
            currentUser = user.searchUserById(idNumber); 
            if (currentUser != nullptr) {
                cout << "Usuario registrado y encontrado. Bienvenido, " << currentUser->getName() << ".\n";
                break;
            } else {
                cout << "Error al registrar el usuario. Intente nuevamente.\n";
            }
        } else {
            cout << "Regresando al menu principal...\n";
            return;
        }
    }



    cout << "Seleccione un evento:\n";
    for (int i = 0; i < event.getEventCount(); i++) {
        cout << i + 1 << ". " << event.getEvents()[i].getName() << "\n";
    }

    int selectedEvent, totalEvents = event.getEventCount();
    validateChoice(selectedEvent, totalEvents);

    Segment** segments = segment.getSegmentsByEvent();
    if (!segments) {
        cout << "Error: No se pudieron obtener los segmentos.\n";
        return;
    }

    int* segmentCounts = new int[event.getEventCount()];
    segment.getSegmentCount();

    int numSegments = segmentCounts[selectedEvent - 1];
    delete[] segmentCounts;

    if (numSegments <= 0) {
        cout << "No hay segmentos disponibles para este evento.\n";
        return;
    }

    cout << "\nSegmentos disponibles:\n";
    for (int i = 0; i < numSegments; i++) {
        cout << i + 1 << ". " << segments[selectedEvent - 1][i].getName()
             << " - Precio: $" << fixed << setprecision(2) << segments[selectedEvent - 1][i].getPrice() << "\n";
    }

    int selectedSegment = validateChoice(selectedSegment, numSegments);
    auto seatingKey = make_tuple(selectedEvent - 1, selectedSegment - 1);

    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        Seating newSeating;
        auto& seg = segments[selectedEvent - 1][selectedSegment - 1];
        newSeating.setNumberOfRows(seg.getRows());
        newSeating.setNumberOfColumns(seg.getSeats());
        newSeating.setCost(seg.getPrice());
        newSeating.initializeRoom();
        seatingMap[seatingKey] = newSeating;
    }

    Seating& seating = seatingMap[seatingKey];
    seating.displaySeats();

    int currentTickets = currentUser->getTicketsPurchased();
    if (currentTickets >= 5) {
        cout << "Ya has comprado el numero maximo de 5 boletos.\n";
        return;
    }

    int numTickets;
    while (true) {
        cout << "\nCuantos boletos desea comprar? ";
        cin >> numTickets;

        if (cin.fail() || numTickets < 1 || numTickets > (5 - currentTickets)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Por favor, ingrese un numero valido.\n";
            continue;
        }
        if (currentUser->purchaseTickets(numTickets)) {
            break;
        }
        cout << "No se pudo completar la compra de boletos. Intente nuevamente.\n";
    }

    vector<pair<int, char>> purchasedSeats;
    for (int i = 0; i < numTickets; i++) {
        int row;
        char col;

        cout << "Ingrese la columna del asiento (A-" << char('A' + seating.getNumberOfColumns() - 1) << "): ";
        cin >> col;

        cout << "Ingrese la fila del asiento (1-" << seating.getNumberOfRows() << "): ";
        cin >> row;

        if (seating.getSeatPurchased()[row - 1][toupper(col) - 'A']) {
            cout << "Asiento ocupado. Elija otro.\n";
            i--;
        } else {
            seating.getSeatPurchased()[row - 1][toupper(col) - 'A'] = true;
            purchasedSeats.push_back({row, toupper(col)});
            cout << "Asiento reservado con exito.\n";
        }
    }

    string discountCode;
    float discountPercentage = 0;
    cout << "\nTiene un codigo de descuento? (S/N): ";
    char useDiscount;
    cin >> useDiscount;

    if (tolower(useDiscount) == 's') {
        cout << "Ingrese su codigo de descuento: ";
        cin.ignore();
        getline(cin, discountCode);

        if (discountManager.verifyCode(discountCode)) {
            discountPercentage = discountManager.getDiscountPercentage();
            cout << "Descuento aplicado: " << discountPercentage << "%\n";
        } else {
            cout << "Codigo de descuento invalido o ya utilizado.\n";
        }
    }

    float ticketPrice = seating.getCost();
    float totalCost = ticketPrice * numTickets;
    if (discountPercentage > 0) {
        totalCost -= totalCost * (discountPercentage / 100);
    }

    string cardNumber;
    while (true) {
        cout << "\nIngrese el numero de su tarjeta: ";
        cin >> cardNumber;
        if (cardNumber.length() >= 12 && cardNumber.find_first_not_of("0123456789") == string::npos) {
            break;
        }
        cout << "Numero de tarjeta invalido. Ingrese solo numeros con al menos 12 digitos.\n";
    }

  cout << "==================== FACTURA ====================\n";
cout << "Usuario: " << currentUser->getName() << endl;
cout << "Cedula: " << currentUser->getIdNumber() << endl;
cout << "Evento: " << event.getEvents()[selectedEvent - 1].getName() << endl;
cout << "Segmento: " << segments[selectedEvent - 1][selectedSegment - 1].getName() << endl;
cout << "Tarjeta: ****-****-****-" << cardNumber.substr(cardNumber.length() - 4) << endl;
cout << "Boletos: " << numTickets << " x $" << fixed << setprecision(2) << ticketPrice << endl;

if (discountPercentage > 0) {
    cout << "Descuento: " << discountPercentage << "%\n";
}

cout << "Asientos comprados: ";
for (const auto& seat : purchasedSeats) {
    cout << "(Fila: " << seat.first << ", Columna: " << seat.second << ") ";
}
cout << "\n";

cout << "Total pagado: $" << totalCost << "\n";
cout << "=================================================\n";


    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

/*User* Menu::searchUserById(const string& idNumber) {
    for (int i = 0; i < numUsers; i++) {
        if (users[i].getIdNumber() == idNumber) {
            return &users[i];
        }
    }
    return nullptr;
}*/

void Menu::checkSales(Event& event) {
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return;
    }

    cout << "\nEventos disponibles:\n";
    for (int i = 0; i < event.getEventCount(); i++) {
        cout << i + 1 << ". " << event.getEvents()[i].getName() << "\n";
    }

    int selectedEvent;
    cout << "\nSeleccione un evento para ver sus segmentos: ";
    int choice = 0;
    int size = event.getEventCount();
    selectedEvent = validateChoice(choice, size);

    Segment** segments = segment.getSegmentsByEvent();
    if (segments == nullptr) {
        cout << "Error: No se pudieron obtener los segmentos del evento.\n";
        return;
    }

    int* segmentCounts = new int[event.getEventCount()];
    segment.getSegmentCount();

    int numSegments = segmentCounts[selectedEvent - 1];
    delete[] segmentCounts;

    if (numSegments <= 0) {
        cout << "No hay segmentos disponibles para este evento.\n";
        return;
    }

    cout << "\nSegmentos disponibles para el evento \"" << event.getEvents()[selectedEvent - 1].getName() << "\":\n";
    for (int i = 0; i < numSegments; i++) {
        cout << i + 1 << ". " << segments[selectedEvent - 1][i].getName()
             << " - Precio: " << segments[selectedEvent - 1][i].getPrice() << "\n";
    }

    int selectedSegment, option;
    cout << "\nSeleccione un segmento para ver su representacion grafica: ";
     validateChoice(option, selectedSegment);

    auto seatingKey = std::make_tuple(selectedEvent - 1, selectedSegment - 1);

    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        Seating newSeating;
        int rows = segments[selectedEvent - 1][selectedSegment - 1].getRows();
        int columns = segments[selectedEvent - 1][selectedSegment - 1].getSeats();
        float price = segments[selectedEvent - 1][selectedSegment - 1].getPrice();

        newSeating.setNumberOfRows(rows);
        newSeating.setNumberOfColumns(columns);
        newSeating.setCost(price);
        newSeating.initializeRoom();

        seatingMap[seatingKey] = newSeating;
    }

    Seating& seating = seatingMap[seatingKey];
    cout << "\nRepresentacion grafica del segmento \"" << segments[selectedEvent - 1][selectedSegment - 1].getName() << "\":\n";
    seating.displaySeats();

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int Menu::updateSegmentEventCount(Event& event) {
    int events = event.getEventCount();
    return events;
}

void Menu::saveEvent(Event& event, Segment& segment){

int events = event.getEventCount();
event.saveEvent(event);
}

void Menu::showAbout()
{
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


