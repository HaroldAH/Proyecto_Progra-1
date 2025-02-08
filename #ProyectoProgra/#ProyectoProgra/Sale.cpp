#include "Sale.h"
#include <sstream>

using namespace std;


void Sale::sell(User& user, Event& event, Segment& segment,
    std::map<std::tuple<int, int>, Seating>& seatingMap, Discount& discount,
    sf::RenderWindow& window)
{
    // 1. Verificar disponibilidad de eventos.
    if (!checkEventsAvailability(event))
    {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            return;
        sf::Text msg("No hay eventos disponibles.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(msg);
        window.display();
        sf::sleep(sf::seconds(2));
        return;
    }

    // 2. Solicitar o registrar usuario (suponemos que getOrRegisterUser ya fue convertido a SFML).
    UserData* currentUser = getOrRegisterUser(user, window);
    if (!currentUser)
        return;

    // 3. Seleccionar evento (versión SFML).
    int selectedEvent = chooseEvent(event, window);
    if (selectedEvent < 0)
        return;

    // 4. Seleccionar segmento (versión SFML).
    int selectedSegment = chooseSegment(segment, selectedEvent, window);

    // 5. Obtener listas y cantidad de segmentos.
    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();
    int numSegments = segmentCounts.getAt(selectedEvent);

    // 6. Asegurar la sala (se usará el parámetro window para dibujar la sala)
    Seating* seatingPtr = nullptr;
    while (true)
    {
        Seating& seating = ensureSeating(selectedEvent, selectedSegment, segments, seatingMap, window);

        int availableSeats = 0;
        for (int i = 0; i < seating.getNumberOfRows(); i++)
            for (int j = 0; j < seating.getNumberOfColumns(); j++)
                if (!seating.getSeatPurchased()[i][j])
                    availableSeats++;

        if (availableSeats > 0)
        {
            seatingPtr = &seating;
            break;
        }

        // Si no hay asientos disponibles en el segmento seleccionado, buscar una alternativa.
        bool alternativeAvailable = false;
        if (numSegments > 1)
        {
            for (int seg = 1; seg <= numSegments; seg++)
            {
                if (seg == selectedSegment)
                    continue;

                Seating& otherSeating = ensureSeating(selectedEvent, seg, segments, seatingMap, window);
                int otherAvailable = 0;
                for (int r = 0; r < otherSeating.getNumberOfRows(); r++)
                    for (int c = 0; c < otherSeating.getNumberOfColumns(); c++)
                        if (!otherSeating.getSeatPurchased()[r][c])
                            otherAvailable++;
                if (otherAvailable > 0)
                {
                    alternativeAvailable = true;
                    break;
                }
            }
        }

        if (!alternativeAvailable)
        {
            sf::Font font;
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
                return;
            sf::Text fullMsg("El segmento seleccionado está lleno. No se pueden comprar más boletos.", font, 24);
            fullMsg.setFillColor(TEXT_COLOR_EV);
            fullMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(fullMsg);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

        // Si hay alternativas, solicitar al usuario elegir otro segmento.
        selectedSegment = chooseSegment(segment, selectedEvent, window);
    }
    Seating& seating = *seatingPtr;

    // 7. Solicitar el número de boletos (se debe implementar una función gráfica; aquí se usa una función adaptada).
    int numTickets = buyTickets(currentUser, event, selectedEvent, window);
    if (numTickets <= 0)
        return;

    // 8. Para cada boleto, solicitar la selección del asiento.
    // Aquí deberías implementar una función de entrada gráfica para escoger fila y columna.
    // En este ejemplo se simulan los datos (por ejemplo, siempre se reserva el asiento en fila 1, columna 'A').
    int* purchasedRows = new int[numTickets];
    char* purchasedCols = new char[numTickets];
    for (int i = 0; i < numTickets; i++)
    {
        // Aquí llamarías a: std::pair<int, char> seat = getSeatSelection(window, seating);
        // Para el ejemplo, usamos valores fijos:
        purchasedRows[i] = 1;      // Fila 1
        purchasedCols[i] = 'A';    // Columna 'A'

        // Mostrar mensaje temporal en la ventana
        sf::Font font;
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
        sf::Text seatMsg("Asiento reservado para boleto " + std::to_string(i+1), font, 20);
        seatMsg.setFillColor(sf::Color::Black);
        seatMsg.setPosition(50.f, 400.f + i * 30.f);
        window.clear(BG_COLOR_EV);
        window.draw(seatMsg);
        window.display();
        sf::sleep(sf::seconds(1));
    }

    // 9. Aplicar descuento (aquí podrías también implementar una entrada gráfica; en este ejemplo se mantiene la función actual).
    float discountPercentage = applyDiscountIfWanted(discount);
    float ticketPrice = seating.getCost();
    float totalCost = ticketPrice * numTickets;
    if (discountPercentage > 0)
        totalCost -= totalCost * (discountPercentage / 100.0f);

    // 10. Pedir el número de tarjeta (deberías implementar una función gráfica; aquí se usa un valor fijo para el ejemplo).
    string cardNumber = "123456789012";

    // 11. Mostrar la factura (aquí se muestra en la consola; puedes crear una versión gráfica similar).
    printInvoice(currentUser, event, selectedEvent, segments, selectedSegment,
        numTickets, ticketPrice, discountPercentage, totalCost,
        purchasedRows, purchasedCols, numTickets, cardNumber);

    // 12. Pausa para que el usuario vea la factura (se usa sf::sleep).
    sf::sleep(sf::seconds(3));

    delete[] purchasedRows;
    delete[] purchasedCols;
}

bool Sale::checkEventsAvailability(Event& event) {

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        cout << "\nPresione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return false;
    }
    return true;
}
UserData* Sale::getOrRegisterUser(User& user, sf::RenderWindow& win) {
    while (true) {
        // Cargar la fuente
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Error al cargar la fuente." << std::endl;
            return nullptr;
        }

        // Texto de la consigna
        sf::Text promptText("Ingrese su numero de cedula (9 digitos):", font, 24);
        promptText.setFillColor(TEXT_COLOR_EV);
        promptText.setPosition(50.f, 50.f);

        // Caja de entrada más pequeña: 300 x 30
        sf::RectangleShape inputBox(sf::Vector2f(300.f, 30.f));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(1.f);
        inputBox.setPosition(50.f, 120.f);

        // Texto para mostrar lo que el usuario escribe
        sf::Text inputText("", font, 24);
        inputText.setFillColor(TEXT_COLOR_EV);
        inputText.setPosition(55.f, 125.f);

        // Texto de error, en rojo y posicionado más abajo
        sf::Text errorText("", font, 20);
        errorText.setFillColor(sf::Color::Red);
        errorText.setPosition(50.f, 160.f);

        std::string idNumber;
        bool done = false;
        while (!done && win.isOpen()) {
            sf::Event event;
            while (win.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    win.close();
                    return nullptr;
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        // Validar que la cédula tenga 9 dígitos y sea numérica
                        if (idNumber.length() == 9 && idNumber.find_first_not_of("0123456789") == std::string::npos) {
                            done = true;
                        }
                        else {
                            errorText.setString("Cedula invalida. Debe contener 9 digitos numericos.");
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Backspace) {
                        if (!idNumber.empty()) {
                            idNumber.pop_back();
                            inputText.setString(idNumber);
                            errorText.setString("");
                        }
                    }
                }
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode < 128) {
                        char c = static_cast<char>(event.text.unicode);
                        if (std::isdigit(c)) {
                            idNumber.push_back(c);
                            inputText.setString(idNumber);
                            errorText.setString("");
                        }
                    }
                }
            }
            win.clear(BG_COLOR_EV);
            win.draw(promptText);
            win.draw(inputBox);
            win.draw(inputText);
            win.draw(errorText);
            win.display();
        }

        // Buscar el usuario por cédula
        UserData* currentUser = user.searchUserById(idNumber);
        if (currentUser) {
            std::string welcomeMessage = "Usuario encontrado. Bienvenido, " + currentUser->getName() + ".";
            sf::Text welcomeText(welcomeMessage, font, 24);
            welcomeText.setFillColor(TEXT_COLOR_EV);
            welcomeText.setPosition(50.f, 200.f);
            win.clear(BG_COLOR_EV);
            win.draw(welcomeText);
            win.display();
            sf::sleep(sf::seconds(2)); // Pausa para mostrar el mensaje
            return currentUser;
        }

        // Si no se encuentra al usuario, mostrar dos botones para elegir registrar
        sf::Text registerPrompt("Usuario no encontrado. ¿Desea registrarse?", font, 24);
        registerPrompt.setFillColor(TEXT_COLOR_EV);
        registerPrompt.setPosition(50.f, 250.f);

        // Botón "Si": verde
        sf::RectangleShape siButton(sf::Vector2f(100.f, 40.f));
        siButton.setFillColor(sf::Color::Green);
        siButton.setPosition(50.f, 320.f);
        sf::Text siText("Si", font, 24);
        siText.setFillColor(sf::Color::White);
        {
            sf::FloatRect siBounds = siText.getLocalBounds();
            siText.setPosition(
                siButton.getPosition().x + (siButton.getSize().x - siBounds.width) / 2.f - siBounds.left,
                siButton.getPosition().y + (siButton.getSize().y - siBounds.height) / 2.f - siBounds.top
            );
        }

        // Botón "No": rojo
        sf::RectangleShape noButton(sf::Vector2f(100.f, 40.f));
        noButton.setFillColor(sf::Color::Red);
        noButton.setPosition(200.f, 320.f);
        sf::Text noText("No", font, 24);
        noText.setFillColor(sf::Color::White);
        {
            sf::FloatRect noBounds = noText.getLocalBounds();
            noText.setPosition(
                noButton.getPosition().x + (noButton.getSize().x - noBounds.width) / 2.f - noBounds.left,
                noButton.getPosition().y + (noButton.getSize().y - noBounds.height) / 2.f - noBounds.top
            );
        }

        bool selectionMade = false;
        bool registerUser = false;
        while (!selectionMade && win.isOpen()) {
            sf::Event event;
            while (win.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    win.close();
                    return nullptr;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    if (siButton.getGlobalBounds().contains(mousePos)) {
                        registerUser = true;
                        selectionMade = true;
                    }
                    else if (noButton.getGlobalBounds().contains(mousePos)) {
                        registerUser = false;
                        selectionMade = true;
                    }
                }
            }
            win.clear(BG_COLOR_EV);
            win.draw(registerPrompt);
            win.draw(siButton);
            win.draw(siText);
            win.draw(noButton);
            win.draw(noText);
            win.display();
        }

        if (!registerUser) {
            std::cout << "Regresando al menu principal...\n";
            return nullptr;
        }

        // Proceder a registrar el usuario
        user.createUser(user, idNumber,win);
        currentUser = user.searchUserById(idNumber);
        if (currentUser) {
            std::string successMessage = "Usuario registrado. Bienvenido, " + currentUser->getName();
            sf::Text successText(successMessage, font, 24);
            successText.setFillColor(TEXT_COLOR_EV);
            successText.setPosition(50.f, 200.f);
            win.clear(BG_COLOR_EV);
            win.draw(successText);
            win.display();
            sf::sleep(sf::seconds(2)); // Pausa para mostrar el mensaje
            return currentUser;
        }

        std::cout << "Error al registrar el usuario. Intente nuevamente.\n";
    }
}


