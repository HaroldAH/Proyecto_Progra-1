#ifndef SEAT_VISUALIZATION_H
#define SEAT_VISUALIZATION_H

class SeatVisualization {
private:
    int amountOfRows;
    int amountOfColumns;
    bool** isSeatPurchased;

public:
    SeatVisualization();
    void initializestadium();
    void displaySeats();
    bool isRoomFull();
};

#endif 
