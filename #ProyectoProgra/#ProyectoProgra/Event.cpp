#include "Event.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

Event::Event() {
    name = "";
    date = "";
    description = "";
    eventCount = 0;
}

Event::~Event() {
    // La lista se liberará automáticamente con su destructor
}

string Event::getName() { return name; }

void Event::setName(string& aName) { name = aName; }

string Event::getDate() { return date; }

void Event::setDate(string& aDate) { date = aDate; }

string Event::getDescription() { return description; }

void Event::setDescription(string& aDescription) { description = aDescription; }

List<Event>& Event::getEvents() { return events; }

int Event::getEventCount() { return eventCount; }

int Event::getValidIntInput()
{
    int input;
    while (true) {
        if (cin >> input && input > 0) {
            return input;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
}

void Event::assignEvents(Event& event, int numEvents) {

    if (numEvents <= 0) {
        cout << "El número de eventos debe ser mayor a cero." << endl;
        return;
    }

    for (int i = 1; i <= numEvents; i++) {
        event.events.insertAtEnd(Event());
    }

    event.eventCount += numEvents;
}

void Event::initializeTracking(int maxUsers) {

    for (int i = 0; i < maxUsers; i++) {
        purchasesByUser.insertAtEnd(0);
        userIds.insertAtEnd("");
    }
}

bool Event::purchaseTickets(const string& userId, int numTickets) {
    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    while (userNode) {
        if (userNode->data == userId) {
            if (purchaseNode->data + numTickets > 5) {
                cout << "No puedes comprar más de 5 boletos para este evento.\n";
                return false;
            }
            purchaseNode->data += numTickets;
            return true;
        }
        if (userNode->data == "") {
            userNode->data = userId;
            purchaseNode->data = numTickets;
            return true;
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }

    cout << "No se pudo registrar la compra. Máximo de usuarios alcanzado.\n";
    return false;
}

int Event::getTicketsPurchasedByUser(const string& userId) {

    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    while (userNode) {
        if (userNode->data == userId) {
            return purchaseNode->data;
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }
    return 0;
}

void Event::saveEvent(Event& event, Segment& segment)
{
    int numNuevosEventos = 0;
    cout << "Cuantos eventos desea agregar?" << endl;
    numNuevosEventos = getValidIntInput();


    int oldEventCount = event.getEventCount();


    assignEvents(event, numNuevosEventos);


    List<Event>::NodePtr currentEvent = event.events.getHead();
    for (int i = 1; i <= oldEventCount; i++) {
        currentEvent = currentEvent->next;
    }

    for (int i = oldEventCount + 1; i <= event.getEventCount(); i++) {
        string name, date, description;

        name = getValidStringInput("\nIngrese el nombre del evento " + to_string(i) + ": \n ");

        bool validDate = false;
        while (!validDate) {
            cout << "\nIngrese la fecha del evento (DD/MM/YYYY): ";
            getline(cin, date);
            if (!isValidDate(date)) {
                cout << "La fecha ingresada no es valida. Por favor intente nuevamente.\n";
            }
            else {
                validDate = true;
            }
        }

        description = getValidStringInput("\nIngrese la descripción del evento: ");


        currentEvent->data.setName(name);
        currentEvent->data.setDate(date);
        currentEvent->data.setDescription(description);
        currentEvent->data.initializeTracking(100);

        segment.saveSegments(segment, event.getEventCount(), i);


        currentEvent = currentEvent->next;
    }
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


bool Event::isValidDate(string& date) {

    if (date.length() != 10) return false;

    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5) && date[i] != '/') return false;
        if (i != 2 && i != 5 && (date[i] < '0' || date[i] > '9')) return false;
    }

    if (date[6] != '2' || date[7] != '0') return false;

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}

bool Event::cancelTickets(const string& userId, int numTickets) {
    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    while (userNode) {
        if (userNode->data == userId) {
            if (purchaseNode->data >= numTickets) {
                purchaseNode->data -= numTickets;
                return true;
            }
            else {
                cout << "Error: El usuario no tiene suficientes boletos para cancelar.\n";
                return false;
            }
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }

    cout << "Error: Usuario no encontrado.\n";
    return false;
}
string Event::getValidStringInput(const string& prompt) {
    string input;

    while (true) {
        cout << prompt;
        getline(cin >> ws, input);

        // Validar que la entrada no esté vacía ni contenga solo espacios
        if (!input.empty() && input.find_first_not_of(' ') != string::npos) {
            return input;  // Entrada válida
        }

        cout << "La entrada no puede estar vacia.\n";
    }
}

