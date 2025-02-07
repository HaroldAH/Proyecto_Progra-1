#include "Segment.h"
#include <iostream>
#include <limits>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cctype>    // Para isdigit

using namespace std;

// Definiciones de colores (ajusta según tus necesidades)
static const sf::Color BG_COLOR_EV(200, 200, 200);
static const sf::Color HEADER_COLOR_EV(160, 160, 160);
static const sf::Color TEXT_COLOR_EV = sf::Color::Black;
static const sf::Color HIGHLIGHT_COLOR_EV = sf::Color::Red;

/////////////////////////////////////////
// Función auxiliar: Solicitar una cadena usando SFML
/////////////////////////////////////////
std::string getValidStringInputSFML(sf::RenderWindow& win, const std::string& prompt) {
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "Error al cargar la fuente." << endl;
        return "";
    }
    // Configurar el texto de la consigna
    sf::Text promptText(prompt, font, 24);
    promptText.setFillColor(TEXT_COLOR_EV);
    promptText.setPosition(50, 50);

    // Caja de entrada
    sf::RectangleShape inputBox(sf::Vector2f(400.f, 40.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50, 120);

    // Texto que muestra la entrada
    sf::Text inputText("", font, 24);
    inputText.setFillColor(TEXT_COLOR_EV);
    inputText.setPosition(55, 125);

    std::string inputStr;
    bool done = false;
    while (!done && win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
                return "";
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (!inputStr.empty()) {
                        done = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::Backspace) {
                    if (!inputStr.empty()) {
                        inputStr.pop_back();
                        inputText.setString(inputStr);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (isprint(c)) {
                        inputStr.push_back(c);
                        inputText.setString(inputStr);
                    }
                }
            }
        }
        win.clear(BG_COLOR_EV);
        win.draw(promptText);
        win.draw(inputBox);
        win.draw(inputText);
        win.display();
    }
    win.clear(sf::Color::White);
    win.display();
    return inputStr;
}

/////////////////////////////////////////
// Función auxiliar: Solicitar un entero usando SFML
/////////////////////////////////////////
int getValidIntInputSFML(sf::RenderWindow& win, const std::string& prompt) {
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "Error al cargar la fuente." << endl;
        return 0;
    }
    sf::Text promptText(prompt, font, 24);
    promptText.setFillColor(TEXT_COLOR_EV);
    promptText.setPosition(50, 50);

    sf::RectangleShape inputBox(sf::Vector2f(200.f, 40.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50, 120);

    sf::Text inputText("", font, 24);
    inputText.setFillColor(TEXT_COLOR_EV);
    inputText.setPosition(55, 125);

    std::string inputStr;
    bool done = false;
    while (!done && win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
                return 0;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (!inputStr.empty()) {
                        done = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::Backspace) {
                    if (!inputStr.empty()) {
                        inputStr.pop_back();
                        inputText.setString(inputStr);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (isdigit(c)) {
                        inputStr.push_back(c);
                        inputText.setString(inputStr);
                    }
                }
            }
        }
        win.clear(BG_COLOR_EV);
        win.draw(promptText);
        win.draw(inputBox);
        win.draw(inputText);
        win.display();
    }
    win.clear(sf::Color::White);
    win.display();
    int value = 0;
    try {
        value = stoi(inputStr);
    }
    catch (...) {
        value = 0;
    }
    return value;
}

/////////////////////////////////////////
// Función auxiliar: Solicitar un float usando SFML
/////////////////////////////////////////
float getValidFloatInputSFML(sf::RenderWindow& win, const std::string& prompt) {
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return 0.0f;
    }
    sf::Text promptText(prompt, font, 24);
    promptText.setFillColor(TEXT_COLOR_EV);
    promptText.setPosition(50, 50);

    sf::RectangleShape inputBox(sf::Vector2f(200.f, 40.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50, 120);

    sf::Text inputText("", font, 24);
    inputText.setFillColor(TEXT_COLOR_EV);
    inputText.setPosition(55, 125);

    std::string inputStr;
    bool done = false;
    while (!done && win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
                return 0.0f;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (!inputStr.empty())
                        done = true;
                }
                else if (event.key.code == sf::Keyboard::Backspace) {
                    if (!inputStr.empty()) {
                        inputStr.pop_back();
                        inputText.setString(inputStr);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (isdigit(c) || c == '.') {
                        inputStr.push_back(c);
                        inputText.setString(inputStr);
                    }
                }
            }
        }
        win.clear(BG_COLOR_EV);
        win.draw(promptText);
        win.draw(inputBox);
        win.draw(inputText);
        win.display();
    }
    win.clear(sf::Color::White);
    win.display();
    float value = 0.0f;
    try {
        value = std::stof(inputStr);
    }
    catch (...) {
        value = 0.0f;
    }
    return value;
}

