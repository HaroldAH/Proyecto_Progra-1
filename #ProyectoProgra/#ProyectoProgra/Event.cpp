#include "Event.h"
#include "Segment.h"
#include "List.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;
namespace sfml = sf;  // Para usar sfml:: en vez de sf::

static const sfml::Color BG_COLOR_EV(200, 200, 200);
static const sfml::Color HEADER_COLOR_EV(160, 160, 160);
static const sfml::Color TEXT_COLOR_EV = sfml::Color::Black;
static const sfml::Color HIGHLIGHT_COLOR_EV = sfml::Color::Red;

Event::Event() {
    name = "";
    date = "";
    description = "";
    eventCount = 0;
}

Event::~Event() {}

string Event::getName() { return name; }
void Event::setName(string& aName) { name = aName; }
string Event::getDate() { return date; }
void Event::setDate(string& aDate) { date = aDate; }
string Event::getDescription() { return description; }
void Event::setDescription(string& aDescription) { description = aDescription; }
List<Event>& Event::getEvents() { return events; }
int Event::getEventCount() { return eventCount; }

void Event::initializeTracking(int maxUsers) {
    for (int i = 0; i < maxUsers; i++) {
        purchasesByUser.insertAtEnd(0);
        userIds.insertAtEnd("");
    }
}

bool Event::purchaseTickets(const string& userId, int numTickets) {
    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    while (userNode) {
        if (userNode->data == userId) {
            if (purchaseNode->data + numTickets > 5) {
                cout << "No puedes comprar más de 5 boletos para este evento.\n";
                return false;
            }
            purchaseNode->data += numTickets;
            return true;
        }
        if (userNode->data == "") {
            userNode->data = userId;
            purchaseNode->data = numTickets;
            return true;
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }

    cout << "No se pudo registrar la compra. Máximo de usuarios alcanzado.\n";
    return false;
}

int Event::getTicketsPurchasedByUser(const string& userId) {

    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    while (userNode) {
        if (userNode->data == userId) {
            return purchaseNode->data;
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }
    return 0;
}


bool Event::cancelTickets(const string& userId, int numTickets) {
    (void)userId; (void)numTickets;
    return true;
}

void Event::assignEvents(Event& evt, int numEvents) {
    if (numEvents <= 0) {
        cout << "El número de eventos debe ser mayor a cero." << endl;
        return;
    }
    for (int i = 1; i <= numEvents; i++) {
        evt.events.insertAtEnd(Event());
    }
    evt.eventCount += numEvents;
}

int Event::getValidIntInput() {
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

bool Event::isValidDate(string& date) {
    if (date.length() != 10) return false;
    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5) && date[i] != '/') return false;
        if (i != 2 && i != 5 && (date[i] < '0' || date[i] > '9')) return false;
    }
    if (date[6] != '2' || date[7] != '0') return false;
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    return true;
}

