#include "SeatVisualization.h"
#include <iostream>
using namespace std;

SeatVisualization::SeatVisualization() {
    amountOfRows = 0;
    amountOfColumns = 0;
    isSeatPurchased = NULL; 
}

void SeatVisualization::initializestadium() {
    if (amountOfRows <= 0 || amountOfColumns <= 0) {
        cout << "Error: Dimensiones inválidas para el estadio.\n";
        return;
    }

    // Asignar memoria para la matriz
    isSeatPurchased = new bool*[amountOfRows];
    for (int i = 0; i < amountOfRows; i++) {
        isSeatPurchased[i] = new bool[amountOfColumns](); // Inicializa automáticamente en `false`
    }

void SeatVisualization::displaySeats() {
    // Chars para el cuadro
    char color[] = "\033[37m";
    char topLeft, topRight, bottomLeft, bottomRight, horizontalLine, verticalLine, screenBlock;
    {
        topLeft = 201;
        topRight = 187;
        bottomLeft = 200;
        bottomRight = 188;
        horizontalLine = 205;
        verticalLine = 186;
        screenBlock = 178;
    }

    // Chars para la silla
    char leftPad, rightPad, centerPad, backPad;
    {
        leftPad = 222;
        rightPad = 221;
        centerPad = 219;
        backPad = 223;
    }

    cout << "\t\033[32m Verde: Disponible \n";
    cout << "\t\033[31m Rojo: Vendido \n";

    // Imprimir parte de arriba
    cout << "\t\033[37m"; // Cambia el color del texto a blanco
    for (int k = 65; k < 65 + amountOfColumns; k++) {
        cout << "   " << static_cast<char>(k) << "   "; // Imprime las letras de las columnas
    }
    cout << "\n\t" << static_cast<char>(topLeft); // Esquina superior izquierda
    for (int k = 0; k < amountOfColumns * 7; k++) {
        cout << static_cast<char>(horizontalLine); // Línea horizontal
    }
    cout << static_cast<char>(topRight) << endl; // Esquina superior derecha

    // Imprimir "pantalla"
    cout << "\t\033[37m"; // Cambia el color del texto a blanco
    for (int k = 65; k < 65 + amountOfColumns; k++) {
        cout << "   " << static_cast<char>(k) << "   "; // Imprime las letras de las columnas
    }
    cout << "\n\t" << static_cast<char>(topLeft); // Esquina superior izquierda
    for (int k = 0; k < amountOfColumns * 7; k++) {
        cout << static_cast<char>(horizontalLine); // Línea horizontal
    }
    cout << static_cast<char>(topRight) << endl; // Esquina superior derecha

    // Imprimir asientos
    for (int j = 0; j < amountOfRows * 3 + 1; j++) {
        cout << "\t\033[37m" << static_cast<char>(verticalLine); // Imprime el borde izquierdo

        if (j % 3 == 0) {
            // Imprime una línea vacía
            for (int k = 0; k < amountOfColumns * 7; k++) {
                cout << " "; // Espacios en blanco
            }
        } else if (j % 3 == 1) {
            // Imprima la parte del medio del asiento
            for (int k = 0; k < amountOfColumns; k++) {
                if (isSeatPurchased[(j - 1) / 3][k]) {
                    color[3] = '1'; // Rojo
                } else {
                    color[3] = '2'; // Verde
                }

                cout << color; // Cambiar el color según el estado
                cout << "  " << static_cast<char>(leftPad)
                     << static_cast<char>(centerPad)
                     << static_cast<char>(rightPad) << "  ";
            }
        } else {
            // Imprima la parte de atrás
            for (int k = 0; k < amountOfColumns; k++) {
                cout << "   " << static_cast<char>(backPad) << "   ";
            }
        }

        cout << "\033[37m" << static_cast<char>(verticalLine); // Restablece el color y añade el borde derecho
        if (j % 3 == 1) {
            cout << "     " << (j - 1) / 3 + 1; // Número de fila
        }
        cout << endl;
    }


}