int Sale::chooseSegment(Segment& segment, int selectedEvent, sf::RenderWindow& window)
{
    // Acceder a la lista de segmentos y cantidad
    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();
    int numSegments = segmentCounts.getAt(selectedEvent);

    // Cargar fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente en chooseSegment." << std::endl;
        return -1;
    }

    // Crear un vector para almacenar las opciones como sf::Text
    std::vector<sf::Text> options;
    float startY = 100.f;    // Posición vertical inicial para las opciones
    float spacing = 40.f;    // Espaciado vertical entre cada opción

    for (int i = 1; i <= numSegments; i++) {
        std::ostringstream oss;
        oss << i << ". "
            << segments.getAt(selectedEvent).getAt(i).getName()
            << " - Precio: $"
            << segments.getAt(selectedEvent).getAt(i).getPrice();
        sf::Text optionText(oss.str(), font, 24);
        optionText.setFillColor(TEXT_COLOR_EV);
        optionText.setPosition(50.f, startY + (i - 1) * spacing);
        options.push_back(optionText);
    }

    // Bucle para esperar la selección mediante clic
    bool selected = false;
    int chosen = -1;
    while (window.isOpen() && !selected) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return -1;
            }
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y));
                // Resaltar la opción sobre la que pasa el mouse
                for (auto& opt : options) {
                    if (opt.getGlobalBounds().contains(mousePos))
                        opt.setFillColor(HIGHLIGHT_COLOR_EV);
                    else
                        opt.setFillColor(TEXT_COLOR_EV);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y));
                for (int i = 0; i < options.size(); i++) {
                    if (options[i].getGlobalBounds().contains(mousePos)) {
                        chosen = i + 1;
                        selected = true;
                        break;
                    }
                }
            }
        }
        window.clear(BG_COLOR_EV);

        // Dibujar el título en la parte superior
        sf::Text title("Segmentos", font, 28);
        title.setFillColor(TEXT_COLOR_EV);
        title.setPosition(50.f, 50.f);
        window.draw(title);

        // Dibujar las opciones
        for (auto& opt : options) {
            window.draw(opt);
        }
        window.display();
    }
    return chosen;
}




