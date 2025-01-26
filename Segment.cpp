#include "Segment.h"
#include <iostream>
#include <limits>

using namespace std;

Segment::Segment() {
    segmentName = "";
    rows = 0;
    seats = 0;
    price = 0;
    segmentCapacity = 2;
    numEvents = 0;
    index = 0;
}

Segment::~Segment() {
    // La lista se libera automáticamente con su destructor
}

string Segment::getName() { return segmentName; }

void Segment::setName(string& aSegmentName){ segmentName = aSegmentName; }

int Segment::getRows() { return rows; }

void Segment::setRows(int& aRows) { rows = aRows; }

int Segment::getSeats() { return seats; }

void Segment::setSeats(int& aSeats) { seats = aSeats; }

float Segment::getPrice() { return price; }

void Segment::setPrice(float& aPrice) { price = aPrice; }

List<int>& Segment::getSegmentCount() { return segmentCount; }

List<List<Segment>>& Segment::getSegmentsByEvent() { return segmentsByEvent; }

void Segment::manageSegments(Segment& segment, int& numEvents) {
    if (numEvents <= 0) {
        cout << "No hay eventos disponibles." << endl;
        return;
    }

    if (segmentsByEvent.getHead() == nullptr) {
        segmentCapacity = numEvents;  

        for (int i = 0; i < segmentCapacity; i++) {
            segmentsByEvent.insertAtEnd(List<Segment>());
            segmentCount.insertAtEnd(0);
        }
        return;
    }

    int currentCapacity = segmentCapacity;

    if (numEvents <= currentCapacity) {
        return;
    }

    for (int i = currentCapacity; i < numEvents; i++) {
        segmentsByEvent.insertAtEnd(List<Segment>());
        segmentCount.insertAtEnd(0);
    }

    segmentCapacity = numEvents;  
}

void Segment::addSegmentData(Segment& segment, int index) {
    string name;
    int rows, seats;
    float price;

    cout << "Ingrese el nombre del segmento " << index  << "." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    cout << "Ingrese el numero de filas:" << endl;
    rows = getValidIntInput();

    cout << "Ingrese la cantidad de asientos por fila:" << endl;
    seats = getValidIntInput();

    cout << "Ingrese el precio del segmento:" << endl;
    price = getValidFloatInput();

    segment.setName(name);
    segment.setRows(rows);
    segment.setSeats(seats);
    segment.setPrice(price);

    cout << "Segmento guardado con exito." << endl << endl;
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

bool Segment::askToAddSegments(int& eventIndex) {
    char addSegments;

    while (true) {
        cout << endl << "Desea agregar segmentos al evento " << eventIndex << "? (S/N): ";
        cin >> addSegments;

        if (addSegments == 'S' || addSegments == 's') {
            return true;  
        }
        if (addSegments == 'N' || addSegments == 'n') {
            return false;  
        }

        cout << "Opcion no valida. Por favor ingrese 'S' o 'N'." << endl;
    }
}

void Segment::saveSegments(Segment& segment, int events)
{
    numEvents = events;

    if (numEvents == 0) {
        cout << "No hay eventos disponibles para asignar segmentos." << endl ;
        cout << "Presione Enter para continuar..."<< endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cin.get(); 
        return;
    }

    manageSegments(segment, numEvents);

    List<List<Segment>>::NodePtr eventNode = segmentsByEvent.getHead();
    List<int>::NodePtr countNode = segmentCount.getHead();

    for (int i = 1; i <= numEvents; i++) {
        if (countNode->data > 0) {
            cout << "El evento " << i << " ya tiene segmentos asignados." << endl << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cin.get(); 
            continue; 
        }

        bool shouldAddSegments = askToAddSegments(i);

        if (!shouldAddSegments) {
            continue;
        }

        int numSegments = 0;
        
        cout << "Cuantos segmentos desea agregar al evento " << i << "?" << endl;
        numSegments = getValidIntInput();

        countNode->data = numSegments;

        for (int j = 1; j <= numSegments; j++) {
            Segment newSegment;
            addSegmentData(newSegment, j);
            eventNode->data.insertAtEnd(newSegment);
        }

        eventNode = eventNode->next;
        countNode = countNode->next;
    }
}


int Segment::getValidIntInput() {
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

float Segment::getValidFloatInput() {
    float input;
    while (true) {
        if (cin >> input && input > 0) {
            return input; 
        }
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "Por favor, ingrese un numero valido." << endl;
    }
}
