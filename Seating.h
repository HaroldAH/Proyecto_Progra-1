#ifndef SEATING_H
#define SEATING_H

#include <string>
#include <map>
#include <tuple>
#include <limits>
#include "Field.h"
#include "Event.h"
#include "Segment.h"

class Seating {
private:
    float cost; 
    int segmentNumber;
    int numberOfRows;
    int numberOfColumns;
    bool **seatPurchased; 
    Field *fields;
    int numberOfFields;
    Event event;
    Segment segment;
     

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
    void checkSales(Event &event, Segment &segment, map<tuple<int, int>, Seating> &seatingMap);
    int validateChoice(int &choice, int &size);
};

#endif 