Seating& Sale::ensureSeating(int selectedEvent, int selectedSegment,
    List<List<Segment>>& segments,
    std::map<std::tuple<int, int>, Seating>& seatingMap,
    sf::RenderWindow& win)   // Agregado el parámetro 'win'
{
    std::tuple<int, int> seatingKey = std::make_tuple(selectedEvent, selectedSegment);

    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        Seating newSeating;
        Segment& seg = segments.getAt(selectedEvent).getAt(selectedSegment);
        newSeating.setNumberOfRows(seg.getRows());
        newSeating.setNumberOfColumns(seg.getSeats());
        newSeating.setCost(seg.getPrice());
        newSeating.initializeRoom();
        seatingMap[seatingKey] = newSeating;
    }

    Seating& seating = seatingMap[seatingKey];
    int availableSeats = 0;
    for (int i = 0; i < seating.getNumberOfRows(); i++) {
        for (int j = 0; j < seating.getNumberOfColumns(); j++) {
            if (!seating.getSeatPurchased()[i][j]) {
                availableSeats++;
            }
        }
    }
    if (availableSeats > 0) {
        seating.displaySeats(win);  // Dibuja la sala en el buffer de la ventana
        win.display();              // Actualiza la ventana para mostrar el dibujo

        // (Opcional) Espera unos segundos o hasta que se pulse una tecla
        sf::sleep(sf::seconds(2));
    }
    else {
        // Este mensaje se repite con lo que haces en `sell()`
        cout << " " << endl;
    }


    return seating;
}


