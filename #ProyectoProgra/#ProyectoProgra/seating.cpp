#include "Seating.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <limits>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

static const sf::Color BG_COLOR_EV(200, 200, 200);
static const sf::Color HEADER_COLOR_EV(160, 160, 160);
static const sf::Color TEXT_COLOR_EV = sf::Color::Black;
static const sf::Color HIGHLIGHT_COLOR_EV = sf::Color::Red;

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
    const float marginY = 150.f;

    // Cargar fuente para etiquetas
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente en Seating::displaySeats." << std::endl;
        return;
    }

    // Dibujar título en la parte superior
    sf::Text title("Representacion de la sala", font, 20);
    title.setFillColor(sf::Color::Black);
    title.setPosition(marginX, marginY - 90.f);
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

// Función checkSales adaptada parcialmente (sigue mostrando texto por consola)
void Seating::checkSales(Event& event, Segment& segment,
    std::map<std::tuple<int, int>, Seating>& seatingMap, sf::RenderWindow& win)
{
    // Cargar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return;
    }

    // 1. Verificar que haya eventos disponibles y mostrarlos en pantalla.
    if (event.getEventCount() == 0) {
        sf::Text noEvents("No hay eventos disponibles.", font, 24);
        noEvents.setFillColor(TEXT_COLOR_EV);
        noEvents.setPosition(50.f, 50.f);
        win.clear(BG_COLOR_EV);
        win.draw(noEvents);
        win.display();
        sf::sleep(sf::seconds(2));
        return;
    }

    // Crear una lista gráfica de los eventos disponibles
    std::vector<sf::Text> eventOptions;
    for (int i = 1; i <= event.getEventCount(); i++) {
        std::string eventName = event.getEvents().getAt(i).getName();
        sf::Text opt(eventName, font, 24);
        opt.setFillColor(TEXT_COLOR_EV);
        opt.setPosition(50.f, 100.f + (i - 1) * 40.f);
        eventOptions.push_back(opt);
    }

    // Mostrar la lista y esperar que el usuario seleccione un evento haciendo clic.
    int selectedEvent = -1;
    bool eventSelected = false;
    while (win.isOpen() && !eventSelected) {
        sf::Event ev;
        while (win.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < static_cast<int>(eventOptions.size()); i++) {
                    if (eventOptions[i].getGlobalBounds().contains(mousePos)) {
                        selectedEvent = i + 1;
                        eventSelected = true;
                        break;
                    }
                }
            }
        }
        win.clear(BG_COLOR_EV);
        sf::Text title("Eventos disponibles:", font, 28);
        title.setFillColor(TEXT_COLOR_EV);
        title.setPosition(50.f, 50.f);
        win.draw(title);
        for (auto& opt : eventOptions)
            win.draw(opt);
        win.display();
    }

    // 2. Mostrar la lista de segmentos disponibles para el evento seleccionado.
    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();
    if (segmentCounts.getHead() == nullptr) {
        sf::Text error("Error: No se pudo obtener el conteo de segmentos.", font, 24);
        error.setFillColor(TEXT_COLOR_EV);
        error.setPosition(50.f, 50.f);
        win.clear(BG_COLOR_EV);
        win.draw(error);
        win.display();
        sf::sleep(sf::seconds(2));
        return;
    }
    int numSegments = segmentCounts.getAt(selectedEvent);
    if (numSegments <= 0) {
        sf::Text noSegments("No hay segmentos disponibles para este evento.", font, 24);
        noSegments.setFillColor(TEXT_COLOR_EV);
        noSegments.setPosition(50.f, 50.f);
        win.clear(BG_COLOR_EV);
        win.draw(noSegments);
        win.display();
        sf::sleep(sf::seconds(2));
        return;
    }

    std::vector<sf::Text> segmentOptions;
    for (int i = 1; i <= numSegments; i++) {
        std::ostringstream oss;
        oss << i << ". " << segments.getAt(selectedEvent).getAt(i).getName()
            << " - Precio: $" << segments.getAt(selectedEvent).getAt(i).getPrice();
        sf::Text opt(oss.str(), font, 24);
        opt.setFillColor(TEXT_COLOR_EV);
        opt.setPosition(50.f, 100.f + (i - 1) * 40.f);
        segmentOptions.push_back(opt);
    }

    int selectedSegment = -1;
    bool segmentSelected = false;
    while (win.isOpen() && !segmentSelected) {
        sf::Event ev;
        while (win.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < static_cast<int>(segmentOptions.size()); i++) {
                    if (segmentOptions[i].getGlobalBounds().contains(mousePos)) {
                        selectedSegment = i + 1;
                        segmentSelected = true;
                        break;
                    }
                }
            }
        }
        win.clear(BG_COLOR_EV);
        sf::Text segTitle("Segmentos para el evento \"" +
            event.getEvents().getAt(selectedEvent).getName() + "\":", font, 28);
        segTitle.setFillColor(TEXT_COLOR_EV);
        segTitle.setPosition(50.f, 50.f);
        win.draw(segTitle);
        for (auto& opt : segmentOptions)
            win.draw(opt);
        win.display();
    }

    // 3. Verificar si existen asientos vendidos para el segmento seleccionado.
    auto seatingKey = std::make_tuple(selectedEvent, selectedSegment);
    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        sf::Text noSales("No se han vendido asientos para este segmento.", font, 24);
        noSales.setFillColor(TEXT_COLOR_EV);
        noSales.setPosition(50.f, 50.f);
        win.clear(BG_COLOR_EV);
        win.draw(noSales);
        win.display();
        sf::sleep(sf::seconds(2));
        return;
    }
    else {
        // **Modificación solicitada: ajustar la posición del título para separar el mensaje y la visualización de asientos.**
        Seating& seating = seatingMap[seatingKey];
        sf::Text seatingTitle("Representacion grafica del segmento \"" +
            segments.getAt(selectedEvent).getAt(selectedSegment).getName() + "\":", font, 28);
        seatingTitle.setFillColor(TEXT_COLOR_EV);
        seatingTitle.setPosition(50.f, 10.f);  // Se cambia de y=50.f a y=10.f para dejar más espacio
        win.clear(BG_COLOR_EV);
        win.draw(seatingTitle);
        seating.displaySeats(win);
        sf::Text continueText("Presione ENTER para continuar...", font, 24);
        continueText.setFillColor(TEXT_COLOR_EV);
        continueText.setPosition(50.f, win.getSize().y - 50.f);
        win.draw(continueText);
        win.display();

        bool cont = false;
        while (win.isOpen() && !cont) {
            sf::Event ev;
            while (win.pollEvent(ev)) {
                if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
                    cont = true;
                    break;
                }
                if (ev.type == sf::Event::Closed) {
                    win.close();
                    return;
                }
            }
        }
    }
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
