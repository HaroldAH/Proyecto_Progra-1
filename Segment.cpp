#include "Segment.h"
#include "Event.h"
#include <iostream>
#include <limits>

using namespace std;

Segment::Segment()
{
    segmentName = "";
    rows = 0;
    seats = 0;
    price = 0;
    segmentCapacity = 2;
    segmentCount = NULL;
    numEvents = 0;
    segmentsByEvent = NULL;
}

Segment::~Segment()
{
    if (segmentsByEvent != NULL)
    {
        for (int i = 0; i < numEvents; i++)
        {
            delete[] segmentsByEvent[i];
        }
        delete[] segmentsByEvent;
    }
    if (segmentCount != NULL)
    {
        delete[] segmentCount;
    }
}

string Segment::getName() { return segmentName; }

void Segment::setName(const std::string &aSegmentName)
{
    if (aSegmentName.empty())
    {
        throw invalid_argument("El nombre del segmento no puede estar vacio.");
    }
    segmentName = aSegmentName;
}

int Segment::getRows() { return rows; }

void Segment::setRows(int &aRows) { rows = aRows; }

int Segment::getSeats() { return seats; }

void Segment::setSeats(int &aSeats) { seats = aSeats; }

float Segment::getPrice() { return price; }

void Segment::setPrice(float &aPrice) { price = aPrice; }

void Segment::initializeSegments(int &capacity)
{
    segmentsByEvent = new Segment *[capacity];
    for (int i = 0; i < capacity; i++)
    {
        segmentsByEvent[i] = NULL;
    }
}

void Segment::saveSegments(Event &event)
{
    // Si ya había memoria asignada a segmentsByEvent, limpiamos
    if (segmentsByEvent != NULL)
    {
        for (int i = 0; i < numEvents; i++)
        {
            delete[] segmentsByEvent[i];
        }
        delete[] segmentsByEvent;
        segmentsByEvent = NULL;
    }

    // Y limpiamos segmentCount también
    if (segmentCount != NULL)
    {
        delete[] segmentCount;
        segmentCount = NULL;
    }

    Event *events = event.getEvents();
    numEvents = event.getEventCount();

    if (numEvents == 0)
    {
        cout << "No hay eventos disponibles para asignar segmentos." << endl
             << endl;
        return;
    }
    segmentsByEvent = new Segment *[numEvents];
    segmentCount = new int[numEvents];

    for (int i = 0; i < numEvents; i++)
    {
        int numSegments = 0;
        cout << endl
             << "Agregar segmentos al evento " << i + 1 << " :" << endl;
        cout << endl
             << "Cuantos segmentos desea agregar?" << endl;
        numSegments = getValidIntInput();

        segmentsByEvent[i] = new Segment[numSegments];
        segmentCount[i] = numSegments;

        for (int j = 0; j < numSegments; j++)
        {
            string name;
            int rows, columns;

            cout << endl
                 << "Ingrese el nombre del segmento " << j + 1 << ":" << endl;
            cin.ignore();
            getline(cin, name);

            cout << "Ingrese el numero de filas:" << endl;
            rows = getValidIntInput();

            cout << "Ingrese la cantidad de asientos por fila:" << endl;
            columns = getValidIntInput();

            cout << "Ingrese el precio del segmento:" << endl;
            price = getValidIntInput();

            segmentsByEvent[i][j].setName(name);
            segmentsByEvent[i][j].setRows(rows);
            segmentsByEvent[i][j].setSeats(columns);
            segmentsByEvent[i][j].setPrice(price);

            cout << "Segmento guardado con exito." << endl
                 << endl;
        }

        cout << "Se han guardado " << numSegments << " segmentos correctamente para el evento " << i + 1 << "." << endl
             << endl;
    }
}

Segment **Segment::getSegmentsByEvent()
{
    return segmentsByEvent;
}

void Segment::getSegmentCount(Event &event, int *destino)
{

    for (int i = 0; i < event.getEventCount(); i++)
    {
        destino[i] = segmentCount[i];
    }
}

int Segment::getValidIntInput()
{
    int input;
    while (!(cin >> input))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
    return input;
}