/////////////////////////////////////////
// Métodos de la clase Segment
/////////////////////////////////////////

Segment::Segment() {
    segmentName = "";
    rows = 0;
    seats = 0;
    price = 0;
    segmentCapacity = 2;
}

Segment::~Segment() {
    // El destructor de List se encargará de liberar la memoria.
}
std::string Segment::getName() {
    return segmentName;
}

int Segment::getRows() {
    return rows;
}

int Segment::getSeats() {
    return seats;
}

float Segment::getPrice() {
    return price;
}

List<int>& Segment::getSegmentCount() {
    return segmentCount;
}

List<List<Segment>>& Segment::getSegmentsByEvent() {
    return segmentsByEvent;
}

// Setters
void Segment::setName(const std::string& aSegmentName) {
    segmentName = aSegmentName;
}

void Segment::setRows(int aRows) {
    rows = aRows;
}

void Segment::setSeats(int aSeats) {
    seats = aSeats;
}

void Segment::setPrice(float aPrice) {
    price = aPrice;
}

/////////////////////////////////////////
// Gestionar (crear/ampliar) las listas de segmentos según el número de eventos
/////////////////////////////////////////
void Segment::manageSegments(Segment& segment, int& numEvents) {
    if (numEvents <= 0) {
        cout << "No hay eventos disponibles." << endl;
        return;
    }
    if (segmentsByEvent.getHead() == nullptr) {
        segmentCapacity = numEvents;
        for (int i = 0; i < segmentCapacity; i++) {
            segmentsByEvent.insertAtEnd(List<Segment>());
            segmentCount.insertAtEnd(0);
        }
        return;
    }
    int currentCapacity = segmentCapacity;
    if (numEvents <= currentCapacity)
        return;
    for (int i = currentCapacity; i < numEvents; i++) {
        segmentsByEvent.insertAtEnd(List<Segment>());
        segmentCount.insertAtEnd(0);
    }
    segmentCapacity = numEvents;
}