int Sale::buyTickets(UserData* currentUser, Event& event, int selectedEvent, sf::RenderWindow& window) {
    // Obtener el ID del usuario y los boletos ya comprados
   
    string userId = currentUser->getIdNumber();
    int currentTickets = event.getEvents().getAt(selectedEvent).getTicketsPurchasedByUser(userId);

    // Si ya se alcanzó el máximo, mostrar mensaje y retornar 0
    if (currentTickets >= 5) {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            cerr << "Error al cargar la fuente." << endl;
            return 0;
        }
        sf::Text message("Ya has comprado el numero maximo de 5 boletos para este evento.", font, 24);
        message.setFillColor(TEXT_COLOR_EV);
        message.setPosition(50.f, 50.f);

        window.clear(BG_COLOR_EV);
        window.draw(message);
        window.display();
        sf::sleep(sf::seconds(5));
     
        return 0;
    }

    int maxTickets = 5 - currentTickets;

    // Preparar la interfaz de entrada
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "Error al cargar la fuente." << endl;
        return 0;
    }

    // Texto de consigna
    sf::Text prompt("¿Cuantos boletos desea comprar? (max " + to_string(maxTickets) + "):", font, 24);
    prompt.setFillColor(TEXT_COLOR_EV);
    prompt.setPosition(50.f, 50.f);

    // Caja de entrada
    sf::RectangleShape inputBox(sf::Vector2f(200.f, 40.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50.f, 120.f);

    // Texto para mostrar lo ingresado
    sf::Text inputText("", font, 24);
    inputText.setFillColor(TEXT_COLOR_EV);
    inputText.setPosition(55.f, 125.f);

    // Texto para mensajes de error
    sf::Text errorText("", font, 20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50.f, 180.f);

    string ticketStr;
    bool done = false;

    // Bucle principal de entrada
    while (!done && window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Enter) {
                    // Al presionar Enter se intenta convertir la cadena a entero
                    try {
                        int numTickets = stoi(ticketStr);
                        if (numTickets >= 1 && numTickets <= maxTickets) {
                            // Intentar registrar la compra
                            if (event.getEvents().getAt(selectedEvent).purchaseTickets(userId, numTickets)) {
                                return numTickets;
                            }
                            else {
                                errorText.setString("No se pudo completar la compra. Intente nuevamente.");
                                ticketStr = "";
                                inputText.setString(ticketStr);
                            }
                        }
                        else {
                            errorText.setString("Ingrese un numero entre 1 y " + to_string(maxTickets));
                        }
                    }
                    catch (...) {
                        errorText.setString("Entrada invalida. Ingrese un numero valido.");
                    }
                }
                else if (ev.key.code == sf::Keyboard::Backspace) {
                    if (!ticketStr.empty()) {
                        ticketStr.pop_back();
                        inputText.setString(ticketStr);
                        errorText.setString("");
                    }
                }
            }
            if (ev.type == sf::Event::TextEntered) {
                if (ev.text.unicode < 128) {
                    char c = static_cast<char>(ev.text.unicode);
                    if (isdigit(c)) {
                        ticketStr.push_back(c);
                        inputText.setString(ticketStr);
                        errorText.setString("");
                    }
                }
            }
        }
        window.clear(BG_COLOR_EV);
        window.draw(prompt);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(errorText);
        window.display();
    }

    return 0; // En caso de que se cierre la ventana o salga el bucle sin seleccionar
}

