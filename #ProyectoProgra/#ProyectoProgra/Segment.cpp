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
}

Segment::~Segment() {
    // La lista se libera automáticamente con su destructor
}

string Segment::getName() { return segmentName; }

void Segment::setName(string& aSegmentName) { segmentName = aSegmentName; }

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

    name = getValidStringInput("\nIngrese el nombre del segmento " + to_string(index) + ": \n");

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
}

void Segment::saveSegments(Segment& segment, int events, int eventIndex = 0)
{

    if (events == 0) {
        cout << "No hay eventos disponibles para asignar segmentos." << endl;
        return;
    }


    manageSegments(segment, events);

    if (eventIndex != 0) {

        List<List<Segment>>::NodePtr eventNode = segmentsByEvent.getHead();
        List<int>::NodePtr countNode = segmentCount.getHead();
        for (int i = 1; i < eventIndex; i++) {
            eventNode = eventNode->next;
            countNode = countNode->next;
        }
        if (countNode->data > 0) {

        }
        else {
            int numSegments = 0;
            do {
                cout << "Cuantos segmentos desea agregar al evento " << eventIndex << "?" << endl;
                numSegments = getValidIntInput();
                if (numSegments <= 0) {
                    cout << "Es necesario registrar segmentos para este evento." << endl;
                }
            } while (numSegments <= 0);

            countNode->data = numSegments;
            for (int j = 1; j <= numSegments; j++) {
                Segment newSegment;
                addSegmentData(newSegment, j);
                eventNode->data.insertAtEnd(newSegment);
            }
        }
        return;
    }
    else {

        List<List<Segment>>::NodePtr eventNode = segmentsByEvent.getHead();
        List<int>::NodePtr countNode = segmentCount.getHead();
        for (int i = 1; i <= events; i++) {
            if (countNode->data > 0) {

            }
            else {
                int numSegments = 0;
                do {
                    cout << "Cuantos segmentos desea agregar al evento " << i << "?" << endl;
                    numSegments = getValidIntInput();
                } while (numSegments <= 0);

                countNode->data = numSegments;
                for (int j = 1; j <= numSegments; j++) {
                    Segment newSegment;
                    addSegmentData(newSegment, j);
                    eventNode->data.insertAtEnd(newSegment);
                }
            }

            eventNode = eventNode->next;
            countNode = countNode->next;
        }
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

string Segment::getValidStringInput(const string& prompt) {
    string input;

    while (true) {
        cout << prompt;
        getline(cin >> ws, input);


        if (!input.empty() && input.find_first_not_of(' ') != string::npos) {
            return input;  // Entrada válida
        }

        cout << "La entrada no puede estar vacia.\n";
    }
}