/////////////////////////////////////////
// Función miembro que solicita datos para un segmento mediante SFML
/////////////////////////////////////////
void Segment::addSegmentData(Segment& segment, int index, sf::RenderWindow& win) {
    // Cargar la fuente (ajusta la ruta si es necesario)
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente." << std::endl;
        return;
    }

    // Fondo del formulario (una ventana dentro de la ventana principal)
    sf::RectangleShape formRect(sf::Vector2f(600.f, 400.f));
    formRect.setFillColor(sf::Color::White);
    formRect.setOutlineColor(sf::Color::Black);
    formRect.setOutlineThickness(2.f);
    formRect.setPosition(100.f, 100.f);

    // Título del formulario
    sf::Text titleText("Nuevo Segmento para Evento #" + std::to_string(index), font, 24);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(150.f, 110.f);

    // Etiquetas para cada campo
    sf::Text labelName("Nombre del Segmento:", font, 18);
    labelName.setFillColor(sf::Color::Black);
    labelName.setPosition(120.f, 160.f);

    sf::Text labelRows("Numero de Filas:", font, 18);
    labelRows.setFillColor(sf::Color::Black);
    labelRows.setPosition(120.f, 210.f);

    sf::Text labelSeats("Cantidad de Asientos:", font, 18);
    labelSeats.setFillColor(sf::Color::Black);
    labelSeats.setPosition(120.f, 260.f);

    sf::Text labelPrice("Precio del Segmento:", font, 18);
    labelPrice.setFillColor(sf::Color::Black);
    labelPrice.setPosition(120.f, 310.f);

    // Cajas de ingreso para cada campo
    sf::RectangleShape boxName(sf::Vector2f(200.f, 30.f));
    boxName.setFillColor(sf::Color::White);
    boxName.setOutlineColor(sf::Color::Black);
    boxName.setOutlineThickness(1.f);
    boxName.setPosition(320.f, 155.f);

    sf::RectangleShape boxRows(sf::Vector2f(100.f, 30.f));
    boxRows.setFillColor(sf::Color::White);
    boxRows.setOutlineColor(sf::Color::Black);
    boxRows.setOutlineThickness(1.f);
    boxRows.setPosition(320.f, 205.f);

    sf::RectangleShape boxSeats(sf::Vector2f(100.f, 30.f));
    boxSeats.setFillColor(sf::Color::White);
    boxSeats.setOutlineColor(sf::Color::Black);
    boxSeats.setOutlineThickness(1.f);
    boxSeats.setPosition(320.f, 255.f);

    sf::RectangleShape boxPrice(sf::Vector2f(100.f, 30.f));
    boxPrice.setFillColor(sf::Color::White);
    boxPrice.setOutlineColor(sf::Color::Black);
    boxPrice.setOutlineThickness(1.f);
    boxPrice.setPosition(320.f, 305.f);

    // Objetos de texto para mostrar la entrada del usuario en cada campo
    sf::Text inputName("", font, 18);
    inputName.setFillColor(sf::Color::Black);
    inputName.setPosition(boxName.getPosition().x + 5, boxName.getPosition().y + 5);

    sf::Text inputRows("", font, 18);
    inputRows.setFillColor(sf::Color::Black);
    inputRows.setPosition(boxRows.getPosition().x + 5, boxRows.getPosition().y + 5);

    sf::Text inputSeats("", font, 18);
    inputSeats.setFillColor(sf::Color::Black);
    inputSeats.setPosition(boxSeats.getPosition().x + 5, boxSeats.getPosition().y + 5);

    sf::Text inputPrice("", font, 18);
    inputPrice.setFillColor(sf::Color::Black);
    inputPrice.setPosition(boxPrice.getPosition().x + 5, boxPrice.getPosition().y + 5);

    // Botón "Guardar"
    sf::Text saveButton("Guardar", font, 18);
    saveButton.setFillColor(sf::Color::Black);
    saveButton.setPosition(300.f, 360.f);

    // Variables para almacenar la entrada del usuario en cada campo
    std::string nameStr = "";
    std::string rowsStr = "";
    std::string seatsStr = "";
    std::string priceStr = "";

    // Variable que indica qué campo está activo (0: nombre, 1: filas, 2: asientos, 3: precio)
    int activeField = 0;

    // Borde rojo para resaltar el campo activo
    sf::RectangleShape activeBorder;
    activeBorder.setFillColor(sf::Color::Transparent);
    activeBorder.setOutlineColor(sf::Color::Red);
    activeBorder.setOutlineThickness(2.f);
    // Inicialmente, el campo activo es el de nombre
    activeBorder.setPosition(boxName.getPosition());
    activeBorder.setSize(boxName.getSize());

    bool editing = true;
    while (editing && win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
                return;
            }
            // Si se hace clic con el mouse, se determina qué caja se selecciona
            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(win).x),
                    static_cast<float>(sf::Mouse::getPosition(win).y));
                if (boxName.getGlobalBounds().contains(mousePos))
                    activeField = 0;
                else if (boxRows.getGlobalBounds().contains(mousePos))
                    activeField = 1;
                else if (boxSeats.getGlobalBounds().contains(mousePos))
                    activeField = 2;
                else if (boxPrice.getGlobalBounds().contains(mousePos))
                    activeField = 3;
                // Si se hace clic en el botón "Guardar"
                if (saveButton.getGlobalBounds().contains(mousePos)) {
                    try {
                        int numRows = std::stoi(rowsStr);
                        int numSeats = std::stoi(seatsStr);
                        float segPrice = std::stof(priceStr);
                        segment.setName(nameStr);
                        segment.setRows(numRows);
                        segment.setSeats(numSeats);
                        segment.setPrice(segPrice);
                        editing = false;
                    }
                    catch (const std::exception& e) {
                        std::cout << "Error: Datos no válidos." << std::endl;
                    }
                }
            }
            // Manejo de teclado
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    activeField = (activeField + 1) % 4;
                }
                else if (event.key.code == sf::Keyboard::Backspace) {
                    if (activeField == 0 && !nameStr.empty()) {
                        nameStr.pop_back();
                        inputName.setString(nameStr);
                    }
                    else if (activeField == 1 && !rowsStr.empty()) {
                        rowsStr.pop_back();
                        inputRows.setString(rowsStr);
                    }
                    else if (activeField == 2 && !seatsStr.empty()) {
                        seatsStr.pop_back();
                        inputSeats.setString(seatsStr);
                    }
                    else if (activeField == 3 && !priceStr.empty()) {
                        priceStr.pop_back();
                        inputPrice.setString(priceStr);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (activeField == 3) {
                        try {
                            int numRows = std::stoi(rowsStr);
                            int numSeats = std::stoi(seatsStr);
                            float segPrice = std::stof(priceStr);
                            segment.setName(nameStr);
                            segment.setRows(numRows);
                            segment.setSeats(numSeats);
                            segment.setPrice(segPrice);
                            editing = false;
                        }
                        catch (...) {
                            std::cout << "Error: Datos no válidos." << std::endl;
                        }
                    }
                    else {
                        activeField++;
                    }
                }
            }
            // Captura de entrada de texto según el campo activo
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (activeField == 0) {
                        if (isprint(c)) {
                            nameStr.push_back(c);
                            inputName.setString(nameStr);
                        }
                    }
                    else if (activeField == 1) {
                        if (isdigit(c)) {
                            rowsStr.push_back(c);
                            inputRows.setString(rowsStr);
                        }
                    }
                    else if (activeField == 2) {
                        if (isdigit(c)) {
                            seatsStr.push_back(c);
                            inputSeats.setString(seatsStr);
                        }
                    }
                    else if (activeField == 3) {
                        if (isdigit(c) || c == '.') {
                            priceStr.push_back(c);
                            inputPrice.setString(priceStr);
                        }
                    }
                }
            }
        }

        // Actualizar el borde resaltado según el campo activo
        if (activeField == 0) {
            activeBorder.setPosition(boxName.getPosition());
            activeBorder.setSize(boxName.getSize());
        }
        else if (activeField == 1) {
            activeBorder.setPosition(boxRows.getPosition());
            activeBorder.setSize(boxRows.getSize());
        }
        else if (activeField == 2) {
            activeBorder.setPosition(boxSeats.getPosition());
            activeBorder.setSize(boxSeats.getSize());
        }
        else if (activeField == 3) {
            activeBorder.setPosition(boxPrice.getPosition());
            activeBorder.setSize(boxPrice.getSize());
        }

        // Renderizar el formulario con fondo gris claro
        win.clear(BG_COLOR_EV);
        win.draw(formRect);
        win.draw(titleText);

        win.draw(labelName);
        win.draw(boxName);
        win.draw(inputName);

        win.draw(labelRows);
        win.draw(boxRows);
        win.draw(inputRows);

        win.draw(labelSeats);
        win.draw(boxSeats);
        win.draw(inputSeats);

        win.draw(labelPrice);
        win.draw(boxPrice);
        win.draw(inputPrice);

        win.draw(saveButton);
        win.draw(activeBorder);
        win.display();
    }
}