float Sale::applyDiscountIfWanted(Discount& discount) {

    char useDiscount;

    cout << "\nTiene un codigo de descuento? (S/N): ";
    cin >> useDiscount;
    cin.ignore();
    useDiscount = tolower(useDiscount);

    if (useDiscount != 's') {
        return 0.0f;
    }

    while (true) {

        string discountCode;
        cout << "Ingrese su codigo de descuento: ";
        getline(cin, discountCode);

        if (discount.verifyCode(discountCode)) {
            float discountPercentage = discount.getDiscountPercentage();
            cout << "Descuento aplicado: " << discountPercentage << "%\n";
            return discountPercentage;
        }

        cout << "Codigo de descuento invalido o ya utilizado.\n";

        while (true) {
            cout << "Desea intentarlo nuevamente? (S/N): ";
            cin >> useDiscount;
            cin.ignore();
            useDiscount = tolower(useDiscount);

            if (useDiscount == 's' || useDiscount == 'n') {
                break;
            }
            cout << "Opcion no valida. Por favor ingrese S o N.\n";
        }

        if (useDiscount == 'n') {
            break;
        }
    }

    cout << "No se aplico ningun descuento.\n";
    return 0.0f;
}

string Sale::askCardNumber() {
    string cardNumber;

    while (true) {
        cout << "\nIngrese el numero de su tarjeta (12 digitos): ";
        cin >> cardNumber;

        if (cardNumber.length() == 12 &&
            cardNumber.find_first_not_of("0123456789") == string::npos) {
            cout << "Numero de tarjeta valido.\n";
            break;
        }

        cout << "Numero de tarjeta invalido. Por favor, ingrese exactamente 12 digitos numericos.\n";
    }

    return cardNumber;
}


