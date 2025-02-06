#include "Seating.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <limits>

using namespace std;

Seating::Seating() {

    segmentNumber = 0;
    numberOfRows = 0;
    numberOfColumns = 0;
    cost = 0.0f;
    seatPurchased = nullptr;
    initializeRoom();
}

void Seating::initializeRoom() {

    if (numberOfRows <= 0 || numberOfColumns <= 0)
        return;


    if (seatPurchased) {
        for (int i = 0; i < numberOfRows; i++) {
            delete[] seatPurchased[i];
        }
        delete[] seatPurchased;
        seatPurchased = nullptr;
    }

    seatPurchased = new bool* [numberOfRows];
    for (int i = 0; i < numberOfRows; i++) {
        seatPurchased[i] = new bool[numberOfColumns];
    }
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            seatPurchased[i][j] = false;
        }
    }
}

Seating::Seating(const Seating& other) {

    segmentNumber = other.segmentNumber;
    numberOfRows = other.numberOfRows;
    numberOfColumns = other.numberOfColumns;
    cost = other.cost;


    if (other.seatPurchased) {

        seatPurchased = new bool* [numberOfRows];
        for (int i = 0; i < numberOfRows; i++) {

            seatPurchased[i] = new bool[numberOfColumns];

            for (int j = 0; j < numberOfColumns; j++) {
                seatPurchased[i][j] = other.seatPurchased[i][j];
            }
        }
    }
    else {
        seatPurchased = nullptr;
    }
}

Seating& Seating::operator=(const Seating& other) {

    if (this != &other) {

        if (seatPurchased) {
            for (int i = 0; i < numberOfRows; i++) {
                delete[] seatPurchased[i];
            }
            delete[] seatPurchased;
        }


        segmentNumber = other.segmentNumber;
        numberOfRows = other.numberOfRows;
        numberOfColumns = other.numberOfColumns;
        cost = other.cost;


        if (other.seatPurchased) {
            seatPurchased = new bool* [numberOfRows];
            for (int i = 0; i < numberOfRows; i++) {
                seatPurchased[i] = new bool[numberOfColumns];
                for (int j = 0; j < numberOfColumns; j++) {
                    seatPurchased[i][j] = other.seatPurchased[i][j];
                }
            }
        }
        else {
            seatPurchased = nullptr;
        }
    }
    return *this;
}

void Seating::displaySeats() {

    char color[] = "\033[37m";
    char topLeft = 201, topRight = 187, bottomLeft = 200, bottomRight = 188;
    char horizontalLine = 205, verticalLine = 186, screenBlock = 178;
    char leftPad = 222, rightPad = 221, centerPad = 219, backPad = 223;

    cout << "\t\033[32m Verde: Disponible \n";
    cout << "\t\033[31m Rojo: Vendido \n\n";

    cout << "\t\033[37m";
    for (int k = 65; k < 65 + numberOfColumns; k++) {
        cout << "   " << char(k) << "   ";
    }
    cout << "\n\t" << topLeft;

    for (int k = 0; k < numberOfColumns * 7; k++) {
        cout << horizontalLine;
    }
    cout << topRight << "\n";

    cout << "\t" << verticalLine;
    for (int i = 0; i < numberOfColumns * 2; i++) {
        cout << " ";
    }
    for (int p = 0; p < numberOfColumns * 3; p++) {
        cout << screenBlock;
    }
    for (int i = 0; i < numberOfColumns * 2; i++) {
        cout << " ";
    }
    cout << verticalLine << "\n";

    for (int j = 0; j < numberOfRows * 3 + 1; j++) {
        cout << "\t\033[37m" << verticalLine;
        if (j % 3 == 0) {
            for (int k = 0; k < numberOfColumns * 7; k++) {
                cout << " ";
            }
        }
        else if (j % 3 == 1) {
            for (int k = 0; k < numberOfColumns; k++) {
                if (seatPurchased && seatPurchased[(j - 1) / 3][k]) {
                    color[3] = '1';
                }
                else {
                    color[3] = '2';
                }
                cout << color;
                cout << "  " << leftPad << centerPad << rightPad << "  ";
            }
        }
        else {
            for (int k = 0; k < numberOfColumns; k++) {
                cout << "   " << backPad << "   ";
            }
        }
        cout << "\033[37m" << verticalLine;

        if (j % 3 == 1) {
            cout << "     " << (j - 1) / 3 + 1;
        }
        cout << "\n";
    }

    cout << "\t\033[37m" << bottomLeft;
    for (int p = 0; p < numberOfColumns * 7; p++) {
        cout << horizontalLine;
    }
    cout << bottomRight << "\n";
}

bool Seating::isRoomFull() {

    return (cost > 0 && numberOfRows > 0 && numberOfColumns > 0);
}

bool Seating::isRoomComplete() {

    return isRoomFull();
}

