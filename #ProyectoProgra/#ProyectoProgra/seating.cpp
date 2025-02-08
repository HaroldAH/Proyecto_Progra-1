#include "Seating.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <limits>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

Seating::Seating() {
    segmentNumber = 0;
    numberOfRows = 0;
    numberOfColumns = 0;
    cost = 0.0f;
    seatPurchased = nullptr;
    initializeRoom();
}

void Seating::initializeRoom() {
    if (numberOfRows <= 0 || numberOfColumns <= 0)
        return;

    if (seatPurchased) {
        for (int i = 0; i < numberOfRows; i++) {
            delete[] seatPurchased[i];
        }
        delete[] seatPurchased;
        seatPurchased = nullptr;
    }

    seatPurchased = new bool* [numberOfRows];
    for (int i = 0; i < numberOfRows; i++) {
        seatPurchased[i] = new bool[numberOfColumns];
    }
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            seatPurchased[i][j] = false;
        }
    }
}

Seating::Seating(const Seating& other) {
    segmentNumber = other.segmentNumber;
    numberOfRows = other.numberOfRows;
    numberOfColumns = other.numberOfColumns;
    cost = other.cost;

    if (other.seatPurchased) {
        seatPurchased = new bool* [numberOfRows];
        for (int i = 0; i < numberOfRows; i++) {
            seatPurchased[i] = new bool[numberOfColumns];
            for (int j = 0; j < numberOfColumns; j++) {
                seatPurchased[i][j] = other.seatPurchased[i][j];
            }
        }
    }
    else {
        seatPurchased = nullptr;
    }
}

Seating& Seating::operator=(const Seating& other) {
    if (this != &other) {
        if (seatPurchased) {
            for (int i = 0; i < numberOfRows; i++) {
                delete[] seatPurchased[i];
            }
            delete[] seatPurchased;
        }

        segmentNumber = other.segmentNumber;
        numberOfRows = other.numberOfRows;
        numberOfColumns = other.numberOfColumns;
        cost = other.cost;

        if (other.seatPurchased) {
            seatPurchased = new bool* [numberOfRows];
            for (int i = 0; i < numberOfRows; i++) {
                seatPurchased[i] = new bool[numberOfColumns];
                for (int j = 0; j < numberOfColumns; j++) {
                    seatPurchased[i][j] = other.seatPurchased[i][j];
                }
            }
        }
        else {
            seatPurchased = nullptr;
        }
    }
    return *this;
}

/// Función adaptada a SFML para dibujar la sala
void Seating::displaySeats(sf::RenderWindow& window) {
    // Parámetros de dibujo
    const float seatWidth = 30.f;
    const float seatHeight = 30.f;
    const float spacing = 10.f;      // Espaciado entre asientos
    const float marginX = 50.f;      // Margen izquierdo
    const float marginY = 100.f;     // Margen superior (donde se dibujarán las filas)

    // Cargar fuente para etiquetas
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente en Seating::displaySeats." << std::endl;
        return;
    }

    // Dibujar título en la parte superior
    sf::Text title("Representacion de la sala", font, 28);
    title.setFillColor(sf::Color::Black);
    title.setPosition(marginX, marginY - 60.f);
    window.draw(title);

    // Dibujar etiquetas de columnas (letras)
    for (int j = 0; j < numberOfColumns; j++) {
        sf::Text colLabel;
        colLabel.setFont(font);
        colLabel.setCharacterSize(20);
        colLabel.setFillColor(sf::Color::Black);
        char colChar = 'A' + j;
        colLabel.setString(std::string(1, colChar));
        // Centrar el label en el asiento
        float posX = marginX + j * (seatWidth + spacing) + seatWidth / 2.f - colLabel.getLocalBounds().width / 2.f;
        float posY = marginY - 40.f;
        colLabel.setPosition(posX, posY);
        window.draw(colLabel);
    }

    // Dibujar la cuadrícula de asientos y etiquetas de filas
    for (int i = 0; i < numberOfRows; i++) {
        // Etiqueta de fila (número)
        sf::Text rowLabel;
        rowLabel.setFont(font);
        rowLabel.setCharacterSize(20);
        rowLabel.setFillColor(sf::Color::Black);
        rowLabel.setString(std::to_string(i + 1));
        float rowLabelX = marginX - 40.f;
        float rowLabelY = marginY + i * (seatHeight + spacing) + seatHeight / 2.f - rowLabel.getLocalBounds().height / 2.f;
        rowLabel.setPosition(rowLabelX, rowLabelY);
        window.draw(rowLabel);

        for (int j = 0; j < numberOfColumns; j++) {
            sf::RectangleShape seatRect(sf::Vector2f(seatWidth, seatHeight));
            float seatX = marginX + j * (seatWidth + spacing);
            float seatY = marginY + i * (seatHeight + spacing);
            seatRect.setPosition(seatX, seatY);

            // Asignar color: verde si está disponible, rojo si está vendido
            if (seatPurchased && seatPurchased[i][j])
                seatRect.setFillColor(sf::Color::Red);
            else
                seatRect.setFillColor(sf::Color::Green);

            seatRect.setOutlineColor(sf::Color::Black);
            seatRect.setOutlineThickness(1.f);

            window.draw(seatRect);
        }
    }
}

