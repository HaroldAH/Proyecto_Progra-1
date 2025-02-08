#include "User.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#include <limits>
#include <cctype>

// Asumiendo que ya tienes definidas estas constantes:
static const sf::Color BG_COLOR_EV(200, 200, 200);
static const sf::Color HEADER_COLOR_EV(160, 160, 160);
static const sf::Color TEXT_COLOR_EV = sf::Color::Black;
static const sf::Color HIGHLIGHT_COLOR_EV = sf::Color::Red;

User::User() {
    userCount = 0;
}

User::~User() {}

void User::expandAndAssignUsers(User& usersObj, int quantity) {
    for (int i = 0; i < quantity; i++) {
        usersObj.users.insertAtEnd(UserData());
    }
}

void User::createUser(User& usersObj, const std::string& idNumber, sf::RenderWindow& win) {
    // Expandir la lista de usuarios y obtener el último nodo
    expandAndAssignUsers(usersObj, 1);
    List<UserData>::NodePtr userNode = usersObj.users.getHead();
    while (userNode->next) {
        userNode = userNode->next;
    }
    userNode->data.setIdNumber(idNumber);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return;
    }

    // Configuración de elementos gráficos para ambos campos en una misma ventana

    // Campo: Nombre
    sf::Text nameLabel("Ingrese el nombre del usuario:", font, 24);
    nameLabel.setFillColor(TEXT_COLOR_EV);
    nameLabel.setPosition(50.f, 50.f);

    sf::RectangleShape nameBox(sf::Vector2f(300.f, 30.f));
    nameBox.setFillColor(sf::Color::White);
    nameBox.setOutlineColor(sf::Color::Black);
    nameBox.setOutlineThickness(1.f);
    nameBox.setPosition(50.f, 90.f);

    sf::Text nameInput("", font, 24);
    nameInput.setFillColor(TEXT_COLOR_EV);
    nameInput.setPosition(55.f, 95.f);

    // Campo: Fecha de nacimiento
    sf::Text birthLabel("Ingrese la fecha de nacimiento (DD/MM/AAAA):", font, 24);
    birthLabel.setFillColor(TEXT_COLOR_EV);
    birthLabel.setPosition(50.f, 140.f);

    sf::RectangleShape birthBox(sf::Vector2f(300.f, 30.f));
    birthBox.setFillColor(sf::Color::White);
    birthBox.setOutlineColor(sf::Color::Black);
    birthBox.setOutlineThickness(1.f);
    birthBox.setPosition(50.f, 180.f);

    sf::Text birthInput("", font, 24);
    birthInput.setFillColor(TEXT_COLOR_EV);
    birthInput.setPosition(55.f, 185.f);

    // Mensaje de error (en rojo, debajo de ambos campos)
    sf::Text errorText("", font, 20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50.f, 230.f);

    // Borde para indicar el campo activo
    sf::RectangleShape activeBorder;
    activeBorder.setFillColor(sf::Color::Transparent);
    activeBorder.setOutlineColor(HIGHLIGHT_COLOR_EV);
    activeBorder.setOutlineThickness(2.f);

    // Variable para saber qué campo está activo: 0 = nombre, 1 = fecha
    int activeField = 0;
    activeBorder.setPosition(nameBox.getPosition());
    activeBorder.setSize(nameBox.getSize());

    std::string name;
    std::string birthDate;

    bool done = false;
    while (!done && win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                // Con la tecla Tab se cambia el campo activo
                if (event.key.code == sf::Keyboard::Tab) {
                    activeField = (activeField + 1) % 2;
                    if (activeField == 0) {
                        activeBorder.setPosition(nameBox.getPosition());
                        activeBorder.setSize(nameBox.getSize());
                    }
                    else {
                        activeBorder.setPosition(birthBox.getPosition());
                        activeBorder.setSize(birthBox.getSize());
                    }
                }
                // La tecla Enter: si se está en el campo nombre, se pasa al campo fecha;
                // si se está en el campo fecha, se intenta confirmar los datos
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (activeField == 0) {
                        // Si se presiona Enter en el campo nombre, pasa a la fecha
                        activeField = 1;
                        activeBorder.setPosition(birthBox.getPosition());
                        activeBorder.setSize(birthBox.getSize());
                    }
                    else if (activeField == 1) {
                        // Si se presiona Enter en el campo fecha, se valida
                        if (name.empty()) {
                            errorText.setString("El nombre no puede estar vacio.");
                        }
                        else if (birthDate.empty() || !isValidDate(birthDate)) {
                            errorText.setString("Fecha invalida. Intente nuevamente.");
                        }
                        else {
                            done = true;
                        }
                    }
                }
                // Tecla Backspace para borrar el último caracter
                else if (event.key.code == sf::Keyboard::Backspace) {
                    if (activeField == 0 && !name.empty()) {
                        name.pop_back();
                        nameInput.setString(name);
                    }
                    else if (activeField == 1 && !birthDate.empty()) {
                        birthDate.pop_back();
                        birthInput.setString(birthDate);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    // Si el campo activo es nombre, se aceptan caracteres imprimibles
                    if (activeField == 0) {
                        if (std::isprint(c)) {
                            name.push_back(c);
                            nameInput.setString(name);
                        }
                    }
                    // Si el campo activo es fecha, se aceptan dígitos y la barra '/'
                    else if (activeField == 1) {
                        if (std::isdigit(c) || c == '/') {
                            birthDate.push_back(c);
                            birthInput.setString(birthDate);
                        }
                    }
                }
            }
        }
        win.clear(BG_COLOR_EV);
        win.draw(nameLabel);
        win.draw(nameBox);
        win.draw(nameInput);
        win.draw(birthLabel);
        win.draw(birthBox);
        win.draw(birthInput);
        win.draw(errorText);
        win.draw(activeBorder);
        win.display();
    }

    // Una vez confirmados los datos, se asignan al usuario
    userNode->data.setName(name);
    userNode->data.setBirthDate(birthDate);
    userCount++;
}

bool User::isValidDate(string& birthDate) {

    if (birthDate.length() != 10) return false;

    for (int i = 0; i < 10; i++) {
        if ((i == 2 || i == 5) && birthDate[i] != '/') return false;
        if (i != 2 && i != 5 && (birthDate[i] < '0' || birthDate[i] > '9')) return false;
    }

    int day = (birthDate[0] - '0') * 10 + (birthDate[1] - '0');
    int month = (birthDate[3] - '0') * 10 + (birthDate[4] - '0');

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}

UserData* User::searchUserById(string& idNumber) {
    List<UserData>::NodePtr userNode = users.getHead();

    while (userNode) {
        if (userNode->data.getIdNumber() == idNumber) {
            return &(userNode->data);
        }
        userNode = userNode->next;
    }
    return nullptr;
}

List<UserData>& User::getUsers() {
    return users;
}

int User::getValidIntInput() {
    int val;
    while (true) {
        if (cin >> val) {
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente nuevamente: ";
    }
}
