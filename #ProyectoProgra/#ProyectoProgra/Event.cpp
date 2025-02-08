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
static const sfml::Color BG_COLOR_EV(200, 200, 200);
static const sfml::Color HEADER_COLOR_EV(160, 160, 160);
static const sfml::Color TEXT_COLOR_EV = sfml::Color::Black;
static const sfml::Color HIGHLIGHT_COLOR_EV = sfml::Color::Red;

// ---------------------------------------------------------------------------
//                          IMPLEMENTACIÓN DE Event
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Lógica de compra/cancelación (como antes)
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

bool Event:: cancelTickets(string userId, int numTickets, sf::RenderWindow& window) {
    // Obtener los nodos iniciales de las listas
    List<int>::NodePtr purchaseNode = purchasesByUser.getHead();
    List<string>::NodePtr userNode = userIds.getHead();

    // Buscar el usuario en la lista
    while (userNode) {
        if (userNode->data == userId) {
            // Si se encontró al usuario, verificar si tiene suficientes boletos para cancelar
            if (purchaseNode->data >= numTickets) {
                purchaseNode->data -= numTickets;
                return true;
            }
            else {
                // Mostrar mensaje de error en la ventana SFML
                sf::Font font;
                if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                    std::cout << "Error: El usuario no tiene suficientes boletos para cancelar.\n";
                }
                else {
                    sf::Text errorMsg("Error: El usuario no tiene suficientes boletos para cancelar.", font, 24);
                    errorMsg.setFillColor(TEXT_COLOR_EV);
                    errorMsg.setPosition(50.f, 50.f);
                    window.clear(BG_COLOR_EV);
                    window.draw(errorMsg);
                    window.display();
                    sf::sleep(sf::seconds(2));
                }
                return false;
            }
        }
        userNode = userNode->next;
        purchaseNode = purchaseNode->next;
    }

    // Si no se encontró al usuario, mostrar mensaje de error en la ventana SFML
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Error: Usuario no encontrado.\n";
    }
    else {
        sf::Text errorMsg("Error: Usuario no encontrado.", font, 24);
        errorMsg.setFillColor(TEXT_COLOR_EV);
        errorMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(errorMsg);
        window.display();
        sf::sleep(sf::seconds(2));
    }
    return false;
}


