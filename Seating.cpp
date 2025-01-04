#include "Seating.h"
#include <iostream>
#include <iomanip>
#include <cstdio>    // Para printf, sscanf, fgets
#include <cstring>   // Para strlen
#include "Field.h"
using namespace std;

Seating::Seating()
{
    segmentNumber   = 0;
    amountOfRows    = 0;
    amountOfColumns = 0;
    cost            = 0.0f;  // Inicializamos cost
    isSeatPurchased = NULL;
    fields=NULL;
    
    initializeRoom();
}

void Seating::initializeRoom()
{
    // Reservamos la memoria para las filas
    isSeatPurchased = new bool*[amountOfRows];
    for(int i = 0; i < amountOfRows; i++)
    {
        // En cada fila, reservamos para las columnas
        isSeatPurchased[i] = new bool[amountOfColumns];
    }

    // Inicializamos cada asiento en false (no comprado)
    for(int i = 0; i < amountOfRows; i++)
    {
        for(int j = 0; j < amountOfColumns; j++)
        {
            isSeatPurchased[i][j] = false;
        }
    }
}

void Seating::displaySeats()
{
    //Chars para el cuadro
    char color[] = "\033[37m";
    char topLeft, topRight, bottomLeft, bottomRight, horizontalLine, verticalLine, screenBlock;
    {
        topLeft       = 201;
        topRight      = 187;
        bottomLeft    = 200;
        bottomRight   = 188;
        horizontalLine= 205;
        verticalLine  = 186;
        screenBlock   = 178;
    }
    
    //Chars para la silla
    char leftPad, rightPad, centerPad, backPad;
    {
        leftPad   = 222;
        rightPad  = 221;
        centerPad = 219;
        backPad   = 223;
    }

    // Leyendas de colores
    printf("\t\033[32m Verde: Disponible \n");
    printf("\t\033[31m Rojo: Vendido \n");

    // Encabezado de columnas (A, B, C, ...)
    printf("\t\033[37m");
    for(int k = 65; k < 65 + amountOfColumns; k++)
    {
        printf("   %c   ", char(k));
    }
    printf("\n\t%c", topLeft);
    for(int k = 0; k < amountOfColumns * 7; k++)
        printf("%c", horizontalLine);
    printf("%c\n", topRight);

    // "Pantalla" (bloque o parte superior)
    printf("\t%c", verticalLine);
    for(int i = 0; i < amountOfColumns * 2; i++)
        printf(" ");
    for(int p = 0; p < amountOfColumns * 3; p++)
        printf("%c", screenBlock);
    for(int i = 0; i < amountOfColumns * 2; i++)
        printf(" ");
    printf("%c\n", verticalLine);
    
    // Imprimir asientos
    for(int j = 0; j < amountOfRows * 3 + 1; j++)
    {
        printf("\t\033[37m%c", verticalLine);

        if(j % 3 == 0)
        {
            // Línea vacía
            for(int k = 0; k < amountOfColumns * 7; k++)
                printf(" ");
        }
        else if(j % 3 == 1)
        {   
            // Parte del medio del asiento
            for(int k = 0; k < amountOfColumns; k++)
            {
                // Decide el color (si isSeatPurchased es true, asume '1' → Rojo)
                if(isSeatPurchased[(j - 1) / 3][k])
                    color[3] = '1';  // \033[31m
                else
                    color[3] = '2';  // \033[32m

                printf("%s", color);
                printf("  %c%c%c  ", leftPad, centerPad, rightPad);
            }
        }
        else
        {
            // Parte de atrás del asiento
            for(int k = 0; k < amountOfColumns; k++)
                printf("   %c   ", backPad);
        }

        printf("\033[37m%c", verticalLine);

        // Al final de cada fila (solo en la parte media) imprimimos el número
        if(j % 3 == 1)
            printf("     %d", (j - 1) / 3 + 1);

        printf("\n");
    }
    
    // Parte de abajo del cuadro
    printf("\t\033[37m%c", bottomLeft);
    for(int p = 0; p < amountOfColumns * 7; p++)
        printf("%c", horizontalLine);
    printf("%c\n", bottomRight);
}

bool Seating::isRoomFull()
{
    // Ajusta la lógica a lo que signifique "sala llena".
    // Aquí se asume que la sala "existe" si cost > 0 y tiene filas/columnas
    return (cost > 0 && amountOfRows > 0 && amountOfColumns > 0);
}

float Seating::getCost()
{
    return cost;
}

bool Seating::verifyAnswer(int* number, int lowestOption, int highestOption)
{
    // Aquí tu misma lógica
    char buffer[128];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return true; // error
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    if (sscanf(buffer, "%d", number) != 1)
        return true;

    if (*number < lowestOption || *number > highestOption)
        return true;

    return false; // Todo ok
}


void Seating::displayRoomInfo()
{
    printf("\n\t El numero de la sala es : %d", segmentNumber);
    printf("\n\t El costo de la sala es : %.2f$ \n", cost);
    printf("\n\t El estado actual de la sala es : \n");
    displaySeats();
}

bool Seating::isRoomComplete()
{
    return isRoomFull();
}
int Seating::getCost() const {
    return cost;
}
void Seating::setCost(int c) {
    cost = c;
}

int Seating::getSegmentNumber() const {
    return segmentNumber;
}
void Seating::setSegmentNumber(int segNum) {
    segmentNumber = segNum;
}

int Seating::getAmountOfRows() const {
    return amountOfRows;
}
void Seating::setAmountOfRows(int rows) {
    amountOfRows = rows;
}

int Seating::getAmountOfColumns() const {
    return amountOfColumns;
}
void Seating::setAmountOfColumns(int cols) {
    amountOfColumns = cols;
}

bool** Seating::getIsSeatPurchased() const {
    return isSeatPurchased;
}
void Seating::setIsSeatPurchased(bool** seats) {
    isSeatPurchased = seats;
}

void Seating::sellField(int filas, int columnas)
{
    // Validar si las coordenadas están dentro del rango de la matriz
    if (filas < 0 || filas >= amountOfRows || columnas < 0 || columnas >= amountOfColumns)
    {
        printf("\nError: Coordenadas fuera de rango.\n");
        return;
    }

    // Verificar si el asiento ya está ocupado
    if (isSeatPurchased[filas][columnas])
    {
        printf("\nError: El asiento ya está ocupado.\n");
        return;
    }

    // Marcar el asiento como vendido
    isSeatPurchased[filas][columnas] = 1;

    // Mostrar un mensaje de confirmación
    printf("\nAsiento en fila %d y columna %c vendido exitosamente.\n", filas + 1, char('A' + columnas));
}

bool Seating::hasNotBeenUsed(int fieldId)
{
    for(int l = 0; l < amountFields; l++)
        if (fields[l].getFieldId() == fieldId) return true;
    return false;
}

void Seating::removeField(int position)
{
   amountFields--;
    Field *aux = new Field[amountFields];

    for(int i = 0; i < position; i++)
    {
        aux[i] = fields[i];
    }
    for(int j = position+1; j < amountFields;j++)
    {
        aux[j] = fields[j];
    }

    delete[] fields;
    fields = aux;
}

void Seating::confirmReserve()
{
    isSeatPurchased[fields[amountFields-1].getSeatInM()-1][fields[amountFields-1].getSeatInN()-1] = 1;
}
bool Seating::checkSeatStatus()
{
    return   isSeatPurchased[fields[amountFields-1].getSeatInM()-1][fields[amountFields-1].getSeatInN()-1];
}
void Seating::finishTicket()
{
    fields[amountFields-1].fillField(amountOfColumns,amountOfRows); 
}