float Seating::getCost() const {
    return cost;
}

void Seating::setCost(float newCost) {
    cost = newCost;
}

int Seating::getSegmentNumber() const {
    return segmentNumber;
}

void Seating::setSegmentNumber(int segNum) {
    segmentNumber = segNum;
}

int Seating::getNumberOfRows() const {
    return numberOfRows;
}

void Seating::setNumberOfRows(int rows) {
    numberOfRows = rows;
}

int Seating::getNumberOfColumns() const {
    return numberOfColumns;
}

void Seating::setNumberOfColumns(int cols) {
    numberOfColumns = cols;
}

bool** Seating::getSeatPurchased() const {

    return seatPurchased;
}

void Seating::setSeatPurchased(bool** seats) {

    seatPurchased = seats;
}

void Seating::sellField(int row, int column) {

    if (row < 0 || row >= numberOfRows ||
        column < 0 || column >= numberOfColumns) {
        cout << "\nError: Coordenadas fuera de rango.\n";
        return;
    }
    if (seatPurchased[row - 1][column - 1]) {
        cout << "\nError: El asiento ya esta ocupado.\n";
        return;
    }
    seatPurchased[row - 1][column - 1] = true;
    cout << "\nAsiento en fila " << (row + 1)
        << " y columna " << char('A' + column)
        << " vendido exitosamente.\n";
}

void Seating::checkSales(Event& event, Segment& segment,
    map<tuple<int, int>, Seating>& seatingMap)
{
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return;
    }

    cout << "\nEventos disponibles:\n";
    for (int i = 0; i < event.getEventCount(); i++) {
        cout << i + 1 << ". " << event.getEvents().getAt(i + 1).getName() << "\n";
    }

    int selectedEvent = 0;
    int size = event.getEventCount();
    selectedEvent = validateChoice(selectedEvent, size);

    List<List<Segment>>& segments = segment.getSegmentsByEvent();

    if (segments.getHead() == nullptr) {

        cin.get();
        return;
    }

    List<int>& segmentCounts = segment.getSegmentCount();

    if (segmentCounts.getHead() == nullptr) {
        cout << "Error: No se pudo obtener el conteo de segmentos.\n";
        cout << "\nPresione Enter para continuar...";
        cin.get();
        return;
    }

    int numSegments = segmentCounts.getAt(selectedEvent);

    if (numSegments <= 0) {
        cout << "No hay segmentos disponibles para este evento.\n";
        cout << "\nPresione Enter para continuar...";
        cin.get();
        return;
    }

    cout << "\nSegmentos disponibles para el evento \""
        << event.getEvents().getAt(selectedEvent).getName() << "\":\n";

    for (int i = 0; i < numSegments; i++) {
        cout << i + 1 << ". "
            << segments.getAt(selectedEvent).getAt(i + 1).getName()

            << " - Precio: "
            << segments.getAt(selectedEvent).getAt(i + 1).getPrice() << "\n";
    }

    int selectedSegment, option = 0;
    cout << "\nSeleccione un segmento: ";
    selectedSegment = validateChoice(option, numSegments);

    auto seatingKey = make_tuple(selectedEvent, selectedSegment);

    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        cout << "\nNo se han vendido asientos para este segmento.\n\n";

        int rows = segments.getAt(selectedEvent).getAt(selectedSegment).getRows();

        int columns = segments.getAt(selectedEvent).getAt(selectedSegment).getSeats();

        float price = segments.getAt(selectedEvent).getAt(selectedSegment).getPrice();

        Seating tempSeating;
        tempSeating.setNumberOfRows(rows);
        tempSeating.setNumberOfColumns(columns);
        tempSeating.setCost(price);
        tempSeating.initializeRoom();

        cout << "\nVista de la sala vacia (sin ventas):\n";
        tempSeating.displaySeats();
    }
    else {
        Seating& seating = seatingMap[seatingKey];
        cout << "\nRepresentacion grafica del segmento \""
            << segments.getAt(selectedEvent).getAt(selectedSegment).getName()
            << "\":\n";
        seating.displaySeats();
    }

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int Seating::validateChoice(int& choice, int& size)
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
        cout << "Entrada invalida. Intente de nuevo." << endl;
    }
}

Seating::~Seating() {
    if (seatPurchased) {
        for (int i = 0; i < numberOfRows; i++) {
            delete[] seatPurchased[i];
        }
        delete[] seatPurchased;
        seatPurchased = nullptr;
    }
}

bool Seating::freeSeat(int row, int col)
{
    if (!seatPurchased) {
        return false;
    }
    if (row < 0 || row >= numberOfRows || col < 0 || col >= numberOfColumns) {
        return false;
    }
    if (seatPurchased[row][col]) {
        seatPurchased[row][col] = false;
        return true;
    }

    return false;
}