// ---------------------------------------------------------------------------
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
    // EXACTAMENTE LA MISMA validación que ya tenías
    if (date.length() != 10) return false;

    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5) && date[i] != '/') return false;
        if (i != 2 && i != 5 && (date[i] < '0' || date[i] > '9')) return false;
    }
    // Validar que empiece con "20" en año, por ejemplo
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
// * Incluye verificación para rechazar una fecha inválida. *
void Event::saveEvent(sfml::RenderWindow& win, Segment& segment)
{
    // Cambiamos el título de la ventana durante la configuración del evento
    win.setTitle("Agregar Nuevo Evento");

    // Cargar fuente
    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // Dimensiones y colores
    const float headerHeight = 100.f;
    sfml::Color bgColor(200, 200, 200);
    sfml::Color headerColor(160, 160, 160);
    sfml::Color textColor = sfml::Color::Black;
    sfml::Color highlightColor = sfml::Color::Red;

    // 1. Encabezado (barra superior)
    sfml::RectangleShape header(sfml::Vector2f(1200.f, headerHeight));
    header.setFillColor(headerColor);
    header.setPosition(0.f, 0.f);

    // (Opcional) Texto de ejemplo en el header: "Agregar nuevo evento"
    sfml::Text headerTitle("Agregar nuevo evento", font, 25);
    headerTitle.setFillColor(textColor);
    {
        sfml::FloatRect bounds = headerTitle.getLocalBounds();
        float posX = 20.f;
        float posY = (headerHeight - bounds.height) / 2.f - bounds.top;
        headerTitle.setPosition(posX, posY);
    }

    // 2. Etiquetas y cajas de texto para: NOMBRE, FECHA, DESCRIPCIÓN
    // --------------------------------------------------------------
    // Preparar posiciones base
    float formStartX = 50.f;
    float formStartY = headerHeight + 50.f; // Debajo del header

    // Etiqueta: Nombre
    sfml::Text labelName("Nombre del Evento:", font, 20);
    labelName.setFillColor(textColor);
    labelName.setPosition(formStartX, formStartY);

    // Caja de texto para Nombre
    sfml::RectangleShape boxName(sfml::Vector2f(400.f, 30.f));
    boxName.setFillColor(sfml::Color::White);
    boxName.setOutlineColor(sfml::Color::Black);
    boxName.setOutlineThickness(1.f);
    boxName.setPosition(formStartX, formStartY + 30.f);

    // Texto que muestra lo que escribe el usuario (Nombre)
    sfml::Text inputName("", font, 20);
    inputName.setFillColor(textColor);
    inputName.setPosition(boxName.getPosition().x + 5.f, boxName.getPosition().y + 2.f);

    // Etiqueta: Fecha
    sfml::Text labelDate("Fecha (DD/MM/YYYY):", font, 20);
    labelDate.setFillColor(textColor);
    labelDate.setPosition(formStartX, formStartY + 80.f);

    // Caja de texto para Fecha
    sfml::RectangleShape boxDate(sfml::Vector2f(400.f, 30.f));
    boxDate.setFillColor(sfml::Color::White);
    boxDate.setOutlineColor(sfml::Color::Black);
    boxDate.setOutlineThickness(1.f);
    boxDate.setPosition(formStartX, formStartY + 80.f + 30.f);

    // Texto que muestra lo que escribe el usuario (Fecha)
    sfml::Text inputDate("", font, 20);
    inputDate.setFillColor(textColor);
    inputDate.setPosition(boxDate.getPosition().x + 5.f, boxDate.getPosition().y + 2.f);

    // Mensaje de error sobre la fecha
    sfml::Text dateErrorText("", font, 18);
    dateErrorText.setFillColor(sfml::Color::Red);
    dateErrorText.setPosition(formStartX, boxDate.getPosition().y + 40.f);

    // Etiqueta: Descripción
    sfml::Text labelDesc("Descripción:", font, 20);
    labelDesc.setFillColor(textColor);
    labelDesc.setPosition(formStartX, formStartY + 160.f);

    // Caja de texto para Descripción (un poco más grande)
    sfml::RectangleShape boxDesc(sfml::Vector2f(400.f, 60.f));
    boxDesc.setFillColor(sfml::Color::White);
    boxDesc.setOutlineColor(sfml::Color::Black);
    boxDesc.setOutlineThickness(1.f);
    boxDesc.setPosition(formStartX, formStartY + 160.f + 30.f);

    // Texto que muestra lo que escribe el usuario (Descripción)
    sfml::Text inputDesc("", font, 20);
    inputDesc.setFillColor(textColor);
    inputDesc.setPosition(boxDesc.getPosition().x + 5.f, boxDesc.getPosition().y + 2.f);

    // 3. Botones: "Guardar" y "Volver"
    // --------------------------------
    // Botón Guardar
    sfml::RectangleShape saveButton(sfml::Vector2f(120.f, 40.f));
    saveButton.setFillColor(sfml::Color(0, 180, 0)); // Verde
    saveButton.setPosition(formStartX, formStartY + 260.f);

    sfml::Text saveButtonText("Guardar", font, 20);
    saveButtonText.setFillColor(sfml::Color::White);
    {
        sfml::FloatRect btnBounds = saveButtonText.getLocalBounds();
        float btnX = saveButton.getPosition().x + (saveButton.getSize().x - btnBounds.width) / 2.f - btnBounds.left;
        float btnY = saveButton.getPosition().y + (saveButton.getSize().y - btnBounds.height) / 2.f - btnBounds.top;
        saveButtonText.setPosition(btnX, btnY);
    }

    // Botón Volver
    sfml::RectangleShape backButton(sfml::Vector2f(120.f, 40.f));
    backButton.setFillColor(sfml::Color(150, 150, 150)); // Gris
    backButton.setPosition(formStartX + 140.f, formStartY + 260.f);

    sfml::Text backButtonText("Volver", font, 20);
    backButtonText.setFillColor(sfml::Color::White);
    {
        sfml::FloatRect btnBounds = backButtonText.getLocalBounds();
        float btnX = backButton.getPosition().x + (backButton.getSize().x - btnBounds.width) / 2.f - btnBounds.left;
        float btnY = backButton.getPosition().y + (backButton.getSize().y - btnBounds.height) / 2.f - btnBounds.top;
        backButtonText.setPosition(btnX, btnY);
    }

    // 4. Variables de estado
    // ----------------------
    // Cadenas donde almacenamos los textos ingresados
    std::string strName;
    std::string strDate;
    std::string strDesc;

    // Para controlar cuál caja tiene "foco" (0 = ninguna, 1 = Nombre, 2 = Fecha, 3 = Desc)
    int activeField = 0;

    // Control del bucle
    bool inEventForm = true;

    // 5. Bucle principal de la pantalla "saveEvent"
    // ---------------------------------------------
    while (inEventForm && win.isOpen())
    {
        sfml::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sfml::Event::Closed)
            {
                // Cierra la ventana si el usuario así lo decide
                win.close();
                return;
            }

            // Clic del mouse: detectar si pulsamos cajas de texto o botones
            if (ev.type == sfml::Event::MouseButtonPressed && ev.mouseButton.button == sfml::Mouse::Left)
            {
                sfml::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));

                // ¿Clic en caja "Nombre"?
                if (boxName.getGlobalBounds().contains(mousePos))
                {
                    activeField = 1; // Nombre
                }
                // ¿Clic en caja "Fecha"?
                else if (boxDate.getGlobalBounds().contains(mousePos))
                {
                    activeField = 2; // Fecha
                }
                // ¿Clic en caja "Descripción"?
                else if (boxDesc.getGlobalBounds().contains(mousePos))
                {
                    activeField = 3; // Descripción
                }
                else
                {
                    // Si no se hizo clic en ninguna caja, se resetea a 0 (sin foco)
                    activeField = 0;
                }

                // ¿Clic en el botón Guardar?
                if (saveButton.getGlobalBounds().contains(mousePos))
                {
                    // Validar que todos los campos tengan algo
                    if (strName.empty() || strDate.empty() || strDesc.empty())
                    {
                        dateErrorText.setString("Por favor complete todos los campos.");
                    }
                    else
                    {
                        // Validar fecha
                        if (!isValidDate(strDate))
                        {
                            dateErrorText.setString("ERROR: Fecha invalida. Use DD/MM/YYYY (año >= 20xx)");
                        }
                        else
                        {
                            dateErrorText.setString("");

                            // Asignamos los datos al objeto actual
                            this->setName(strName);
                            this->setDate(strDate);
                            this->setDescription(strDesc);

                            // Inicializar el tracking de compras (puedes ajustar el número de usuarios)
                            this->initializeTracking(100);

                            // Se guarda en la lista de eventos
                            this->events.insertAtEnd(*this);
                            this->eventCount++;

                            // Llamar a la función que guarda segmentos para el evento que acabamos de crear
                            // El nuevo evento es siempre el 'getEventCount()'-ésimo
                            segment.saveSegments(win, segment, this->getEventCount(), this->getEventCount());

                            // Terminamos este formulario
                            inEventForm = false;
                        }
                    }
                }

                // ¿Clic en el botón Volver?
                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    // Simplemente salimos sin guardar nada adicional
                    inEventForm = false;
                }
            }

            // Manejo de teclado para escribir en la caja activa
            if (ev.type == sfml::Event::TextEntered)
            {
                // Solo caracteres "imprimibles" (evitamos los de control, excepto Backspace)
                if (ev.text.unicode >= 32 && ev.text.unicode < 127)
                {
                    char c = static_cast<char>(ev.text.unicode);

                    if (activeField == 1) // Nombre
                    {
                        strName.push_back(c);
                        inputName.setString(strName);
                    }
                    else if (activeField == 2) // Fecha
                    {
                        strDate.push_back(c);
                        inputDate.setString(strDate);
                        dateErrorText.setString(""); // limpiamos al escribir
                    }
                    else if (activeField == 3) // Descripción
                    {
                        strDesc.push_back(c);
                        inputDesc.setString(strDesc);
                    }
                }
            }

            // Manejo especial de Backspace
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
        } // Fin pollEvent

        // (Opcional) Resaltar la caja seleccionada
        boxName.setOutlineColor(activeField == 1 ? highlightColor : sfml::Color::Black);
        boxDate.setOutlineColor(activeField == 2 ? highlightColor : sfml::Color::Black);
        boxDesc.setOutlineColor(activeField == 3 ? highlightColor : sfml::Color::Black);

        // Renderizar
        win.clear(bgColor);

        // Dibujamos el header y su título
        win.draw(header);
        win.draw(headerTitle);

        // Etiquetas y cajas
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

        // Botones
        win.draw(saveButton);
        win.draw(saveButtonText);
        win.draw(backButton);
        win.draw(backButtonText);

        // Mostrar en pantalla
        win.display();
    }

    // Restablecemos el título de la ventana al salir
    win.setTitle("Sistema de Ventas de Entradas");
}
