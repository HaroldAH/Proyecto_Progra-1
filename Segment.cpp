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
    int index = 0;
    segmentCount = nullptr;
    segmentsByEvent = nullptr;
}

Segment::~Segment() {
    if (segmentsByEvent != nullptr) {
        for (int i = 0; i < numEvents; i++) {
            if (segmentsByEvent[i] != nullptr) {
                delete[] segmentsByEvent[i];
            }
        }
        delete[] segmentsByEvent;
        delete[] segmentCount;
    }
}

string Segment::getName() { return segmentName; }

void Segment::setName(string& aSegmentName){ segmentName = aSegmentName;}

int Segment::getRows() { return rows; }

void Segment::setRows(int& aRows) { rows = aRows; }

int Segment::getSeats() { return seats; }

void Segment::setSeats(int& aSeats) { seats = aSeats; }

float Segment::getPrice() { return price; }

void Segment::setPrice(float& aPrice) { price = aPrice; }

Segment** Segment::getSegmentsByEvent() {
    if (segmentsByEvent == nullptr) {
        std::cout << "No hay segmentos asociados\n";
        return nullptr;
    }
    return segmentsByEvent;
}

int* Segment::getSegmentCount() { return segmentCount; }

void Segment::manageSegments(Segment& segment, int& numEvents) {

    if (numEvents <= 0) {
        cout << "No hay eventos disponibles." << endl;
        return;
    }

    if (segment.segmentsByEvent == nullptr) {
        segment.segmentsByEvent = new Segment*[numEvents];
        segment.segmentCount = new int[numEvents];

        for (int i = 0; i < numEvents; i++) {
            segment.segmentsByEvent[i] = nullptr;
            segment.segmentCount[i] = 0;
        }
        return;
    }

    
    int currentCapacity = segmentCapacity; 

    
    if (numEvents <= currentCapacity) {
        return;
    }

    
    int newCapacity = currentCapacity * 2; 
    if (newCapacity < numEvents) {
        newCapacity = numEvents; 
    }

    
    int* newSegmentCount = new int[newCapacity];
    Segment** newSegmentsByEvent = new Segment*[newCapacity];

    
    for (int i = 0; i < currentCapacity; i++) {
        newSegmentCount[i] = segment.segmentCount[i];
        newSegmentsByEvent[i] = segment.segmentsByEvent[i];
    }

    
    for (int i = currentCapacity; i < newCapacity; i++) {
        newSegmentsByEvent[i] = nullptr;
        newSegmentCount[i] = 0;
    }

   
    delete[] segment.segmentsByEvent;
    delete[] segment.segmentCount;

    
    segment.segmentsByEvent = newSegmentsByEvent;
    segment.segmentCount = newSegmentCount;
    segmentCapacity = newCapacity;
}

void Segment::addSegmentData(Segment& segment) {

    string name;
    int rows, seats;
    float price;

    cout << "Ingrese el nombre del segmento:" << endl;
    cin.ignore();
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

        cout << endl << "Desea agregar segmentos al evento " << eventIndex + 1 << "? (S/N): ";
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

void Segment::saveSegments(Segment& segment, int events) {

    numEvents = events;

    if (numEvents == 0) {
        cout << "No hay eventos disponibles para asignar segmentos." << endl << endl;
        return;
    }

    manageSegments(segment, numEvents);

    for (int i = 0; i < numEvents; i++) {

        if (segment.segmentsByEvent[i] != nullptr && segment.segmentCount[i] > 0) {
            cout << "El evento " << i + 1 << " ya tiene segmentos asignados. No se puede agregar mas segmentos." << endl << endl;
            continue; 
        }

        bool shouldAddSegments = askToAddSegments(i);

        if (!shouldAddSegments) {
            continue;  
        }

        int numSegments = 0;
        cout << "Cuantos segmentos desea agregar al evento " << i + 1 << "?" << endl;
        numSegments = getValidIntInput();

      
        if (numSegments <= 0) {
            cout << "Numero de segmentos invalido." << endl;
            continue;
        }

        segment.segmentCount[i] = numSegments;
        segment.segmentsByEvent[i] = new Segment[numSegments];  

        for (int j = 0; j < numSegments; j++) {
            addSegmentData(segment.segmentsByEvent[i][j]);  
        }
        cin.get();
    }
}

int Segment::getValidIntInput() {
    int input;
    while (!(cin >> input)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
    return input;
}

float Segment::getValidFloatInput() {
    float input;
    while (!(cin >> input)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
    return input;
}
