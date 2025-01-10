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
}

Event::~Event() {
    delete[] events;  
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
    while (!(cin >> input)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
    return input;
}

void Event::expandAndAssignEvents(Event& event, int& numEvents) {
    
    if (event.eventCount + numEvents > 0) {
        int newCapacity = event.eventCount + numEvents;

        Event* newEvents = new Event[newCapacity];
        for (int j = 0; j < event.eventCount; j++) {
            newEvents[j] = event.events[j];  
        }
        delete[] event.events;  
        event.events = newEvents;  
         
    }
}

void Event::saveEvent(Event& event) {

    int numEvents = 0;

    cout << "Cuantos eventos desea agregar?" << endl;
    numEvents = getValidIntInput();

    expandAndAssignEvents(event, numEvents);
    
    for (int i = 0; i < numEvents; i++) {

        string name, date, description;
        
        cout << endl << "Ingrese el nombre del evento " << event.eventCount + 1  << ":" << endl;
        cin.ignore();
        getline(cin, name);  

        do {
            cout << endl << "Ingrese la fecha del evento (por ejemplo: DD/MM/AAAA):" << endl;
            getline(cin, date);
            if (!isValidDate(date)) {
                cout << "Fecha invalida. Intente nuevamente." << endl;
            }
        } while (!isValidDate(date));

        cout << endl << "Ingrese la descripcion del evento " << event.eventCount + 1 << ":" << endl;
        getline(cin, description);

        event.events[event.eventCount].setName(name);  
        event.events[event.eventCount].setDate(date);
        event.events[event.eventCount].setDescription(description);
        event.eventCount++;  

        cout << "Evento guardado con exito." << endl << endl;
    }

    cout << "Se han guardado " << numEvents << " eventos correctamente." << endl << endl;
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

