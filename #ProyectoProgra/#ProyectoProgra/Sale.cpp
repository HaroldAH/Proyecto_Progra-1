#include "Sale.h"
#include <sstream>
#include "seating.h"
using namespace std;



std::pair<int, int> getSeatSelection(sf::RenderWindow& window, Seating& seating)
{
    // Parámetros de dibujo (deben coincidir con los usados en Seating::displaySeats)
    const float seatWidth = 30.f;
    const float seatHeight = 30.f;
    const float spacing = 10.f;
    const float marginX = 50.f;
    const float marginY = 150.f;

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        // Si no se pudo cargar la fuente, se retorna una selección inválida.
        return { -1, -1 };
    }

    // Bucle para esperar la selección
    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return { -1, -1 };
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                float x = static_cast<float>(ev.mouseButton.x);
                float y = static_cast<float>(ev.mouseButton.y);

                // Verificar que el clic esté dentro de la región de la sala
                if (x < marginX || y < marginY)
                    continue;

                // Calcular la columna y la fila a partir de las coordenadas
                int col = static_cast<int>((x - marginX) / (seatWidth + spacing));
                int row = static_cast<int>((y - marginY) / (seatHeight + spacing));

                // Calcular las coordenadas exactas del recuadro del asiento seleccionado
                float seatX = marginX + col * (seatWidth + spacing);
                float seatY = marginY + row * (seatHeight + spacing);
                if (x >= seatX && x <= seatX + seatWidth &&
                    y >= seatY && y <= seatY + seatHeight)
                {
                    // Verificar que los índices sean válidos
                    if (row < 0 || row >= seating.getNumberOfRows() || col < 0 || col >= seating.getNumberOfColumns())
                        continue;

                    // Si el asiento ya está ocupado, mostrar mensaje de error
                    if (seating.getSeatPurchased()[row][col])
                    {
                        sf::Text errorMsg("Asiento ocupado, elija otro.", font, 24);
                        errorMsg.setFillColor(sf::Color::Red);
                        errorMsg.setPosition(50.f, marginY + seating.getNumberOfRows() * (seatHeight + spacing) + 20.f);
                        window.clear(BG_COLOR_EV);
                        seating.displaySeats(window); // Redibuja la sala
                        window.draw(errorMsg);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        // Luego se continúa el bucle para reintentar
                        continue;
                    }
                    // Asiento válido seleccionado; devolver fila y columna (0-indexado)
                    return { row, col };
                }
            }
        }
        // Redibujar la sala mientras se espera la entrada
        window.clear(BG_COLOR_EV);
        seating.displaySeats(window);
        window.display();
    }
    return { -1, -1 };
}


void Sale::sell(User& user, Event& event, Segment& segment,
    std::map<std::tuple<int, int>, Seating>& seatingMap, Discount& discount,
    sf::RenderWindow& window)
{
    // 1. Verificar disponibilidad de eventos.
    if (event.getEventCount() == 0)
    {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            return;
        sf::Text msg("No hay eventos disponibles para la venta.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(msg);
        window.display();
        sf::sleep(sf::seconds(2));
        return;  // Se interrumpe el proceso de venta.
    }

    // 2. Solicitar o registrar usuario (con función SFML adaptada).
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

    // 6. Asegurar la sala y verificar disponibilidad.
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

        // Si el segmento seleccionado está lleno, buscar alternativas.
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

        // Si hay alternativas, pedir al usuario elegir otro segmento.
        selectedSegment = chooseSegment(segment, selectedEvent, window);
    }
    Seating& seating = *seatingPtr;

    // 7. Solicitar el número de boletos mediante una función gráfica (implementada en SFML).
    int numTickets = buyTickets(currentUser, event, selectedEvent, seating, window);
    if (numTickets <= 0)
        return;

    // 8. Para cada boleto, permitir que el usuario elija el asiento haciendo clic.
    int* purchasedRows = new int[numTickets];
    char* purchasedCols = new char[numTickets];
    for (int i = 0; i < numTickets; i++)
    {
        // Mostrar la sala y esperar que el usuario haga clic sobre un asiento disponible.
        std::pair<int, int> seat = getSeatSelection(window, seating);
        if (seat.first == -1 || seat.second == -1)
        {
            // Si se cierra la ventana o se devuelve una selección inválida, abortar la compra.
            delete[] purchasedRows;
            delete[] purchasedCols;
            return;
        }
        // Registrar la selección (convertir índices 0-indexados a números de fila y columna para mostrar)
        purchasedRows[i] = seat.first + 1;
        purchasedCols[i] = 'A' + seat.second;
        // Marcar el asiento como vendido
        seating.getSeatPurchased()[seat.first][seat.second] = true;

        // Mostrar mensaje temporal en la ventana
        sf::Font font;
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
        sf::Text seatMsg("Asiento reservado para boleto " + std::to_string(i + 1), font, 20);
        seatMsg.setFillColor(sf::Color::Black);
        seatMsg.setPosition(50.f, 400.f + i * 30.f);
        window.clear(BG_COLOR_EV);
        window.draw(seatMsg);
        window.display();
        sf::sleep(sf::seconds(1));
    }

    // 9. Aplicar descuento.
    float discountPercentage = applyDiscountIfWanted(discount,window);
    float ticketPrice = seating.getCost();
    float totalCost = ticketPrice * numTickets;
    if (discountPercentage > 0)
        totalCost -= totalCost * (discountPercentage / 100.0f);

    // 10. Solicitar el número de tarjeta (aquí se usa un valor fijo; idealmente implementa una función gráfica).
    string cardNumber = "123456789012";

    // 11. Mostrar la factura (aquí se sigue usando la función original, que imprime por consola).
    printInvoice(currentUser, event, selectedEvent, segments, selectedSegment,
        numTickets, ticketPrice, discountPercentage, totalCost,
        purchasedRows, purchasedCols, numTickets, cardNumber,window);

    // 12. Pausa para que el usuario vea la factura.
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

Seating& Sale::ensureSeating(int selectedEvent, int selectedSegment, List<List<Segment>>& segments, std::map<std::tuple<int, int>, Seating>& seatingMap, sf::RenderWindow& win)   // Agregado el parámetro 'win'
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
        
    }
    else {
     
        cout << " " << endl;
    }


    return seating;
}

