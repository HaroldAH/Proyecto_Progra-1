#include "Segment.h"
#include <iostream>
#include <limits>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cctype>    // Para isdigit

using namespace std;

// Definiciones de colores (ajusta según tus necesidades)
static const sf::Color BG_COLOR_EV(50, 50, 50);
static const sf::Color HEADER_COLOR_EV(70, 70, 70);
static const sf::Color TEXT_COLOR_EV = sf::Color::White;
static const sf::Color HIGHLIGHT_COLOR_EV = sf::Color(255, 140, 0);



// Función auxiliar: Solicitar una cadena usando SFML

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


// Función auxiliar: Solicitar un entero 

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


// Función auxiliar: Solicitar un float 

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

// Métodos de la clase Segment


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

// Gestionar (crear/ampliar) las listas de segmentos según el número de eventos

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


void Segment::addSegmentData(Segment& segment, int index, sf::RenderWindow& win)
{
    // 1. Encabezado (barra superior de 100 px)
    const float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(win.getSize().x, headerHeight));
    header.setFillColor(HEADER_COLOR_EV);
    header.setPosition(0.f, 0.f);

    // 2. Título dentro del encabezado
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente." << std::endl;
        return;
    }
    sf::Text headerTitle("Agregar Segmento al Evento #" + std::to_string(index), font, 25);
    headerTitle.setFillColor(TEXT_COLOR_EV);
    {
        sf::FloatRect bounds = headerTitle.getLocalBounds();
        float posX = 20.f;
        float posY = (headerHeight - bounds.height) / 2.f - bounds.top;
        headerTitle.setPosition(posX, posY);
    }

    // 3. Área de formulario debajo del header
    float formStartX = 50.f;
    float formStartY = headerHeight + 50.f;

    // Etiquetas
    sf::Text labelName("Nombre del Segmento:", font, 18);
    labelName.setFillColor(sf::Color::White);
    labelName.setPosition(formStartX, formStartY);

    sf::Text labelRows("Numero de Filas:", font, 18);
    labelRows.setFillColor(sf::Color::White);
    labelRows.setPosition(formStartX, formStartY + 60.f);

    sf::Text labelSeats("Cantidad de Asientos:", font, 18);
    labelSeats.setFillColor(sf::Color::White);
    labelSeats.setPosition(formStartX, formStartY + 120.f);

    sf::Text labelPrice("Precio del Segmento:", font, 18);
    labelPrice.setFillColor(sf::Color::White);
    labelPrice.setPosition(formStartX, formStartY + 180.f);

    // Cajas de texto
    sf::RectangleShape boxName(sf::Vector2f(300.f, 30.f));
    boxName.setFillColor(sf::Color::White);
    boxName.setOutlineColor(sf::Color::Black);
    boxName.setOutlineThickness(1.f);
    boxName.setPosition(formStartX + 220.f, formStartY - 5.f);

    sf::RectangleShape boxRows(sf::Vector2f(100.f, 30.f));
    boxRows.setFillColor(sf::Color::White);
    boxRows.setOutlineColor(sf::Color::Black);
    boxRows.setOutlineThickness(1.f);
    boxRows.setPosition(formStartX + 220.f, formStartY + 55.f);

    sf::RectangleShape boxSeats(sf::Vector2f(100.f, 30.f));
    boxSeats.setFillColor(sf::Color::White);
    boxSeats.setOutlineColor(sf::Color::Black);
    boxSeats.setOutlineThickness(1.f);
    boxSeats.setPosition(formStartX + 220.f, formStartY + 115.f);

    sf::RectangleShape boxPrice(sf::Vector2f(100.f, 30.f));
    boxPrice.setFillColor(sf::Color::White);
    boxPrice.setOutlineColor(sf::Color::Black);
    boxPrice.setOutlineThickness(1.f);
    boxPrice.setPosition(formStartX + 220.f, formStartY + 175.f);

    // Textos que muestran lo que va digitando el usuario
    sf::Text inputName("", font, 18);
    inputName.setFillColor(sf::Color::Black);
    inputName.setPosition(boxName.getPosition().x + 5, boxName.getPosition().y + 2);

    sf::Text inputRows("", font, 18);
    inputRows.setFillColor(sf::Color::Black);
    inputRows.setPosition(boxRows.getPosition().x + 5, boxRows.getPosition().y + 2);

    sf::Text inputSeats("", font, 18);
    inputSeats.setFillColor(sf::Color::Black);
    inputSeats.setPosition(boxSeats.getPosition().x + 5, boxSeats.getPosition().y + 2);

    sf::Text inputPrice("", font, 18);
    inputPrice.setFillColor(sf::Color::Black);
    inputPrice.setPosition(boxPrice.getPosition().x + 5, boxPrice.getPosition().y + 2);

    // Botón "Guardar"
    sf::Text saveButton("Guardar", font, 20);
    saveButton.setFillColor(sf::Color::Black);
    {
        sf::FloatRect sbBounds = saveButton.getLocalBounds();
        float btnX = formStartX + 220.f;
        float btnY = formStartY + 240.f;
        saveButton.setPosition(btnX, btnY);
    }

    // Variables para el texto ingresado
    std::string nameStr, rowsStr, seatsStr, priceStr;

    // Campo activo: 0 = nombre, 1 = filas, 2 = asientos, 3 = precio
    int activeField = 0;

    // Borde que resalta la caja activa
    sf::RectangleShape activeBorder;
    activeBorder.setFillColor(sf::Color::Transparent);
    activeBorder.setOutlineColor(HIGHLIGHT_COLOR_EV);
    activeBorder.setOutlineThickness(2.f);
    activeBorder.setPosition(boxName.getPosition());
    activeBorder.setSize(boxName.getSize());

    bool editing = true;
    while (editing && win.isOpen())
    {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                win.close();
                return;
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                // Detección de clic en cajas de texto
                if (boxName.getGlobalBounds().contains(mousePos))       activeField = 0;
                else if (boxRows.getGlobalBounds().contains(mousePos))  activeField = 1;
                else if (boxSeats.getGlobalBounds().contains(mousePos)) activeField = 2;
                else if (boxPrice.getGlobalBounds().contains(mousePos)) activeField = 3;

                // Detección de clic en "Guardar"
                sf::FloatRect saveRect = saveButton.getGlobalBounds();
                if (saveRect.contains(mousePos))
                {
                    try {
                        int numRows = std::stoi(rowsStr);
                        int numSeats = std::stoi(seatsStr);
                        float segPrice = std::stof(priceStr);

                        // Validar que el número de filas no supere 10 y asientos no supere 26
                        if (numRows > 10) {
                            sf::Text errorMsg("Error: El numero maximo de filas es 10.", font, 24);
                            errorMsg.setFillColor(sf::Color::Red);
                            // Posicionar el mensaje (por ejemplo, debajo del formulario)
                            errorMsg.setPosition(formStartX, formStartY + 300.f);
                            win.clear(BG_COLOR_EV);
                            win.draw(header);
                            win.draw(headerTitle);
                            win.draw(labelName);    win.draw(boxName);   win.draw(inputName);
                            win.draw(labelRows);    win.draw(boxRows);   win.draw(inputRows);
                            win.draw(labelSeats);   win.draw(boxSeats);  win.draw(inputSeats);
                            win.draw(labelPrice);   win.draw(boxPrice);  win.draw(inputPrice);
                            win.draw(saveButton);
                            win.draw(activeBorder);
                            win.draw(errorMsg);
                            win.display();
                            sf::sleep(sf::seconds(2));
                        }
                        else if (numSeats > 26) {
                            sf::Text errorMsg("Error: El numero maximo de asientos es 26.", font, 24);
                            errorMsg.setFillColor(sf::Color::Red);
                            errorMsg.setPosition(formStartX, formStartY + 300.f);
                            win.clear(BG_COLOR_EV);
                            win.draw(header);
                            win.draw(headerTitle);
                            win.draw(labelName);    win.draw(boxName);   win.draw(inputName);
                            win.draw(labelRows);    win.draw(boxRows);   win.draw(inputRows);
                            win.draw(labelSeats);   win.draw(boxSeats);  win.draw(inputSeats);
                            win.draw(labelPrice);   win.draw(boxPrice);  win.draw(inputPrice);
                            win.draw(saveButton);
                            win.draw(activeBorder);
                            win.draw(errorMsg);
                            win.display();
                            sf::sleep(sf::seconds(2));
                        }
                        else {
                            segment.setName(nameStr);
                            segment.setRows(numRows);
                            segment.setSeats(numSeats);
                            segment.setPrice(segPrice);

                            editing = false; // Cerramos el formulario
                        }
                    }
                    catch (...) {
                        sf::Text errorMsg("Error: datos no validos.", font, 24);
                        errorMsg.setFillColor(sf::Color::Red);
                        errorMsg.setPosition(formStartX, formStartY + 300.f);
                        win.clear(BG_COLOR_EV);
                        win.draw(header);
                        win.draw(headerTitle);
                        win.draw(labelName);    win.draw(boxName);   win.draw(inputName);
                        win.draw(labelRows);    win.draw(boxRows);   win.draw(inputRows);
                        win.draw(labelSeats);   win.draw(boxSeats);  win.draw(inputSeats);
                        win.draw(labelPrice);   win.draw(boxPrice);  win.draw(inputPrice);
                        win.draw(saveButton);
                        win.draw(activeBorder);
                        win.draw(errorMsg);
                        win.display();
                        sf::sleep(sf::seconds(2));
                    }
                }
            }
            // Navegación con teclado
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Tab) {
                    activeField = (activeField + 1) % 4;
                }
                else if (event.key.code == sf::Keyboard::Backspace) {
                    if (activeField == 0 && !nameStr.empty()) { nameStr.pop_back();  inputName.setString(nameStr); }
                    else if (activeField == 1 && !rowsStr.empty()) { rowsStr.pop_back();  inputRows.setString(rowsStr); }
                    else if (activeField == 2 && !seatsStr.empty()) { seatsStr.pop_back(); inputSeats.setString(seatsStr); }
                    else if (activeField == 3 && !priceStr.empty()) { priceStr.pop_back(); inputPrice.setString(priceStr); }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    // Si estamos en el campo de precio (último), intentamos guardar
                    if (activeField == 3)
                    {
                        try {
                            int numRows = std::stoi(rowsStr);
                            int numSeats = std::stoi(seatsStr);
                            float segPrice = std::stof(priceStr);

                            // Validar que el número de filas no supere 10 y asientos no supere 26
                            if (numRows > 10) {
                                sf::Text errorMsg("Error: El numero maximo de filas es 10.", font, 24);
                                errorMsg.setFillColor(sf::Color::Red);
                                errorMsg.setPosition(formStartX, formStartY + 300.f);
                                win.clear(BG_COLOR_EV);
                                win.draw(header);
                                win.draw(headerTitle);
                                win.draw(labelName);    win.draw(boxName);   win.draw(inputName);
                                win.draw(labelRows);    win.draw(boxRows);   win.draw(inputRows);
                                win.draw(labelSeats);   win.draw(boxSeats);  win.draw(inputSeats);
                                win.draw(labelPrice);   win.draw(boxPrice);  win.draw(inputPrice);
                                win.draw(saveButton);
                                win.draw(activeBorder);
                                win.draw(errorMsg);
                                win.display();
                                sf::sleep(sf::seconds(2));
                            }
                            else if (numSeats > 26) {
                                sf::Text errorMsg("Error: El numero maximo de asientos es 26.", font, 24);
                                errorMsg.setFillColor(sf::Color::Red);
                                errorMsg.setPosition(formStartX, formStartY + 300.f);
                                win.clear(BG_COLOR_EV);
                                win.draw(header);
                                win.draw(headerTitle);
                                win.draw(labelName);    win.draw(boxName);   win.draw(inputName);
                                win.draw(labelRows);    win.draw(boxRows);   win.draw(inputRows);
                                win.draw(labelSeats);   win.draw(boxSeats);  win.draw(inputSeats);
                                win.draw(labelPrice);   win.draw(boxPrice);  win.draw(inputPrice);
                                win.draw(saveButton);
                                win.draw(activeBorder);
                                win.draw(errorMsg);
                                win.display();
                                sf::sleep(sf::seconds(2));
                            }
                            else {
                                segment.setName(nameStr);
                                segment.setRows(numRows);
                                segment.setSeats(numSeats);
                                segment.setPrice(segPrice);

                                editing = false;
                            }
                        }
                        catch (...) {
                            sf::Text errorMsg("Error: datos no validos.", font, 24);
                            errorMsg.setFillColor(sf::Color::Red);
                            errorMsg.setPosition(formStartX, formStartY + 300.f);
                            win.clear(BG_COLOR_EV);
                            win.draw(header);
                            win.draw(headerTitle);
                            win.draw(labelName);    win.draw(boxName);   win.draw(inputName);
                            win.draw(labelRows);    win.draw(boxRows);   win.draw(inputRows);
                            win.draw(labelSeats);   win.draw(boxSeats);  win.draw(inputSeats);
                            win.draw(labelPrice);   win.draw(boxPrice);  win.draw(inputPrice);
                            win.draw(saveButton);
                            win.draw(activeBorder);
                            win.draw(errorMsg);
                            win.display();
                            sf::sleep(sf::seconds(2));
                        }
                    }
                    else {
                        // Avanzamos de campo
                        activeField++;
                    }
                }
            }
            // Captura de texto
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    char c = static_cast<char>(event.text.unicode);
                    if (activeField == 0) {
                        // Nombre (cualquier caracter imprimible)
                        if (isprint(c)) {
                            nameStr.push_back(c);
                            inputName.setString(nameStr);
                        }
                    }
                    else if (activeField == 1) {
                        // Filas (sólo dígitos)
                        if (isdigit(c)) {
                            rowsStr.push_back(c);
                            inputRows.setString(rowsStr);
                        }
                    }
                    else if (activeField == 2) {
                        // Asientos (sólo dígitos)
                        if (isdigit(c)) {
                            seatsStr.push_back(c);
                            inputSeats.setString(seatsStr);
                        }
                    }
                    else if (activeField == 3) {
                        // Precio (dígitos y '.')
                        if (isdigit(c) || c == '.') {
                            priceStr.push_back(c);
                            inputPrice.setString(priceStr);
                        }
                    }
                }
            }
        } // fin while pollEvent

        // Actualizar el borde activo
        if (activeField == 0) { activeBorder.setPosition(boxName.getPosition());  activeBorder.setSize(boxName.getSize()); }
        else if (activeField == 1) { activeBorder.setPosition(boxRows.getPosition());  activeBorder.setSize(boxRows.getSize()); }
        else if (activeField == 2) { activeBorder.setPosition(boxSeats.getPosition()); activeBorder.setSize(boxSeats.getSize()); }
        else if (activeField == 3) { activeBorder.setPosition(boxPrice.getPosition()); activeBorder.setSize(boxPrice.getSize()); }

        // Render
        win.clear(BG_COLOR_EV);

        // Dibujar el header
        win.draw(header);
        win.draw(headerTitle);

        // Dibujar etiquetas y cajas
        win.draw(labelName);    win.draw(boxName);   win.draw(inputName);
        win.draw(labelRows);    win.draw(boxRows);   win.draw(inputRows);
        win.draw(labelSeats);   win.draw(boxSeats);  win.draw(inputSeats);
        win.draw(labelPrice);   win.draw(boxPrice);  win.draw(inputPrice);

        // Botón "Guardar"
        win.draw(saveButton);

        // Borde de caja activa
        win.draw(activeBorder);

        win.display();
    }
}


