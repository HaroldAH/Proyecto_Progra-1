#ifndef SEATING_H
#define SEATING_H

#include <string>
#include "Field.h"

class Seating {
private:
    float cost; // Cambiado a float para ser coherente con getCost()
    int segmentNumber;
    int numberOfRows;
    int numberOfColumns;
    bool **seatPurchased; // Matriz dinámica para indicar qué asientos están comprados
    Field *fields;
    int numberOfFields;

public:
    Seating();

    float getCost() const;
    void setCost(float c);

    int getSegmentNumber() const;
    void setSegmentNumber(int segNum);

    int getNumberOfRows() const;
    void setNumberOfRows(int rows);

    int getNumberOfColumns() const;
    void setNumberOfColumns(int cols);

    bool **getSeatPurchased() const;
    void setSeatPurchased(bool **seats);

    void initializeRoom();
    void displaySeats();
    bool isRoomFull();

    void displayRoomInfo();
    bool isRoomComplete();
    void sellField(int row, int column);
    void removeField(int position);
    void confirmReserve();
    bool checkSeatStatus();
    void finishTicket();
    bool hasNotBeenUsed(int fieldId);
};

#endif 