string Event::getValidStringInput(const string& prompt) {
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

// ----------------------------------------------------------------------
// Método que recibe una ventana SFML para pedir datos (nombre, fecha, etc.)
// y luego llama a "segment.saveSegments(...)" al final.
void Event::saveEvent(sfml::RenderWindow& win, Segment& segment) {
    // Cargar fuente
    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // Definir un "header" de 100 px
    float headerHeight = 100.f;
    sfml::RectangleShape header(sfml::Vector2f(win.getSize().x, headerHeight));
    header.setFillColor(HEADER_COLOR_EV);
    header.setPosition(0.f, 0.f);

    const int numOptions = 4;
    std::string optionLabels[numOptions] = {
        "Agregar nuevo evento",
        "Modificar segmentos",
        "Mostrar eventos actuales",
        "Inicio"
    };
    sfml::Text options[numOptions];
    float spacing = static_cast<float>(win.getSize().x) / numOptions;
    for (int i = 0; i < numOptions; i++) {
        options[i].setFont(font);
        options[i].setString(optionLabels[i]);
        options[i].setCharacterSize(20);
        options[i].setFillColor(TEXT_COLOR_EV);
        sfml::FloatRect textBounds = options[i].getLocalBounds();
        float posX = i * spacing + (spacing - textBounds.width) / 2.f - textBounds.left;
        float posY = (headerHeight - textBounds.height) / 2.f - textBounds.top;
        options[i].setPosition(posX, posY);
    }

    // Campos para formulario
    sfml::Text title("Nuevo Evento", font, 28);
    title.setFillColor(TEXT_COLOR_EV);
    sfml::FloatRect titleBounds = title.getLocalBounds();
    float formWidth = win.getSize().x - 100.f;
    float formHeight = win.getSize().y - headerHeight - 100.f;
    float offsetX = (win.getSize().x - formWidth) / 2.f;
    float offsetY = headerHeight + 50.f;
    title.setPosition(offsetX + (formWidth - titleBounds.width) / 2.f, offsetY);

    // Etiquetas/cajas para nombre, fecha, descripción
    sfml::Text labelName("Nombre del Evento:", font, 20);
    labelName.setFillColor(TEXT_COLOR_EV);
    labelName.setPosition(offsetX + 10.f, offsetY + 60.f);

    sfml::RectangleShape boxName(sfml::Vector2f(formWidth - 20.f, 40.f));
    boxName.setFillColor(sfml::Color::White);
    boxName.setPosition(offsetX + 10.f, offsetY + 85.f);

    sfml::Text inputName("", font, 20);
    inputName.setFillColor(TEXT_COLOR_EV);
    inputName.setPosition(offsetX + 15.f, offsetY + 90.f);

    // Fecha
    sfml::Text labelDate("Fecha (DD/MM/YYYY):", font, 20);
    labelDate.setFillColor(TEXT_COLOR_EV);
    labelDate.setPosition(offsetX + 10.f, offsetY + 140.f);

    sfml::RectangleShape boxDate(sfml::Vector2f(formWidth - 20.f, 40.f));
    boxDate.setFillColor(sfml::Color::White);
    boxDate.setPosition(offsetX + 10.f, offsetY + 165.f);

    sfml::Text inputDate("", font, 20);
    inputDate.setFillColor(TEXT_COLOR_EV);
    inputDate.setPosition(offsetX + 15.f, offsetY + 170.f);

    // Descripción
    sfml::Text labelDesc("Descripcion:", font, 20);
    labelDesc.setFillColor(TEXT_COLOR_EV);
    labelDesc.setPosition(offsetX + 10.f, offsetY + 220.f);

    sfml::RectangleShape boxDesc(sfml::Vector2f(formWidth - 20.f, 80.f));
    boxDesc.setFillColor(sfml::Color::White);
    boxDesc.setPosition(offsetX + 10.f, offsetY + 245.f);

    sfml::Text inputDesc("", font, 20);
    inputDesc.setFillColor(TEXT_COLOR_EV);
    inputDesc.setPosition(offsetX + 15.f, offsetY + 250.f);

    // Mensaje de instrucciones
    sfml::Text instruction("Presione Enter para guardar (Tab para cambiar campo)", font, 18);
    instruction.setFillColor(sfml::Color::Blue);
    instruction.setPosition(offsetX + 10.f, offsetY + formHeight - 40.f);

    // Variables para almacenar la entrada
    int activeField = 0; // 0 = nombre, 1 = fecha, 2 = descripción
    std::string strName, strDate, strDesc;

    bool inForm = true;
    while (inForm && win.isOpen()) {
        sfml::Event ev;
        while (win.pollEvent(ev)) {
            if (ev.type == sfml::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sfml::Event::KeyPressed) {
                if (ev.key.code == sfml::Keyboard::Tab) {
                    activeField = (activeField + 1) % 3;
                }
                else if (ev.key.code == sfml::Keyboard::Enter) {
                    if (!strName.empty() && !strDate.empty() && !strDesc.empty())
                        inForm = false;
                }
                else if (ev.key.code == sfml::Keyboard::Backspace) {
                    if (activeField == 0 && !strName.empty()) {
                        strName.pop_back();
                        inputName.setString(strName);
                    }
                    else if (activeField == 1 && !strDate.empty()) {
                        strDate.pop_back();
                        inputDate.setString(strDate);
                    }
                    else if (activeField == 2 && !strDesc.empty()) {
                        strDesc.pop_back();
                        inputDesc.setString(strDesc);
                    }
                }
            }
            if (ev.type == sfml::Event::TextEntered) {
                if (ev.text.unicode < 128 &&
                    ev.text.unicode != '\r' &&
                    ev.text.unicode != '\n' &&
                    ev.text.unicode != 9) {
                    char c = static_cast<char>(ev.text.unicode);
                    if (activeField == 0) {
                        strName.push_back(c);
                        inputName.setString(strName);
                    }
                    else if (activeField == 1) {
                        strDate.push_back(c);
                        inputDate.setString(strDate);
                    }
                    else if (activeField == 2) {
                        strDesc.push_back(c);
                        inputDesc.setString(strDesc);
                    }
                }
            }
        }

        win.clear(BG_COLOR_EV);
        // Header
        win.draw(header);
        for (int i = 0; i < numOptions; i++) {
            win.draw(options[i]);
        }
        // Formulario
        win.draw(title);
        win.draw(labelName);
        win.draw(boxName);
        win.draw(inputName);
        win.draw(labelDate);
        win.draw(boxDate);
        win.draw(inputDate);
        win.draw(labelDesc);
        win.draw(boxDesc);
        win.draw(inputDesc);
        win.draw(instruction);

        // Dibujar un borde de "resalte" en el campo activo
        sfml::RectangleShape activeBorder;
        activeBorder.setFillColor(sfml::Color::Transparent);
        activeBorder.setOutlineThickness(2.f);
        activeBorder.setOutlineColor(HIGHLIGHT_COLOR_EV);
        if (activeField == 0) {
            activeBorder.setSize(boxName.getSize());
            activeBorder.setPosition(boxName.getPosition());
        }
        else if (activeField == 1) {
            activeBorder.setSize(boxDate.getSize());
            activeBorder.setPosition(boxDate.getPosition());
        }
        else if (activeField == 2) {
            activeBorder.setSize(boxDesc.getSize());
            activeBorder.setPosition(boxDesc.getPosition());
        }
        win.draw(activeBorder);

        win.display();
    }

    // Al salir del formulario, asignamos los datos
    this->setName(strName);
    this->setDate(strDate);
    this->setDescription(strDesc);
    this->initializeTracking(100);

    // OJO: si quieres guardar este evento en tu lista 'events':
    // this->events.insertAtEnd(*this);
    // this->eventCount++;

    // Guardar segmentos para el nuevo evento:
    segment.saveSegments(segment, this->getEventCount(), this->getEventCount());
}