int Sale::buyTickets(UserData* currentUser, Event& event, int selectedEvent, Seating& seating, sf::RenderWindow& window) {
    // 1. Contar los asientos disponibles en el segmento
    int availableSeats = 0;
    for (int i = 0; i < seating.getNumberOfRows(); i++) {
        for (int j = 0; j < seating.getNumberOfColumns(); j++) {
            if (!seating.getSeatPurchased()[i][j])
                availableSeats++;
        }
    }

    // 2. Obtener los boletos ya comprados por el usuario para este evento
    std::string userId = currentUser->getIdNumber();
    int currentTickets = event.getTicketsPurchasedByUser(userId);

    // 3. Calcular el máximo de boletos adicionales permitidos para este evento
    int maxTickets = 5 - currentTickets;

    // Si ya alcanzó o superó el límite de 5 boletos para este evento, mostramos mensaje y salimos
    if (maxTickets <= 0) {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Error al cargar la fuente." << std::endl;
            return 0;
        }
        sf::Text msg("Ya has comprado el numero maximo de 5 boletos para este evento.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(msg);
        window.display();
        sf::sleep(sf::seconds(3));
        return 0;
    }

    // 4. Ajustar el máximo permitido según la disponibilidad de asientos en el segmento
    if (availableSeats < maxTickets)
        maxTickets = availableSeats;

    // 5. Preparar la interfaz para solicitar la cantidad de boletos
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return 0;
    }

    sf::Text prompt("¿Cuantos boletos desea comprar? (max " + std::to_string(maxTickets) + "):", font, 24);
    prompt.setFillColor(TEXT_COLOR_EV);
    prompt.setPosition(50.f, 50.f);

    sf::RectangleShape inputBox(sf::Vector2f(200.f, 40.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50.f, 120.f);

    sf::Text inputText("", font, 24);
    inputText.setFillColor(TEXT_COLOR_EV);
    inputText.setPosition(55.f, 125.f);

    sf::Text errorText("", font, 20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50.f, 180.f);

    std::string ticketStr;
    bool done = false;

    while (!done && window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Enter) {
                    try {
                        int numTickets = std::stoi(ticketStr);
                        if (numTickets >= 1 && numTickets <= maxTickets) {
                            // Registrar la compra en el evento (actualiza el mapa ticketsByUser)
                            if (event.purchaseTickets(userId, numTickets)) {
                                return numTickets;
                            }
                            else {
                                errorText.setString("No se pudo completar la compra. Intente nuevamente.");
                                ticketStr = "";
                                inputText.setString(ticketStr);
                            }
                        }
                        else {
                            errorText.setString("Ingrese un numero entre 1 y " + std::to_string(maxTickets));
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
                    if (std::isdigit(c)) {
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

    return 0;
}


float Sale::applyDiscountIfWanted(Discount& discount, sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return 0.0f;
    }

    // 1. Preguntar si se desea aplicar descuento
    sf::Text prompt("Tiene un codigo de descuento?", font, 24);
    prompt.setFillColor(TEXT_COLOR_EV);
    prompt.setPosition(50.f, 50.f);

    sf::RectangleShape siButton(sf::Vector2f(100.f, 40.f));
    siButton.setFillColor(sf::Color::Green);
    siButton.setPosition(50.f, 120.f);
    sf::Text siText("Si", font, 24);
    siText.setFillColor(sf::Color::White);
    {
        sf::FloatRect bounds = siText.getLocalBounds();
        siText.setPosition(
            siButton.getPosition().x + (siButton.getSize().x - bounds.width) / 2.f - bounds.left,
            siButton.getPosition().y + (siButton.getSize().y - bounds.height) / 2.f - bounds.top
        );
    }

    sf::RectangleShape noButton(sf::Vector2f(100.f, 40.f));
    noButton.setFillColor(sf::Color::Red);
    noButton.setPosition(200.f, 120.f);
    sf::Text noText("No", font, 24);
    noText.setFillColor(sf::Color::White);
    {
        sf::FloatRect bounds = noText.getLocalBounds();
        noText.setPosition(
            noButton.getPosition().x + (noButton.getSize().x - bounds.width) / 2.f - bounds.left,
            noButton.getPosition().y + (noButton.getSize().y - bounds.height) / 2.f - bounds.top
        );
    }

    bool decisionMade = false;
    bool wantsDiscount = false;
    while (!decisionMade && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0.0f;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y));
                if (siButton.getGlobalBounds().contains(mousePos)) {
                    wantsDiscount = true;
                    decisionMade = true;
                }
                else if (noButton.getGlobalBounds().contains(mousePos)) {
                    wantsDiscount = false;
                    decisionMade = true;
                }
            }
        }
        window.clear(BG_COLOR_EV);
        window.draw(prompt);
        window.draw(siButton);
        window.draw(siText);
        window.draw(noButton);
        window.draw(noText);
        window.display();
    }

    if (!wantsDiscount) {
        sf::Text noDiscountMsg("No se aplico ningun descuento.", font, 24);
        noDiscountMsg.setFillColor(TEXT_COLOR_EV);
        noDiscountMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(noDiscountMsg);
        window.display();
        sf::sleep(sf::seconds(2));
        return 0.0f;
    }

    // 2. Ingreso del código de descuento
    while (true) {
        sf::Text codePrompt("Ingrese su codigo de descuento:", font, 24);
        codePrompt.setFillColor(TEXT_COLOR_EV);
        codePrompt.setPosition(50.f, 50.f);

        sf::RectangleShape inputBox(sf::Vector2f(300.f, 40.f));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(1.f);
        inputBox.setPosition(50.f, 120.f);

        sf::Text codeText("", font, 24);
        codeText.setFillColor(TEXT_COLOR_EV);
        codeText.setPosition(55.f, 125.f);

        // Botón "Aplicar"
        sf::RectangleShape applyButton(sf::Vector2f(120.f, 40.f));
        applyButton.setFillColor(sf::Color::Blue);
        applyButton.setPosition(50.f, 200.f);
        sf::Text applyText("Aplicar", font, 24);
        applyText.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = applyText.getLocalBounds();
            applyText.setPosition(
                applyButton.getPosition().x + (applyButton.getSize().x - bounds.width) / 2.f - bounds.left,
                applyButton.getPosition().y + (applyButton.getSize().y - bounds.height) / 2.f - bounds.top
            );
        }

        std::string discountCode;
        bool codeEntered = false;
        while (!codeEntered && window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) {
                    window.close();
                    return 0.0f;
                }
                if (ev.type == sf::Event::TextEntered) {
                    if (ev.text.unicode < 128) {
                        char c = static_cast<char>(ev.text.unicode);
                        if (isalnum(c)) {
                            discountCode.push_back(c);
                            codeText.setString(discountCode);
                        }
                    }
                }
                if (ev.type == sf::Event::KeyPressed) {
                    if (ev.key.code == sf::Keyboard::Backspace) {
                        if (!discountCode.empty()) {
                            discountCode.pop_back();
                            codeText.setString(discountCode);
                        }
                    }
                    if (ev.key.code == sf::Keyboard::Enter) {
                        codeEntered = true;
                    }
                }
                if (ev.type == sf::Event::MouseButtonPressed &&
                    ev.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);
                    if (applyButton.getGlobalBounds().contains(mousePos)) {
                        codeEntered = true;
                    }
                }
            }
            window.clear(BG_COLOR_EV);
            window.draw(codePrompt);
            window.draw(inputBox);
            window.draw(codeText);
            window.draw(applyButton);
            window.draw(applyText);
            window.display();
        }

        // 3. Verificar el código ingresado
        if (discount.verifyCode(discountCode)) {
            float discountPercentage = discount.getDiscountPercentage();
            sf::Text successMsg("Descuento aplicado: " + std::to_string(discountPercentage) + "%", font, 24);
            successMsg.setFillColor(TEXT_COLOR_EV);
            successMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(successMsg);
            window.display();
            sf::sleep(sf::seconds(2));
            return discountPercentage;
        }
        else {
            sf::Text errorMsg("Codigo de descuento invalido o ya utilizado.", font, 24);
            errorMsg.setFillColor(sf::Color::Red);
            errorMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(errorMsg);
            window.display();
            sf::sleep(sf::seconds(2));
        }

        // 4. Preguntar si se desea reintentar
        sf::Text retryPrompt("Desea intentarlo nuevamente?", font, 24);
        retryPrompt.setFillColor(TEXT_COLOR_EV);
        retryPrompt.setPosition(50.f, 100.f);

        sf::RectangleShape retrySiButton(sf::Vector2f(100.f, 40.f));
        retrySiButton.setFillColor(sf::Color::Green);
        retrySiButton.setPosition(50.f, 160.f);
        sf::Text retrySiText("Si", font, 24);
        retrySiText.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = retrySiText.getLocalBounds();
            retrySiText.setPosition(
                retrySiButton.getPosition().x + (retrySiButton.getSize().x - bounds.width) / 2.f - bounds.left,
                retrySiButton.getPosition().y + (retrySiButton.getSize().y - bounds.height) / 2.f - bounds.top
            );
        }

        sf::RectangleShape retryNoButton(sf::Vector2f(100.f, 40.f));
        retryNoButton.setFillColor(sf::Color::Red);
        retryNoButton.setPosition(200.f, 160.f);
        sf::Text retryNoText("No", font, 24);
        retryNoText.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = retryNoText.getLocalBounds();
            retryNoText.setPosition(
                retryNoButton.getPosition().x + (retryNoButton.getSize().x - bounds.width) / 2.f - bounds.left,
                retryNoButton.getPosition().y + (retryNoButton.getSize().y - bounds.height) / 2.f - bounds.top
            );
        }

        bool decisionMade = false;
        bool tryAgain = false;
        while (!decisionMade && window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) {
                    window.close();
                    return 0.0f;
                }
                if (ev.type == sf::Event::MouseButtonPressed &&
                    ev.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);
                    if (retrySiButton.getGlobalBounds().contains(mousePos)) {
                        tryAgain = true;
                        decisionMade = true;
                    }
                    else if (retryNoButton.getGlobalBounds().contains(mousePos)) {
                        tryAgain = false;
                        decisionMade = true;
                    }
                }
            }
            window.clear(BG_COLOR_EV);
            window.draw(retryPrompt);
            window.draw(retrySiButton);
            window.draw(retrySiText);
            window.draw(retryNoButton);
            window.draw(retryNoText);
            window.display();
        }
        if (!tryAgain)
            break;
    }

    sf::Text noDiscountMsg("No se aplico ningun descuento.", font, 24);
    noDiscountMsg.setFillColor(TEXT_COLOR_EV);
    noDiscountMsg.setPosition(50.f, 50.f);
    window.clear(BG_COLOR_EV);
    window.draw(noDiscountMsg);
    window.display();
    sf::sleep(sf::seconds(2));
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

