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
    events = nullptr; 
    purchasesByUser = nullptr; 
    userIds=nullptr;
}

Event::~Event() {
    delete[] events;
    delete[] purchasesByUser;
    delete[] userIds;  
}

string Event::getName()  { return name;}

void Event::setName( string& aName) {name = aName;}

string Event::getDate()  { return date;}

void Event::setDate(string& aDate) { date = aDate;}

string Event::getDescription() { return description;}

void Event::setDescription(string& aDescription) { description = aDescription;}

Event* Event::getEvents() { return events; }

int Event::getEventCount(){ return eventCount; };

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

void Event::expandAndAssignEvents(Event& event, int& numEvents) {
    
    if (numEvents <= 0) {
        cout << "El número de eventos debe ser mayor a cero." << endl;
        return;
    }

    
    int newCapacity = event.eventCount + numEvents;

    
    Event* newEvents = new Event[newCapacity];

   
    for (int j = 0; j < event.eventCount; j++) {
        newEvents[j] = event.events[j];
    }

   
    delete[] event.events;

    event.events = newEvents;
}

void Event::initializeTracking(int maxUsers) {

    purchasesByUser = new int[maxUsers];
    userIds = new string[maxUsers];
    for (int i = 0; i < maxUsers; i++) {
        purchasesByUser[i] = 0;
        userIds[i] = "";
    }
}

bool Event::purchaseTickets(const string& userId, int numTickets) {

    for (int i = 0; i < 100; i++) {
        if (userIds[i] == userId) {
            if (purchasesByUser[i] + numTickets > 5) {
                cout << "No puedes comprar más de 5 boletos para este evento.\n";
                return false;
            }
            purchasesByUser[i] += numTickets;
            return true;
        }
        if (userIds[i] == "") {
            userIds[i] = userId;
            purchasesByUser[i] = numTickets;
            return true;
        }
    }
    cout << "No se pudo registrar la compra. Máximo de usuarios alcanzado.\n";
    return false;
}

int Event::getTicketsPurchasedByUser(const string& userId) {
    for (int i = 0; i < 100; i++) {
        if (userIds[i] == userId) {
            return purchasesByUser[i];
        }
    }
    return 0;
}

void Event::saveEvent(Event& event) {
    int numEvents = 0;

    cout <<"Cuantos eventos desea agregar?" << endl;
    numEvents = getValidIntInput();

    expandAndAssignEvents(event, numEvents);

    for (int i = 0; i < numEvents; i++) {
        string name, date, description;

        cout << "\nIngrese el nombre del evento " << event.eventCount + 1 << ": ";
        cin.ignore();
        getline(cin, name);

        do {
            cout << "\nIngrese la fecha del evento (DD/MM/AAAA): ";
            getline(cin, date);
            if (!isValidDate(date)) {
                cout << "Fecha invalida. Intente nuevamente.\n";
            }
        } while (!isValidDate(date));

        cout << "\nIngrese la descripcion del evento " << event.eventCount + 1 << ": ";
        getline(cin, description);

        event.events[event.eventCount].setName(name);
        event.events[event.eventCount].setDate(date);
        event.events[event.eventCount].setDescription(description);

        event.events[event.eventCount].initializeTracking(100);

        event.eventCount++;
        cout << "Evento guardado con exito.\n";
    }
    cout << "\nPresione Enter para continuar...";
    cin.get();
}


bool Event::isValidDate(string &date) {
    
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

