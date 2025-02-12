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

// Colores para la interfaz
static const sfml::Color BG_COLOR_EV(50, 50, 50);     // Fondo oscuro suave
static const sfml::Color HEADER_COLOR_EV(80, 80, 80); // Encabezado gris oscuro
static const sfml::Color TEXT_COLOR_EV(230, 230, 230); // Texto en gris claro
static const sfml::Color HIGHLIGHT_COLOR_EV(255, 120, 0); // Resaltado en naranja



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
// ---------------------------------------------------------------------------
// Lógica de compra/cancelación (como antes)
bool Event::purchaseTickets(const string& userId, int numTickets, sfml::RenderWindow& window) {
    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    while (userNode) {
        if (userNode->data == userId) {
            if (purchaseNode->data + numTickets > 5) {
                // Mostrar mensaje de error en SFML
                sfml::Font font;
                if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                    sfml::Text errorMsg("No puedes comprar mas de 5 boletos para este evento.", font, 24);
                    errorMsg.setFillColor(TEXT_COLOR_EV);
                    errorMsg.setPosition(50.f, 50.f);
                    window.clear(BG_COLOR_EV);
                    window.draw(errorMsg);
                    window.display();
                    sfml::sleep(sfml::seconds(2));
                }
                return false;
            }
            purchaseNode->data += numTickets;
            return true;
        }
        if (userNode->data == "") {
            // Si aún no se ha registrado el usuario, se asigna aquí
            userNode->data = userId;
            purchaseNode->data = numTickets;
            return true;
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }
    // Si se llega al final sin encontrar espacio, mostrar error
    sfml::Font font;
    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        sfml::Text errorMsg("No se pudo registrar la compra. Maximo de usuarios alcanzado.", font, 24);
        errorMsg.setFillColor(TEXT_COLOR_EV);
        errorMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(errorMsg);
        window.display();
        sfml::sleep(sfml::seconds(2));
    }
    return false;
}

bool Event::cancelTickets(string userId, int numTickets, sfml::RenderWindow& window) {
    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    while (userNode) {
        if (userNode->data == userId) {
            if (purchaseNode->data >= numTickets) {
                purchaseNode->data -= numTickets;
                return true;
            }
            else {
                sfml::Font font;
                if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                    sfml::Text errorMsg("Error: El usuario no tiene suficientes boletos para cancelar.", font, 24);
                    errorMsg.setFillColor(TEXT_COLOR_EV);
                    errorMsg.setPosition(50.f, 50.f);
                    window.clear(BG_COLOR_EV);
                    window.draw(errorMsg);
                    window.display();
                    sfml::sleep(sfml::seconds(2));
                }
                return false;
            }
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }
    sfml::Font font;
    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        sfml::Text errorMsg("Error: Usuario no encontrado.", font, 24);
        errorMsg.setFillColor(TEXT_COLOR_EV);
        errorMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(errorMsg);
        window.display();
        sfml::sleep(sfml::seconds(2));
    }
    return false;
}



//  Asigna 'numEvents' al objeto 'evt' (ejemplo muy sencillo)
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
    // Mismo largo
    if (date.length() != 10) return false;

    // Mismo formato dd/mm/yyyy
    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5) && date[i] != '/') return false;
        if (i != 2 && i != 5 && (date[i] < '0' || date[i] > '9')) return false;
    }

 
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100
        + (date[8] - '0') * 10 + (date[9] - '0');

    if (year < 2025) return false;

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