void Sale::printInvoice(UserData* currentUser, Event& event, int selectedEvent,List<List<Segment>>& segments, int selectedSegment,
    int numTickets, float ticketPrice, float discountPercentage, float totalCost, int* purchasedRows, char* purchasedCols, int numPurchasedSeats, std::string cardNumber,sf::RenderWindow& window)
{
    // 1. Preparar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error al cargar la fuente para la factura." << std::endl;
        return;
    }

    // 2. Crear un vector de objetos sf::Text para cada línea de la factura.
    std::vector<sf::Text> invoiceLines;
    float startY = 20.f;           // Posición vertical inicial
    const float lineSpacing = 30.f; // Espaciado vertical entre líneas

    // Encabezado de la factura
    sf::Text header("==================== FACTURA ====================", font, 24);
    header.setFillColor(TEXT_COLOR_EV);
    header.setPosition(20.f, startY);
    invoiceLines.push_back(header);
    startY += lineSpacing;

    // Datos del usuario
    sf::Text userLine("Usuario: " + currentUser->getName(), font, 20);
    userLine.setFillColor(TEXT_COLOR_EV);
    userLine.setPosition(20.f, startY);
    invoiceLines.push_back(userLine);
    startY += lineSpacing;

    sf::Text cedulaLine("Cedula: " + currentUser->getIdNumber(), font, 20);
    cedulaLine.setFillColor(TEXT_COLOR_EV);
    cedulaLine.setPosition(20.f, startY);
    invoiceLines.push_back(cedulaLine);
    startY += lineSpacing;

    // Datos del evento
    sf::Text eventLine("Evento: " + event.getEvents().getAt(selectedEvent).getName(), font, 20);
    eventLine.setFillColor(TEXT_COLOR_EV);
    eventLine.setPosition(20.f, startY);
    invoiceLines.push_back(eventLine);
    startY += lineSpacing;

    sf::Text segmentLine("Segmento: " + segments.getAt(selectedEvent).getAt(selectedSegment).getName(), font, 20);
    segmentLine.setFillColor(TEXT_COLOR_EV);
    segmentLine.setPosition(20.f, startY);
    invoiceLines.push_back(segmentLine);
    startY += lineSpacing;

    // Información de la tarjeta (mostrar solo los 4 últimos dígitos)
    std::string cardMasked = "****-****-****-" + cardNumber.substr(cardNumber.length() - 4);
    sf::Text cardLine("Tarjeta: " + cardMasked, font, 20);
    cardLine.setFillColor(TEXT_COLOR_EV);
    cardLine.setPosition(20.f, startY);
    invoiceLines.push_back(cardLine);
    startY += lineSpacing;

    // Información de los boletos
    std::ostringstream oss;
    oss << "Boletos: " << numTickets << " x $" << std::fixed << std::setprecision(2) << ticketPrice;
    sf::Text ticketsLine(oss.str(), font, 20);
    ticketsLine.setFillColor(TEXT_COLOR_EV);
    ticketsLine.setPosition(20.f, startY);
    invoiceLines.push_back(ticketsLine);
    startY += lineSpacing;

    // Descuento (si se aplicó)
    if (discountPercentage > 0)
    {
        sf::Text discountLine("Descuento: " + std::to_string(discountPercentage) + "%", font, 20);
        discountLine.setFillColor(TEXT_COLOR_EV);
        discountLine.setPosition(20.f, startY);
        invoiceLines.push_back(discountLine);
        startY += lineSpacing;
    }

    // Asientos comprados
    std::string seatsStr = "Asientos comprados: ";
    for (int i = 0; i < numPurchasedSeats; i++)
    {
        seatsStr += "(Fila: " + std::to_string(purchasedRows[i]) + ", Columna: " + purchasedCols[i] + ") ";
    }
    sf::Text seatsLine(seatsStr, font, 20);
    seatsLine.setFillColor(TEXT_COLOR_EV);
    seatsLine.setPosition(20.f, startY);
    invoiceLines.push_back(seatsLine);
    startY += lineSpacing;

    // Total pagado
    std::ostringstream oss2;
    oss2 << "Total pagado: $" << std::fixed << std::setprecision(2) << totalCost;
    sf::Text totalLine(oss2.str(), font, 20);
    totalLine.setFillColor(TEXT_COLOR_EV);
    totalLine.setPosition(20.f, startY);
    invoiceLines.push_back(totalLine);
    startY += lineSpacing;

    // Pie de página
    sf::Text footer("=================================================", font, 24);
    footer.setFillColor(TEXT_COLOR_EV);
    footer.setPosition(20.f, startY);
    invoiceLines.push_back(footer);

    // 3. Dibujar la factura en la ventana.
    window.clear(BG_COLOR_EV);
    for (const auto& line : invoiceLines)
    {
        window.draw(line);
    }

    // 4. Crear y dibujar un botón verde "Avanzar" para finalizar la visualización.
    sf::RectangleShape avanzarButton(sf::Vector2f(120.f, 40.f));
    avanzarButton.setFillColor(sf::Color::Green);
    avanzarButton.setPosition(window.getSize().x - 150.f, window.getSize().y - 70.f);

    sf::Text avanzarText("Avanzar", font, 24);
    avanzarText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = avanzarText.getLocalBounds();
    avanzarText.setPosition(
        avanzarButton.getPosition().x + (avanzarButton.getSize().x - textBounds.width) / 2.f - textBounds.left,
        avanzarButton.getPosition().y + (avanzarButton.getSize().y - textBounds.height) / 2.f - textBounds.top
    );

    window.draw(avanzarButton);
    window.draw(avanzarText);
    window.display();

    // 5. Esperar que el usuario presione ENTER para volver al menú principal.
    bool proceed = false;
    while (window.isOpen() && !proceed)
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter)
            {
                proceed = true;
                break;
            }
        }
    }

    // Al presionar ENTER, se limpia la ventana (lo que en el flujo de tu aplicación debería retornar al menú principal)
    window.clear(BG_COLOR_EV);
    window.display();
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
    std::map<std::tuple<int, int>, Seating>& seatingMap, sf::RenderWindow& window)
{
    // 1. Verificar si hay eventos disponibles.
    if (event.getEventCount() == 0) {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            return;
        sf::Text msg("No hay eventos disponibles para cancelar compras.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(msg);
        window.display();
        sf::sleep(sf::seconds(2));
        return;
    }

    // 2. Solicitar cédula mediante SFML.
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        return;
    sf::Text prompt("Ingrese su numero de cedula (9 digitos):", font, 24);
    prompt.setFillColor(TEXT_COLOR_EV);
    prompt.setPosition(50.f, 50.f);
    sf::RectangleShape inputBox(sf::Vector2f(300.f, 40.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50.f, 120.f);
    sf::Text inputText("", font, 24);
    inputText.setFillColor(TEXT_COLOR_EV);
    inputText.setPosition(55.f, 125.f);

    std::string idNumber;
    bool inputDone = false;
    while (!inputDone && window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (ev.type == sf::Event::TextEntered) {
                if (ev.text.unicode < 128) {
                    char c = static_cast<char>(ev.text.unicode);
                    if (isdigit(c) && idNumber.size() < 9) {
                        idNumber.push_back(c);
                        inputText.setString(idNumber);
                    }
                }
            }
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Backspace) {
                    if (!idNumber.empty()) {
                        idNumber.pop_back();
                        inputText.setString(idNumber);
                    }
                }
                if (ev.key.code == sf::Keyboard::Enter) {
                    if (idNumber.size() == 9)
                        inputDone = true;
                    else {
                        sf::Text errorMsg("Cedula invalida. Debe tener 9 digitos.", font, 24);
                        errorMsg.setFillColor(sf::Color::Red);
                        errorMsg.setPosition(50.f, 180.f);
                        window.clear(BG_COLOR_EV);
                        window.draw(prompt);
                        window.draw(inputBox);
                        window.draw(inputText);
                        window.draw(errorMsg);
                        window.display();
                        sf::sleep(sf::seconds(2));
                    }
                }
            }
        }
        window.clear(BG_COLOR_EV);
        window.draw(prompt);
        window.draw(inputBox);
        window.draw(inputText);
        window.display();
    }

    // 3. Buscar usuario por cédula.
    UserData* currentUser = user.searchUserById(idNumber);
    if (!currentUser) {
        sf::Text err("Usuario no encontrado.", font, 24);
        err.setFillColor(TEXT_COLOR_EV);
        err.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(err);
        window.display();
        bool wait = true;
        while (window.isOpen() && wait) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter)
                    wait = false;
                if (ev.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
            }
        }
        return;
    }

    // 4. Mostrar eventos disponibles.
    std::vector<sf::Text> eventOptions;
    for (int i = 1; i <= event.getEventCount(); i++) {
        std::string evName = event.getEvents().getAt(i).getName();
        sf::Text opt(to_string(i) + ". " + evName, font, 24);
        opt.setFillColor(TEXT_COLOR_EV);
        opt.setPosition(50.f, 100.f + (i - 1) * 40.f);
        eventOptions.push_back(opt);
    }
    int selectedEvent = -1;
    bool eventSelected = false;
    while (window.isOpen() && !eventSelected) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) { window.close(); return; }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < eventOptions.size(); i++) {
                    if (eventOptions[i].getGlobalBounds().contains(mousePos)) {
                        selectedEvent = i + 1;
                        eventSelected = true;
                        break;
                    }
                }
            }
        }
        window.clear(BG_COLOR_EV);
        sf::Text title("Eventos disponibles:", font, 28);
        title.setFillColor(TEXT_COLOR_EV);
        title.setPosition(50.f, 50.f);
        window.draw(title);
        for (auto& opt : eventOptions)
            window.draw(opt);
        window.display();
    }

    // 5. Verificar boletos comprados en el evento seleccionado.
    int purchasedTickets = event.getEvents().getAt(selectedEvent).getTicketsPurchasedByUser(idNumber);
    if (purchasedTickets == 0) {
        sf::Text msg("El usuario no tiene boletos comprados para este evento.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(msg);
        window.display();
        sf::sleep(sf::seconds(2));
        return;
    }
    sf::Text ticketInfo("Usted tiene " + to_string(purchasedTickets) + " boletos para este evento.", font, 24);
    ticketInfo.setFillColor(TEXT_COLOR_EV);
    ticketInfo.setPosition(50.f, 50.f);
    window.clear(BG_COLOR_EV);
    window.draw(ticketInfo);
    window.display();
    sf::sleep(sf::seconds(2));

    // 6. Solicitar el número de boletos a cancelar.
    sf::Text cancelPrompt("Cuantos boletos desea cancelar?", font, 24);
    cancelPrompt.setFillColor(TEXT_COLOR_EV);
    cancelPrompt.setPosition(50.f, 50.f);
    sf::RectangleShape cancelInputBox(sf::Vector2f(200.f, 40.f));
    cancelInputBox.setFillColor(sf::Color::White);
    cancelInputBox.setOutlineColor(sf::Color::Black);
    cancelInputBox.setOutlineThickness(1.f);
    cancelInputBox.setPosition(50.f, 120.f);
    sf::Text cancelInputText("", font, 24);
    cancelInputText.setFillColor(TEXT_COLOR_EV);
    cancelInputText.setPosition(55.f, 125.f);
    std::string cancelStr;
    bool cancelDone = false;
    while (!cancelDone && window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) { window.close(); return; }
            if (ev.type == sf::Event::TextEntered) {
                if (ev.text.unicode < 128) {
                    char c = static_cast<char>(ev.text.unicode);
                    if (isdigit(c))
                        cancelStr.push_back(c);
                    cancelInputText.setString(cancelStr);
                }
            }
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Backspace) {
                    if (!cancelStr.empty()) {
                        cancelStr.pop_back();
                        cancelInputText.setString(cancelStr);
                    }
                }
                if (ev.key.code == sf::Keyboard::Enter) {
                    try {
                        int num = stoi(cancelStr);
                        if (num >= 1 && num <= purchasedTickets)
                            cancelDone = true;
                        else {
                            sf::Text err("Ingrese un numero entre 1 y " + to_string(purchasedTickets), font, 24);
                            err.setFillColor(sf::Color::Red);
                            err.setPosition(50.f, 180.f);
                            window.clear(BG_COLOR_EV);
                            window.draw(cancelPrompt);
                            window.draw(cancelInputBox);
                            window.draw(cancelInputText);
                            window.draw(err);
                            window.display();
                            sf::sleep(sf::seconds(2));
                            cancelStr = "";
                            cancelInputText.setString("");
                        }
                    }
                    catch (...) {
                        sf::Text err("Entrada invalida. Ingrese un numero.", font, 24);
                        err.setFillColor(sf::Color::Red);
                        err.setPosition(50.f, 180.f);
                        window.clear(BG_COLOR_EV);
                        window.draw(cancelPrompt);
                        window.draw(cancelInputBox);
                        window.draw(cancelInputText);
                        window.draw(err);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        cancelStr = "";
                        cancelInputText.setString("");
                    }
                }
            }
        }
        window.clear(BG_COLOR_EV);
        window.draw(cancelPrompt);
        window.draw(cancelInputBox);
        window.draw(cancelInputText);
        window.display();
    }
    int toCancel = stoi(cancelStr);

    // 7. Mostrar segmentos disponibles para el evento seleccionado.
    std::vector<sf::Text> segmentOptions;
    int numSegs = segment.getSegmentCount().getAt(selectedEvent);
    for (int i = 1; i <= numSegs; i++) {
        std::ostringstream oss;
        oss << i << ". " << segment.getSegmentsByEvent().getAt(selectedEvent).getAt(i).getName()
            << " - Precio: $" << segment.getSegmentsByEvent().getAt(selectedEvent).getAt(i).getPrice();
        sf::Text opt(oss.str(), font, 24);
        opt.setFillColor(TEXT_COLOR_EV);
        opt.setPosition(50.f, 100.f + (i - 1) * 40.f);
        segmentOptions.push_back(opt);
    }
    int chosenSegment = -1;
    bool segmentChosen = false;
    while (window.isOpen() && !segmentChosen) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) { window.close(); return; }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < segmentOptions.size(); i++) {
                    if (segmentOptions[i].getGlobalBounds().contains(mousePos)) {
                        chosenSegment = i + 1;
                        segmentChosen = true;
                        break;
                    }
                }
            }
        }
        window.clear(BG_COLOR_EV);
        sf::Text segTitle("Segmentos disponibles:", font, 28);
        segTitle.setFillColor(TEXT_COLOR_EV);
        segTitle.setPosition(50.f, 50.f);
        window.draw(segTitle);
        for (auto& opt : segmentOptions)
            window.draw(opt);
        window.display();
    }

    // 8. Verificar si en el segmento seleccionado hay asientos vendidos.
    auto seatingKey = std::make_tuple(selectedEvent, chosenSegment);
    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        sf::Text noSales("No se han vendido asientos para este segmento.", font, 24);
        noSales.setFillColor(TEXT_COLOR_EV);
        noSales.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(noSales);
        window.display();
        sf::sleep(sf::seconds(2));

        // Mostrar vista de la sala vacía.
        int rows = segment.getSegmentsByEvent().getAt(selectedEvent).getAt(chosenSegment).getRows();
        int cols = segment.getSegmentsByEvent().getAt(selectedEvent).getAt(chosenSegment).getSeats();
        float price = segment.getSegmentsByEvent().getAt(selectedEvent).getAt(chosenSegment).getPrice();
        Seating tempSeating;
        tempSeating.setNumberOfRows(rows);
        tempSeating.setNumberOfColumns(cols);
        tempSeating.setCost(price);
        tempSeating.initializeRoom();
        sf::Text emptyTitle("Vista de la sala vacia (sin ventas):", font, 28);
        emptyTitle.setFillColor(TEXT_COLOR_EV);
        emptyTitle.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(emptyTitle);
        tempSeating.displaySeats(window);
        sf::Text continueText("Presione ENTER para continuar...", font, 24);
        continueText.setFillColor(TEXT_COLOR_EV);
        continueText.setPosition(50.f, window.getSize().y - 50.f);
        window.draw(continueText);
        window.display();
        bool cont = false;
        while (window.isOpen() && !cont) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
                    cont = true;
                    break;
                }
                if (ev.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
            }
        }
    }
    else {
        // Usamos un puntero para referirnos a la sala del segmento seleccionado.
        Seating* pSeating = &seatingMap[seatingKey];
        sf::Text segDisplay(" ", font, 28);
        segDisplay.setFillColor(TEXT_COLOR_EV);
        segDisplay.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(segDisplay);
        pSeating->displaySeats(window);
        sf::Text continueText("Presione ENTER para continuar...", font, 24);
        continueText.setFillColor(TEXT_COLOR_EV);
        continueText.setPosition(50.f, window.getSize().y - 50.f);
        window.draw(continueText);
        window.display();
        bool cont = false;
        while (window.isOpen() && !cont) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
                    cont = true;
                    break;
                }
                if (ev.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
            }
        }
    }

    // 9. Para cada boleto a cancelar, solicitar que el usuario ingrese la columna y la fila.
    // Usaremos el puntero pSeating para operar sobre la sala.
    Seating* pSeating = nullptr;
    if (seatingMap.find(seatingKey) != seatingMap.end())
        pSeating = &seatingMap[seatingKey];
    else {
        sf::Text errMsg("Error: No se pudo obtener la sala.", font, 24);
        errMsg.setFillColor(sf::Color::Red);
        errMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(errMsg);
        window.display();
        sf::sleep(sf::seconds(2));
        return;
    }
    for (int i = 0; i < toCancel; i++) {
        // Solicitar columna:
        sf::Text colPrompt("Ingrese la columna a liberar (A-" +
            std::string(1, 'A' + pSeating->getNumberOfColumns() - 1) + "):", font, 24);
        colPrompt.setFillColor(TEXT_COLOR_EV);
        colPrompt.setPosition(50.f, 50.f);
        sf::RectangleShape colInputBox(sf::Vector2f(100.f, 40.f));
        colInputBox.setFillColor(sf::Color::White);
        colInputBox.setOutlineColor(sf::Color::Black);
        colInputBox.setOutlineThickness(1.f);
        colInputBox.setPosition(50.f, 120.f);
        sf::Text colInputText("", font, 24);
        colInputText.setFillColor(TEXT_COLOR_EV);
        colInputText.setPosition(55.f, 125.f);
        std::string colStr;
        bool colDone = false;
        while (!colDone && window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) { window.close(); return; }
                if (ev.type == sf::Event::TextEntered) {
                    if (ev.text.unicode < 128) {
                        char c = static_cast<char>(ev.text.unicode);
                        if (isalpha(c)) {
                            colStr.push_back(toupper(c));
                            colInputText.setString(colStr);
                        }
                    }
                }
                if (ev.type == sf::Event::KeyPressed) {
                    if (ev.key.code == sf::Keyboard::Enter)
                        colDone = true;
                    else if (ev.key.code == sf::Keyboard::Backspace) {
                        if (!colStr.empty()) {
                            colStr.pop_back();
                            colInputText.setString(colStr);
                        }
                    }
                }
            }
            window.clear(BG_COLOR_EV);
            window.draw(colPrompt);
            window.draw(colInputBox);
            window.draw(colInputText);
            window.display();
        }
        char colChar = colStr.empty() ? ' ' : colStr[0];

        // Solicitar fila:
        sf::Text rowPrompt("Ingrese la fila a liberar (1-" + to_string(pSeating->getNumberOfRows()) + "):", font, 24);
        rowPrompt.setFillColor(TEXT_COLOR_EV);
        rowPrompt.setPosition(50.f, 50.f);
        sf::RectangleShape rowInputBox(sf::Vector2f(100.f, 40.f));
        rowInputBox.setFillColor(sf::Color::White);
        rowInputBox.setOutlineColor(sf::Color::Black);
        rowInputBox.setOutlineThickness(1.f);
        rowInputBox.setPosition(50.f, 120.f);
        sf::Text rowInputText("", font, 24);
        rowInputText.setFillColor(TEXT_COLOR_EV);
        rowInputText.setPosition(55.f, 125.f);
        std::string rowStr;
        bool rowDone = false;
        while (!rowDone && window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) { window.close(); return; }
                if (ev.type == sf::Event::TextEntered) {
                    if (ev.text.unicode < 128) {
                        char c = static_cast<char>(ev.text.unicode);
                        if (isdigit(c)) {
                            rowStr.push_back(c);
                            rowInputText.setString(rowStr);
                        }
                    }
                }
                if (ev.type == sf::Event::KeyPressed) {
                    if (ev.key.code == sf::Keyboard::Enter)
                        rowDone = true;
                    else if (ev.key.code == sf::Keyboard::Backspace) {
                        if (!rowStr.empty()) {
                            rowStr.pop_back();
                            rowInputText.setString(rowStr);
                        }
                    }
                }
            }
            window.clear(BG_COLOR_EV);
            window.draw(rowPrompt);
            window.draw(rowInputBox);
            window.draw(rowInputText);
            window.display();
        }
        int rowVal = rowStr.empty() ? 0 : stoi(rowStr);
        int colIndex = colChar - 'A';
        // Intentar liberar el asiento usando el puntero pSeating.
        if (pSeating->freeSeat(rowVal - 1, colIndex)) {
            sf::Text freedMsg("Asiento liberado correctamente.", font, 24);
            freedMsg.setFillColor(TEXT_COLOR_EV);
            freedMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(freedMsg);
            window.display();
            sf::sleep(sf::seconds(1));
        }
        else {
            sf::Text errMsg("Ese asiento no estaba comprado. Intente otro.", font, 24);
            errMsg.setFillColor(sf::Color::Red);
            errMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(errMsg);
            window.display();
            sf::sleep(sf::seconds(2));
            i--; // Repetir este ciclo si la liberación falla.
        }
    }

    // 10. Actualizar la cantidad de boletos en el evento.
    if (event.getEvents().getAt(selectedEvent).cancelTickets(idNumber, toCancel, window)) {
        sf::Text successMsg("Se han cancelado los boletos correctamente.", font, 24);
        successMsg.setFillColor(TEXT_COLOR_EV);
        successMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(successMsg);
        window.display();
        sf::sleep(sf::seconds(2));
    }
    else {
        sf::Text errorMsg("No fue posible cancelar los boletos.", font, 24);
        errorMsg.setFillColor(sf::Color::Red);
        errorMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(errorMsg);
        window.display();
        sf::sleep(sf::seconds(2));
    }

    // 11. Mostrar mensaje final y esperar que el usuario presione ENTER para volver al menú.
    sf::Text finalMsg("Presione ENTER para continuar...", font, 24);
    finalMsg.setFillColor(TEXT_COLOR_EV);
    finalMsg.setPosition(50.f, window.getSize().y - 50.f);
    window.clear(BG_COLOR_EV);
    window.draw(finalMsg);
    window.display();

    bool proceed = false;
    while (window.isOpen() && !proceed) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
                proceed = true;
                break;
            }
            if (ev.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }
    }
}