void Sale::printInvoice(UserData* currentUser, Event& event, int selectedEvent, List<List<Segment>>& segments, int selectedSegment,
    int numTickets, float ticketPrice, float discountPercentage, float totalCost,
    int* purchasedRows, char* purchasedCols, int numPurchasedSeats, std::string cardNumber)
{
    cout << "\n\n==================== FACTURA ====================\n\n";
    cout << "Usuario: " << currentUser->getName() << endl;
    cout << "Cedula: " << currentUser->getIdNumber() << endl;
    cout << "Evento: " << event.getEvents().getAt(selectedEvent).getName() << endl;
    cout << "Segmento: " << segments.getAt(selectedEvent).getAt(selectedSegment).getName() << endl;
    cout << "Tarjeta: ****-****-****-" << cardNumber.substr(cardNumber.length() - 4) << endl;
    cout << "Boletos: " << numTickets << " x $" << fixed << setprecision(2) << ticketPrice << endl;
    if (discountPercentage > 0) {
        cout << "Descuento: " << discountPercentage << "%\n";
    }
    cout << "Asientos comprados: ";
    for (int i = 0; i < numPurchasedSeats; i++) {
        cout << "(Fila: " << purchasedRows[i]
            << ", Columna: " << purchasedCols[i] << ") ";
    }
    cout << "\n";
    cout << "Total pagado: $" << fixed << setprecision(2) << totalCost << "\n";
    cout << "=================================================\n";
}


