#pragma once

class SeatVisualization{

  int amountOfRows;
  int amountOfColumns;
  bool isSeatPurchased;

  public:

SeatVisualization();
void initializestadium();
void displaySeats();
bool isRoomFull();
};