int Sale::chooseEvent(Event& event, sf::RenderWindow& window) {
    // Verificar si hay eventos disponibles
    if (event.getEventCount() == 0) {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            return -1;
        sf::Text msg("No hay eventos disponibles.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(msg);
        window.display();
        sf::sleep(sf::seconds(2));
        return -1;
    }

    // Cargar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return -1;
    }

    // Crear título de la pantalla
    sf::Text title("Seleccione un evento:", font, 24);
    title.setFillColor(TEXT_COLOR_EV);
    title.setPosition(50.f, 20.f);

    // Crear un vector con las opciones (una por cada evento)
    std::vector<sf::Text> options;
    float startY = 60.f;
    float spacing = 40.f;
    for (int i = 1; i <= event.getEventCount(); i++) {
        std::ostringstream oss;
        oss << i << ". " << event.getEvents().getAt(i).getName();
        sf::Text option(oss.str(), font, 24);
        option.setFillColor(TEXT_COLOR_EV);
        option.setPosition(50.f, startY + (i - 1) * spacing);
        options.push_back(option);
    }

    // Bucle para esperar la selección del usuario
    int selected = -1;
    bool selectionMade = false;
    while (window.isOpen() && !selectionMade) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return -1;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                for (size_t i = 0; i < options.size(); i++) {
                    if (options[i].getGlobalBounds().contains(mousePos)) {
                        selected = static_cast<int>(i) + 1;
                        selectionMade = true;
                        break;
                    }
                }
            }
        }

        // Renderizar la pantalla
        window.clear(BG_COLOR_EV);
        window.draw(title);
        for (const auto& opt : options)
            window.draw(opt);
        window.display();
    }
    return selected;
}