int Sale::readIntInRange(int minValue, int maxValue, const std::string& errorPrompt)
{
    int value;
    while (true) {
        cin >> value;

        if (!cin.fail() && value >= minValue && value <= maxValue) {

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << errorPrompt;
    }
}

void Sale::cancelPurchase(User& user, Event& event, Segment& segment,
    std::map<std::tuple<int, int>, Seating>& seatingMap)
{
    // Verificar si hay eventos disponibles
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles para cancelar compras.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Solicitar cédula
    string idNumber;
    cout << "Ingrese su numero de cedula (9 digitos) para cancelar compra: ";
    cin >> idNumber;

    // Validar cédula
    if (idNumber.size() != 9 || idNumber.find_first_not_of("0123456789") != string::npos) {
        cout << "Cedula invalida o no existe.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Buscar usuario
    UserData* currentUser = user.searchUserById(idNumber);
    if (!currentUser) {
        cout << "Usuario no encontrado.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Mostrar eventos disponibles
    cout << "\nEventos disponibles:\n";
    for (int i = 1; i <= event.getEventCount(); i++) {
        cout << i << ". " << event.getEvents().getAt(i).getName() << "\n";
    }

    cout << "\nSeleccione el numero de evento para cancelar boletos: ";
    int selectedEvent = readIntInRange(1, event.getEventCount(), "Evento invalido. Intente de nuevo: ");

    // Verificar boletos comprados en el evento
    int purchasedTickets = event.getEvents().getAt(selectedEvent).getTicketsPurchasedByUser(idNumber);
    if (purchasedTickets == 0) {
        cout << "El usuario no tiene boletos comprados para este evento.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    cout << "Usted tiene " << purchasedTickets << " boletos para este evento.\n";

    cout << "\nCuantos boletos desea cancelar? ";
    int toCancel = readIntInRange(1, purchasedTickets, "Cantidad invalida. Intente de nuevo: ");

    // Obtener segmentos asociados al evento
    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();
    int numSegments = segmentCounts.getAt(selectedEvent);

    if (numSegments == 0) {
        cout << "El evento no tiene segmentos registrados.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Mostrar segmentos disponibles
    cout << "\nSegmentos disponibles para ese evento:\n";
    for (int i = 1; i <= numSegments; i++) {
        cout << i << ". " << segments.getAt(selectedEvent).getAt(i).getName()
            << " - Precio: $" << segments.getAt(selectedEvent).getAt(i).getPrice() << "\n";
    }

    cout << "\nSeleccione el numero de segmento para liberar asientos: ";
    int chosenSegment = readIntInRange(1, numSegments, "Segmento invalido. Intente de nuevo: ");

    // Verificar si en el segmento hay asientos vendidos
    tuple<int, int> seatingKey = make_tuple(selectedEvent, chosenSegment);
    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        cout << "\nEn este segmento no hay asientos vendidos. Nada que cancelar.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    Seating& seating = seatingMap[seatingKey];

    cout << "\nIndique los asientos que desea liberar:\n";

    // Para cada boleto a cancelar, se solicita primero la columna y luego la fila.
    for (int i = 0; i < toCancel; i++) {
        char colChar = '\0';
        int row = -1;

        // Solicitar columna
        while (true) {
            cout << "Columna (A-" << char('A' + seating.getNumberOfColumns() - 1) << "): ";
            string colInput;
            cin >> colInput;
            if (colInput.size() == 1) {
                colChar = toupper(colInput[0]);
                int colIndex = colChar - 'A';
                if (colIndex >= 0 && colIndex < seating.getNumberOfColumns()) {
                    break; // Columna válida
                }
                else {
                    cout << "Columna fuera de rango.\n";
                }
            }
            else {
                cout << "Ingrese solo una letra.\n";
            }
        }

        // Solicitar fila
        while (true) {
            cout << "Fila (1-" << seating.getNumberOfRows() << "): ";
            if (cin >> row && row >= 1 && row <= seating.getNumberOfRows()) {
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Fila invalida.\n";
        }

        int colIndex = colChar - 'A';
        // Intentar liberar el asiento
        if (seating.freeSeat(row - 1, colIndex)) {
            cout << "Asiento liberado correctamente.\n";
        }
        else {
            cout << "Ese asiento no estaba comprado. Intente otro.\n";
            i--; // Repetir este ciclo si la liberación falla
        }
    }

    // Actualizar la cantidad de boletos en el evento
    if (event.getEvents().getAt(selectedEvent).cancelTickets(idNumber, toCancel)) {
        cout << "\nSe han cancelado " << toCancel << " boletos correctamente.\n";
    }
    else {
        cout << "\nNo fue posible cancelar los boletos.\n";
    }

    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int Sale::chooseEvent(Event& event, sf::RenderWindow& win) {
    if (event.getEventCount() == 0) {
        std::cout << "No hay eventos disponibles.\n";
        return -1;
    }

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return -1;
    }

    sf::Text titleText("Seleccione un evento:", font, 24);
    titleText.setFillColor(TEXT_COLOR_EV);
    titleText.setPosition(50.f, 50.f);

    std::vector<sf::Text> eventTexts;
    float eventStartY = 100.f;
    for (int i = 1; i <= event.getEventCount(); i++) {
        std::string eventName = event.getEvents().getAt(i).getName();
        sf::Text eventText(eventName, font, 24);
        eventText.setFillColor(TEXT_COLOR_EV);
        eventText.setPosition(50.f, eventStartY + (i - 1) * 40.f);
        eventTexts.push_back(eventText);
    }

    bool eventSelected = false;
    int selectedEvent = -1;

    while (!eventSelected && win.isOpen()) {
        sf::Event ev;
        while (win.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                win.close();
                return -1;
            }

            // Resaltar evento al pasar el ratón
            if (ev.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseMove.x), static_cast<float>(ev.mouseMove.y));
                for (int i = 0; i < eventTexts.size(); i++) {
                    if (eventTexts[i].getGlobalBounds().contains(mousePos))
                        eventTexts[i].setFillColor(HIGHLIGHT_COLOR_EV);
                    else
                        eventTexts[i].setFillColor(TEXT_COLOR_EV);
                }
            }

            // Seleccionar evento al hacer clic
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < eventTexts.size(); i++) {
                    if (eventTexts[i].getGlobalBounds().contains(mousePos)) {
                        selectedEvent = i + 1;
                        eventSelected = true;
                        break;
                    }
                }
            }
        }

        // Dibujar la interfaz
        win.clear(BG_COLOR_EV);
        win.draw(titleText);
        for (auto& eventText : eventTexts) {
            win.draw(eventText);
        }
        win.display();
    }

    return selectedEvent;
}