bool Seating::isRoomFull() {
    return (cost > 0 && numberOfRows > 0 && numberOfColumns > 0);
}

bool Seating::isRoomComplete() {
    return isRoomFull();
}

float Seating::getCost() const {
    return cost;
}

void Seating::setCost(float newCost) {
    cost = newCost;
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
    if (row < 0 || row >= numberOfRows ||
        column < 0 || column >= numberOfColumns) {
        cout << "\nError: Coordenadas fuera de rango.\n";
        return;
    }
    if (seatPurchased[row - 1][column - 1]) {
        cout << "\nError: El asiento ya esta ocupado.\n";
        return;
    }
    seatPurchased[row - 1][column - 1] = true;
    cout << "\nAsiento en fila " << (row + 1)
        << " y columna " << char('A' + column)
        << " vendido exitosamente.\n";
}

/// Función checkSales adaptada parcialmente (sigue mostrando texto por consola)
void Seating::checkSales(Event& event, Segment& segment,
    std::map<std::tuple<int, int>, Seating>& seatingMap, sf::RenderWindow& win)
{
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return;
    }

    cout << "\nEventos disponibles:\n";
    for (int i = 0; i < event.getEventCount(); i++) {
        cout << i + 1 << ". " << event.getEvents().getAt(i + 1).getName() << "\n";
    }

    int selectedEvent = 0;
    int size = event.getEventCount();
    selectedEvent = validateChoice(selectedEvent, size);

    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    if (segments.getHead() == nullptr) {
        cin.get();
        return;
    }

    List<int>& segmentCounts = segment.getSegmentCount();
    if (segmentCounts.getHead() == nullptr) {
        cout << "Error: No se pudo obtener el conteo de segmentos.\n";
        cout << "\nPresione Enter para continuar...";
        cin.get();
        return;
    }

    int numSegments = segmentCounts.getAt(selectedEvent);
    if (numSegments <= 0) {
        cout << "No hay segmentos disponibles para este evento.\n";
        cout << "\nPresione Enter para continuar...";
        cin.get();
        return;
    }

    cout << "\nSegmentos disponibles para el evento \""
        << event.getEvents().getAt(selectedEvent).getName() << "\":\n";
    for (int i = 0; i < numSegments; i++) {
        cout << i + 1 << ". "
            << segments.getAt(selectedEvent).getAt(i + 1).getName()
            << " - Precio: "
            << segments.getAt(selectedEvent).getAt(i + 1).getPrice() << "\n";
    }
    int selectedSegment, option = 0;
    cout << "\nSeleccione un segmento: ";
    selectedSegment = validateChoice(option, numSegments);

    auto seatingKey = make_tuple(selectedEvent, selectedSegment);
    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        cout << "\nNo se han vendido asientos para este segmento.\n\n";
        int rows = segments.getAt(selectedEvent).getAt(selectedSegment).getRows();
        int columns = segments.getAt(selectedEvent).getAt(selectedSegment).getSeats();
        float price = segments.getAt(selectedEvent).getAt(selectedSegment).getPrice();
        Seating tempSeating;
        tempSeating.setNumberOfRows(rows);
        tempSeating.setNumberOfColumns(columns);
        tempSeating.setCost(price);
        tempSeating.initializeRoom();
        cout << "\nVista de la sala vacia (sin ventas):\n";
        tempSeating.displaySeats(win);
    }
    else {
        Seating& seating = seatingMap[seatingKey];
        cout << "\nRepresentacion grafica del segmento \""
            << segments.getAt(selectedEvent).getAt(selectedSegment).getName()
            << "\":\n";
        seating.displaySeats(win);
    }

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int Seating::validateChoice(int& choice, int& size) {
    while (true) {
        cout << "Seleccione una opcion: " << endl;
        if (cin >> choice && choice >= 1 && choice <= size) {
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente de nuevo." << endl;
    }
}

Seating::~Seating() {
    if (seatPurchased) {
        for (int i = 0; i < numberOfRows; i++) {
            delete[] seatPurchased[i];
        }
        delete[] seatPurchased;
        seatPurchased = nullptr;
    }
}

bool Seating::freeSeat(int row, int col) {
    if (!seatPurchased) {
        return false;
    }
    if (row < 0 || row >= numberOfRows || col < 0 || col >= numberOfColumns) {
        return false;
    }
    if (seatPurchased[row][col]) {
        seatPurchased[row][col] = false;
        return true;
    }
    return false;
}