/////////////////////////////////////////
// Función que solicita la cantidad de segmentos para cada evento y guarda los datos ingresados
/////////////////////////////////////////
void Segment::saveSegments(sf::RenderWindow& win, Segment& segment, int events, int eventIndex) {
    if (events == 0) {
        cout << "No hay eventos disponibles para asignar segmentos." << endl;
        return;
    }
    // Asegurarse de que las listas tienen capacidad para el número de eventos
    manageSegments(segment, events);
    if (eventIndex != 0) {
        auto eventNode = segmentsByEvent.getHead();
        auto countNode = segmentCount.getHead();
        for (int i = 1; i < eventIndex; i++) {
            eventNode = eventNode->next;
            countNode = countNode->next;
        }
        if (countNode->data <= 0) {
            int numSegments = getValidIntInputSFML(win, "Cuantos segmentos desea agregar al evento " + to_string(eventIndex) + "?");
            if (numSegments <= 0)
                return;
            countNode->data = numSegments;
            win.clear(sf::Color::White);
            win.display();
            for (int j = 1; j <= numSegments; j++) {
                win.clear(sf::Color::White);
                win.display();
                Segment newSegment;
                addSegmentData(newSegment, j, win);
                eventNode->data.insertAtEnd(newSegment);
            }
        }
    }
    else {
        auto eventNode = segmentsByEvent.getHead();
        auto countNode = segmentCount.getHead();
        for (int i = 1; i <= events; i++) {
            if (countNode->data <= 0) {
                int numSegments = getValidIntInputSFML(win, "Cuantos segmentos desea agregar al evento " + to_string(i) + "?");
                if (numSegments <= 0)
                    continue;
                countNode->data = numSegments;
                win.clear(sf::Color::White);
                win.display();
                for (int j = 1; j <= numSegments; j++) {
                    win.clear(sf::Color::White);
                    win.display();
                    Segment newSegment;
                    addSegmentData(newSegment, j, win);
                    eventNode->data.insertAtEnd(newSegment);
                }
            }
            eventNode = eventNode->next;
            countNode = countNode->next;
        }
    }
}

/////////////////////////////////////////
// Métodos de validación de entrada vía consola (para otros usos)
/////////////////////////////////////////
int Segment::getValidIntInput() {
    int input;
    while (true) {
        if (cin >> input && input > 0) {
            return input;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
}

float Segment::getValidFloatInput() {
    float input;
    while (true) {
        if (cin >> input && input > 0) {
            return input;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Por favor, ingrese un numero valido." << endl;
    }
}

string Segment::getValidStringInput(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin >> ws, input);
        if (!input.empty() && input.find_first_not_of(' ') != string::npos) {
            return input;
        }
        cout << "La entrada no puede estar vacia.\n";
    }
}