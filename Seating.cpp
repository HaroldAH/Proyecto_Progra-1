#include "Seating.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include "Field.h"
using namespace std;

Seating::Seating() {
    segmentNumber = 0;
    numberOfRows = 0;
    numberOfColumns = 0;
    cost = 0.0f;
    seatPurchased = NULL;
    fields = NULL;

    initializeRoom();
}

void Seating::initializeRoom() {
    
    seatPurchased = new bool*[numberOfRows];
    for (int i = 0; i < numberOfRows; i++) {
        seatPurchased[i] = new bool[numberOfColumns];
    }

    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            seatPurchased[i][j] = false;
        }
    }
}


void Seating::displaySeats() {
    char color[] = "\033[37m";
    char topLeft = 201, topRight = 187, bottomLeft = 200, bottomRight = 188;
    char horizontalLine = 205, verticalLine = 186, screenBlock = 178;
    char leftPad = 222, rightPad = 221, centerPad = 219, backPad = 223;

    cout << "\t\033[32m Verde: Disponible \n";
    cout << "\t\033[31m Rojo: Vendido \n";

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
        } else if (j % 3 == 1) {
            for (int k = 0; k < numberOfColumns; k++) {
                if (seatPurchased[(j - 1) / 3][k]) {
                    color[3] = '1';
                } else {
                    color[3] = '2';
                }
                cout << color;
                cout << "  " << leftPad << centerPad << rightPad << "  ";
            }
        } else {
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


float Seating::getCost()const {
    return cost;
}

void Seating::displayRoomInfo() {
    cout << "\n\t El numero de la sala es : " << segmentNumber;
    cout << "\n\t El costo de la sala es : " << fixed << setprecision(2) << cost << "$";
    cout << "\n\t El estado actual de la sala es : \n";
    displaySeats();
}

bool Seating::isRoomComplete() {
    return isRoomFull();
}


void Seating::setCost(float c) {
    cost = c;
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
    if (row < 0 || row >= numberOfRows || column < 0 || column >= numberOfColumns) {
        cout << "\nError: Coordenadas fuera de rango.\n";
        return;
    }

    if (seatPurchased[row][column]) {
        cout << "\nError: El asiento ya esta ocupado.\n";
        return;
    }

    seatPurchased[row][column] = true;
    cout << "\nAsiento en fila " << (row + 1) << " y columna " << char('A' + column) << " vendido exitosamente.\n";
}


bool Seating::hasNotBeenUsed(int fieldId) {
    for (int l = 0; l < numberOfFields; l++) {
        if (fields[l].getFieldId() == fieldId) {
            return true;
        }
    }
    return false;
}

void Seating::removeField(int position) {
    numberOfFields--;
    Field* aux = new Field[numberOfFields];

    for (int i = 0; i < position; i++) {
        aux[i] = fields[i];
    }
    for (int j = position + 1; j < numberOfFields; j++) {
        aux[j - 1] = fields[j];
    }

    delete[] fields;
    fields = aux;
}

void Seating::confirmReserve() {
    seatPurchased[fields[numberOfFields - 1].getSeatColumn() - 1][fields[numberOfFields - 1].getSeatRow() - 1] = true;
}

bool Seating::checkSeatStatus() {
    return seatPurchased[fields[numberOfFields - 1].getSeatColumn() - 1][fields[numberOfFields - 1].getSeatRow() - 1];
}

void Seating::finishTicket() {
    fields[numberOfFields - 1].fillField(numberOfColumns, numberOfRows);
}