void Event::saveEvent(sfml::RenderWindow& win, Segment& segment) {
    // Cambiar título de la ventana
    win.setTitle("Agregar Nuevo Evento");

    // Cargar la fuente
    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente." << std::endl;
        return;
    }

    // Definir dimensiones y colores
    const float headerHeight = 100.f;
    sfml::Color bgColor(50, 50, 50);      // Fondo oscuro igual que en Menu.cpp
    sfml::Color headerColor(70, 70, 70);  // Encabezado gris igual que en Menu.cpp
    sfml::Color textColor = sfml::Color::White;
    sfml::Color highlightColor = sfml::Color::Red;

    // Crear un header
    sfml::RectangleShape header(sfml::Vector2f(1200.f, headerHeight));
    header.setFillColor(headerColor);
    header.setPosition(0.f, 0.f);
    sfml::Text headerTitle("Agregar nuevo evento", font, 25);
    headerTitle.setFillColor(textColor);
    {
        sfml::FloatRect bounds = headerTitle.getLocalBounds();
        float posX = 20.f;
        float posY = (headerHeight - bounds.height) / 2.f - bounds.top;
        headerTitle.setPosition(posX, posY);
    }

    // Posiciones base para el formulario
    float formStartX = 50.f;
    float formStartY = headerHeight + 50.f;

    // Caja para el nombre
    sfml::Text labelName("Nombre del Evento:", font, 20);
    labelName.setFillColor(textColor);
    labelName.setPosition(formStartX, formStartY);
    sfml::RectangleShape boxName(sfml::Vector2f(400.f, 30.f));
    boxName.setFillColor(sfml::Color::White);
    boxName.setOutlineColor(sfml::Color::Black);
    boxName.setOutlineThickness(1.f);
    boxName.setPosition(formStartX, formStartY + 30.f);
    sfml::Text inputName("", font, 20);
    inputName.setFillColor(sfml::Color::Black);
    inputName.setPosition(boxName.getPosition().x + 5.f, boxName.getPosition().y + 2.f);

    // Caja para la fecha
    sfml::Text labelDate("Fecha (DD/MM/YYYY):", font, 20);
    labelDate.setFillColor(textColor);
    labelDate.setPosition(formStartX, formStartY + 80.f);
    sfml::RectangleShape boxDate(sfml::Vector2f(400.f, 30.f));
    boxDate.setFillColor(sfml::Color::White);
    boxDate.setOutlineColor(sfml::Color::Black);
    boxDate.setOutlineThickness(1.f);
    boxDate.setPosition(formStartX, formStartY + 80.f + 30.f);
    sfml::Text inputDate("", font, 20);
    inputDate.setFillColor(sfml::Color::Black);
    inputDate.setPosition(boxDate.getPosition().x + 5.f, boxDate.getPosition().y + 2.f);
    sfml::Text dateErrorText("", font, 12);
    dateErrorText.setFillColor(sfml::Color::Red);
    dateErrorText.setPosition(formStartX, boxDate.getPosition().y + 30.f);

    // Caja para la descripción
    sfml::Text labelDesc("Descripción:", font, 20);
    labelDesc.setFillColor(textColor);
    labelDesc.setPosition(formStartX, formStartY + 160.f);
    sfml::RectangleShape boxDesc(sfml::Vector2f(400.f, 60.f));
    boxDesc.setFillColor(sfml::Color::White);
    boxDesc.setOutlineColor(sfml::Color::Black);
    boxDesc.setOutlineThickness(1.f);
    boxDesc.setPosition(formStartX, formStartY + 160.f + 30.f);
    sfml::Text inputDesc("", font, 20);
    inputDesc.setFillColor(sfml::Color::Black);
    inputDesc.setPosition(boxDesc.getPosition().x + 5.f, boxDesc.getPosition().y + 2.f);

    // Botones "Guardar" y "Volver"
    sfml::RectangleShape saveButton(sfml::Vector2f(120.f, 40.f));
    saveButton.setFillColor(sfml::Color(0, 180, 0));
    saveButton.setPosition(formStartX, formStartY + 260.f);
    sfml::Text saveButtonText("Guardar", font, 20);
    saveButtonText.setFillColor(sfml::Color::White);
    {
        sfml::FloatRect btnBounds = saveButtonText.getLocalBounds();
        float btnX = saveButton.getPosition().x + (saveButton.getSize().x - btnBounds.width) / 2.f - btnBounds.left;
        float btnY = saveButton.getPosition().y + (saveButton.getSize().y - btnBounds.height) / 2.f - btnBounds.top;
        saveButtonText.setPosition(btnX, btnY);
    }
    sfml::RectangleShape backButton(sfml::Vector2f(120.f, 40.f));
    backButton.setFillColor(sfml::Color(150, 150, 150));
    backButton.setPosition(formStartX + 140.f, formStartY + 260.f);
    sfml::Text backButtonText("Volver", font, 20);
    backButtonText.setFillColor(sfml::Color::White);
    {
        sfml::FloatRect btnBounds = backButtonText.getLocalBounds();
        float btnX = backButton.getPosition().x + (backButton.getSize().x - btnBounds.width) / 2.f - btnBounds.left;
        float btnY = backButton.getPosition().y + (backButton.getSize().y - btnBounds.height) / 2.f - btnBounds.top;
        backButtonText.setPosition(btnX, btnY);
    }

    // Variables para almacenar los textos ingresados
    std::string strName;
    std::string strDate;
    std::string strDesc;
    int activeField = 0; // 0 = sin foco, 1 = Nombre, 2 = Fecha, 3 = Descripción
    bool inEventForm = true;

    // Bucle principal del formulario
    while (inEventForm && win.isOpen())
    {
        sfml::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sfml::Event::Closed)
            {
                win.close();
                return;
            }
            if (ev.type == sfml::Event::MouseButtonPressed && ev.mouseButton.button == sfml::Mouse::Left)
            {
                sfml::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                if (boxName.getGlobalBounds().contains(mousePos))
                    activeField = 1;
                else if (boxDate.getGlobalBounds().contains(mousePos))
                    activeField = 2;
                else if (boxDesc.getGlobalBounds().contains(mousePos))
                    activeField = 3;
                else
                    activeField = 0;
                if (saveButton.getGlobalBounds().contains(mousePos))
                {
                    if (strName.empty() || strDate.empty() || strDesc.empty())
                    {
                        dateErrorText.setString("Por favor complete todos los campos.");
                    }
                    else
                    {
                        if (!isValidDate(strDate))
                        {
                            dateErrorText.setString("ERROR: Fecha invalida. Use DD/MM/YYYY");
                        }
                        else
                        {
                            dateErrorText.setString("");
                           
                            // creamos un nuevo objeto de tipo Event y lo llenamos con los datos ingresados.
                            Event newEvent;
                            newEvent.setName(strName);
                            newEvent.setDate(strDate);
                            newEvent.setDescription(strDesc);
                            newEvent.initializeTracking(100);
                            // Insertamos el nuevo evento en la lista del objeto global.
                            this->events.insertAtEnd(newEvent);
                            this->eventCount++;
                            // Se invoca la función de segmentos para el nuevo evento.
                            // Se asume que segment.saveSegments acepta (ventana, segment, indiceEvento, indiceEvento)
                            segment.saveSegments(win, segment, this->getEventCount(), this->getEventCount());
                            
                            sfml::Text successMsg("Evento guardado", font, 24);
                            successMsg.setFillColor(sfml::Color::Red);
                            successMsg.setPosition(50.f, 300.f);  // Ajusta la posición si es necesario

                            // Mostrar mensaje por 1 segundo
                            win.clear(bgColor);
                            win.draw(header);
                            win.draw(headerTitle);
                            win.draw(successMsg);
                            win.display();
                            sfml::sleep(sfml::seconds(1));

                            inEventForm = false;
                        }
                    }
                }
                if (backButton.getGlobalBounds().contains(mousePos))
                    inEventForm = false;
            }
            if (ev.type == sfml::Event::TextEntered)
            {
                if (ev.text.unicode >= 32 && ev.text.unicode < 127)
                {
                    char c = static_cast<char>(ev.text.unicode);
                    if (activeField == 1)
                    {
                        strName.push_back(c);
                        inputName.setString(strName);
                    }
                    else if (activeField == 2)
                    {
                        strDate.push_back(c);
                        inputDate.setString(strDate);
                        dateErrorText.setString("");
                    }
                    else if (activeField == 3)
                    {
                        strDesc.push_back(c);
                        inputDesc.setString(strDesc);
                    }
                }
            }
            if (ev.type == sfml::Event::KeyPressed && ev.key.code == sfml::Keyboard::Backspace)
            {
                if (activeField == 1 && !strName.empty())
                {
                    strName.pop_back();
                    inputName.setString(strName);
                }
                else if (activeField == 2 && !strDate.empty())
                {
                    strDate.pop_back();
                    inputDate.setString(strDate);
                }
                else if (activeField == 3 && !strDesc.empty())
                {
                    strDesc.pop_back();
                    inputDesc.setString(strDesc);
                }
            }
        }
        boxName.setOutlineColor(activeField == 1 ? highlightColor : sfml::Color::Black);
        boxDate.setOutlineColor(activeField == 2 ? highlightColor : sfml::Color::Black);
        boxDesc.setOutlineColor(activeField == 3 ? highlightColor : sfml::Color::Black);
        win.clear(bgColor);
        win.draw(header);
        win.draw(headerTitle);
        win.draw(labelName);
        win.draw(boxName);
        win.draw(inputName);
        win.draw(labelDate);
        win.draw(boxDate);
        win.draw(inputDate);
        win.draw(dateErrorText);
        win.draw(labelDesc);
        win.draw(boxDesc);
        win.draw(inputDesc);
        win.draw(saveButton);
        win.draw(saveButtonText);
        win.draw(backButton);
        win.draw(backButtonText);
        win.display();
    }
    // Al salir, se restablece el título de la ventana
    win.setTitle("Sistema de Ventas de Entradas");
}