pair<int, int> getSoldSeatSelection(sf::RenderWindow& window, Seating& seating)
{
    // Parámetros de dibujo (deben coincidir con los usados en Seating::displaySeats)
    const float seatWidth = 30.f;
    const float seatHeight = 30.f;
    const float spacing = 10.f;
    const float marginX = 50.f;
    const float marginY = 100.f;

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        // Si no se pudo cargar la fuente, se retorna una selección inválida.
        return { -1, -1 };
    }

    // Bucle para esperar la selección
    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return { -1, -1 };
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                float x = static_cast<float>(ev.mouseButton.x);
                float y = static_cast<float>(ev.mouseButton.y);

                // Verificar que el clic esté dentro de la región de la sala
                if (x < marginX || y < marginY)
                    continue;

                // Calcular la columna y la fila a partir de las coordenadas
                int col = static_cast<int>((x - marginX) / (seatWidth + spacing));
                int row = static_cast<int>((y - marginY) / (seatHeight + spacing));

                // Calcular las coordenadas exactas del recuadro del asiento seleccionado
                float seatX = marginX + col * (seatWidth + spacing);
                float seatY = marginY + row * (seatHeight + spacing);
                if (x >= seatX && x <= seatX + seatWidth &&
                    y >= seatY && y <= seatY + seatHeight)
                {
                    // Verificar que los índices sean válidos
                    if (row < 0 || row >= seating.getNumberOfRows() || col < 0 || col >= seating.getNumberOfColumns())
                        continue;

                    // Para cancelar, el asiento debe estar ocupado
                    if (!seating.getSeatPurchased()[row][col])
                    {
                        sf::Text errorMsg("Asiento no vendido, elija otro.", font, 24);
                        errorMsg.setFillColor(sf::Color::Red);
                        errorMsg.setPosition(50.f, marginY + seating.getNumberOfRows() * (seatHeight + spacing) + 20.f);
                        window.clear(BG_COLOR_EV);
                        seating.displaySeats(window); // Redibuja la sala
                        window.draw(errorMsg);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        continue;
                    }
                    // Asiento válido seleccionado; devolver fila y columna (0-indexado)
                    return { row, col };
                }
            }
        }
        // Redibujar la sala mientras se espera la entrada
        window.clear(BG_COLOR_EV);
        seating.displaySeats(window);
        window.display();
    }
    return { -1, -1 };
}