/////////////////////////////////////////
// Función que solicita la cantidad de segmentos para cada evento y guarda los datos ingresados
/////////////////////////////////////////
void Segment::saveSegments(sf::RenderWindow& win, Segment& segment, int events, int eventIndex)
{
    if (events == 0) {
        std::cout << "No hay eventos disponibles para asignar segmentos." << std::endl;
        return;
    }

    // Asegurarnos de que la estructura interna tenga espacio para 'events' eventos
    manageSegments(segment, events);

    
    const float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(win.getSize().x, headerHeight));
    header.setFillColor(HEADER_COLOR_EV);
    header.setPosition(0.f, 0.f);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente en saveSegments()." << std::endl;
        return;
    }

    sf::Text headerTitle("Configuracion de Segmentos", font, 25);
    headerTitle.setFillColor(TEXT_COLOR_EV);
    {
        sf::FloatRect bounds = headerTitle.getLocalBounds();
        float posX = 20.f;
        float posY = (headerHeight - bounds.height) / 2.f - bounds.top;
        headerTitle.setPosition(posX, posY);
    }

    // Área "main" donde se pregunta cuántos segmentos se añadirán
    float formStartX = 50.f;
    float formStartY = headerHeight + 50.f;

    sf::Text instruction("", font, 20);
    instruction.setFillColor(sf::Color::White);
    instruction.setPosition(formStartX, formStartY);

    // Cajas para mostrar/ingresar cantidad de segmentos
    sf::RectangleShape boxNumSegments(sf::Vector2f(150.f, 30.f));
    boxNumSegments.setFillColor(sf::Color::White);
    boxNumSegments.setOutlineColor(sf::Color::Black);
    boxNumSegments.setOutlineThickness(1.f);
    boxNumSegments.setPosition(formStartX, formStartY + 40.f);

    sf::Text inputSegments("", font, 20);
    inputSegments.setFillColor(sf::Color::Black);
    inputSegments.setPosition(boxNumSegments.getPosition().x + 5, boxNumSegments.getPosition().y + 2);

    // Botón "Guardar"
    sf::Text saveButton("Guardar", font, 20);
    {
        float btnX = formStartX + 160.f;
        float btnY = formStartY + 40.f;
        saveButton.setPosition(btnX, btnY);
    }

    // Para resaltar la caja activa
    sf::RectangleShape activeBorder;
    activeBorder.setFillColor(sf::Color::Transparent);
    activeBorder.setOutlineColor(HIGHLIGHT_COLOR_EV);
    activeBorder.setOutlineThickness(2.f);
    activeBorder.setPosition(boxNumSegments.getPosition());
    activeBorder.setSize(boxNumSegments.getSize());

    bool inSegmentForm = true;
    bool haveSegmentsCount = false;
    std::string segmentsCountStr;

   
    auto eventNode = segmentsByEvent.getHead();
    auto countNode = segmentCount.getHead();
    for (int i = 1; i < eventIndex; i++) {
        if (eventNode)  eventNode = eventNode->next;
        if (countNode)  countNode = countNode->next;
    }

    while (inSegmentForm && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);
                // Clic en "Guardar" => se convierte la cadena a int
                sf::FloatRect sbBounds = saveButton.getGlobalBounds();
                if (sbBounds.contains(mousePos)) {
                    try {
                        if (!segmentsCountStr.empty()) {
                            int numSegments = std::stoi(segmentsCountStr);
                            if (numSegments > 0) {
                                haveSegmentsCount = true;
                                // Asignar en la lista de segmentCount
                                countNode->data = numSegments;
                                inSegmentForm = false; // Salimos del bucle para crear los segmentos
                            }
                        }
                    }
                    catch (...) {
                        // Si falla el stoi, ignoramos
                    }
                }
            }
            // Manejo de teclado
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Backspace && !segmentsCountStr.empty()) {
                    segmentsCountStr.pop_back();
                    inputSegments.setString(segmentsCountStr);
                }
                else if (ev.key.code == sf::Keyboard::Enter) {
                    // Similar a pulsar "Guardar"
                    try {
                        if (!segmentsCountStr.empty()) {
                            int numSegments = std::stoi(segmentsCountStr);
                            if (numSegments > 0) {
                                haveSegmentsCount = true;
                                countNode->data = numSegments;
                                inSegmentForm = false;
                            }
                        }
                    }
                    catch (...) {
                        // stoi falló
                    }
                }
            }
            // Captura de dígitos
            if (ev.type == sf::Event::TextEntered) {
                if (ev.text.unicode < 128) {
                    char c = static_cast<char>(ev.text.unicode);
                    if (isdigit(c)) {
                        segmentsCountStr.push_back(c);
                        inputSegments.setString(segmentsCountStr);
                    }
                }
            }
        }

        // Construir la instrucción en pantalla
        if (eventIndex > 0)
            instruction.setString("Indique cuantos segmentos desea para el evento #" + std::to_string(eventIndex) + ":");
        else
            instruction.setString("Indique cuantos segmentos desea para el evento actual:");

        // Render
        win.clear(BG_COLOR_EV);
        // Header
        win.draw(header);
        win.draw(headerTitle);

        // Texto principal
        win.draw(instruction);
        win.draw(boxNumSegments);
        win.draw(inputSegments);

        // Botón Guardar
        win.draw(saveButton);

        // Borde activo (para la caja de texto)
        win.draw(activeBorder);

        win.display();
    }

    
    if (haveSegmentsCount) {
        int numSegments = countNode->data;
        // Bucle para agregar cada uno de los segmentos
        for (int j = 1; j <= numSegments; j++) {
            Segment newSegment;
            addSegmentData(newSegment, j, win);
            eventNode->data.insertAtEnd(newSegment);
        }
    }
    else {
        
        return;
    }
}



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