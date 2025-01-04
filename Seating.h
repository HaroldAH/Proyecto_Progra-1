#pragma once
#include <string>
#include "Field.h"

class Seating
{
private:
  int cost; // Cambiado a float para ser coherente con getCost()
  int segmentNumber;
  int amountOfRows;
  int amountOfColumns;
  bool **isSeatPurchased; // Matriz dinámica para indicar qué asientos están comprados
  Field *fields;
  int amountFields;

public:
  // Constructor
  Seating();

  // Getters y Setters
  int getCost() const; // Devuelve el costo
  void setCost(int c); // Asigna el costo

  int getSegmentNumber() const;
  void setSegmentNumber(int segNum);

  int getAmountOfRows() const;
  void setAmountOfRows(int rows);

  int getAmountOfColumns() const;
  void setAmountOfColumns(int cols);

  bool **getIsSeatPurchased() const;
  void setIsSeatPurchased(bool **seats);

  // Métodos principales
  void initializeRoom();
  void displaySeats();
  bool isRoomFull();

  // Métodos auxiliares
  float getCost(); // (Si quieres mantener esta firma adicional)

  bool verifyAnswer(int *number, int lowestOption, int highestOption);
  void displayRoomInfo();
  bool isRoomComplete();
  void sellField(int filas, int columnas);
  void removeField(int position);
  void confirmReserve();
  bool checkSeatStatus();
  void finishTicket();
  bool hasNotBeenUsed(int fieldId);
};