void cancelSelectedSeats(sf::RenderWindow& window, Seating* pSeating, int toCancel, int* purchasedRows, char* purchasedCols, sf::Font& font)
{
    // Se asume que "pSeating" apunta a la sala del segmento seleccionado.
    for (int i = 0; i < toCancel; i++)
    {
        // Mostrar la sala y esperar que el usuario haga clic sobre un asiento vendido.
        std::pair<int, int> seat = getSoldSeatSelection(window, *pSeating);
        if (seat.first == -1 || seat.second == -1)
        {
            // Si se cierra la ventana o se devuelve una selección inválida, abortamos.
            delete[] purchasedRows;
            delete[] purchasedCols;
            return;
        }

        // Intentar liberar el asiento. La función freeSeat() cambia el estado del asiento a libre y
        // retorna true si la operación fue exitosa.
        if (pSeating->freeSeat(seat.first, seat.second))
        {
            sf::Text freedMsg("Asiento liberado correctamente para boleto " + std::to_string(i + 1), font, 24);
            freedMsg.setFillColor(TEXT_COLOR_EV);
            freedMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(freedMsg);
            window.display();
            sf::sleep(sf::seconds(1));
        }
        else
        {
            sf::Text errMsg("Error al liberar asiento. Intente de nuevo.", font, 24);
            errMsg.setFillColor(sf::Color::Red);
            errMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(errMsg);
            window.display();
            sf::sleep(sf::seconds(2));
            i--; // Repetir esta iteración si ocurre un error.
        }
    }
}