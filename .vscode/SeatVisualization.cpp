// #include "SeatVisualization.h"
// #include <iostream>
// using namespace std;

// SeatVisualization::SeatVisualization() {
//     amountOfRows = 0;
//     amountOfColumns = 0;
//     isSeatPurchased = NULL; // Corregido
// }

// void SeatVisualization::initializestadium() {
//     if (amountOfRows <= 0 || amountOfColumns <= 0) {
//         cout << "Error: Dimensiones inválidas para el estadio.\n";
//         return;
//     }

//     // Asignar memoria para la matriz
//     isSeatPurchased = new bool*[amountOfRows];
//     for (int i = 0; i < amountOfRows; i++) {
//         isSeatPurchased[i] = new bool[amountOfColumns](); // Inicializa automáticamente en `false`
//     }
// }

