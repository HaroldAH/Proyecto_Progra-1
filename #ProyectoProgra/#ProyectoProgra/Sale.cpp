#include "Sale.h"
#include <sstream>
#include "seating.h"
using namespace std;
#include <iostream>


std::pair<int, int> getSeatSelection(sf::RenderWindow& window, Seating& seating)
{
    
    const float seatWidth = 30.f;
    const float seatHeight = 30.f;
    const float spacing = 10.f;
    const float marginX = 50.f;
    const float marginY = 150.f;

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        // Si no se pudo cargar la fuente, se retorna una selecci�n inv�lida.
        return { -1, -1 };
    }

    // Bucle para esperar la selecci�n
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

             
                if (x < marginX || y < marginY)
                    continue;

                
                int col = static_cast<int>((x - marginX) / (seatWidth + spacing));
                int row = static_cast<int>((y - marginY) / (seatHeight + spacing));

            
                float seatX = marginX + col * (seatWidth + spacing);
                float seatY = marginY + row * (seatHeight + spacing);
                if (x >= seatX && x <= seatX + seatWidth &&
                    y >= seatY && y <= seatY + seatHeight)
                {
                    // Verificar que los �ndices sean v�lidos
                    if (row < 0 || row >= seating.getNumberOfRows() || col < 0 || col >= seating.getNumberOfColumns())
                        continue;

                    // Si el asiento ya est� ocupado, mostrar mensaje de error
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
                        
                        continue;
                    }
                 
                    return { row, col };
                }
            }
        }
       
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

    // 2. Solicitar o registrar usuario (con funci�n SFML adaptada).
    UserData* currentUser = getOrRegisterUser(user, window);
    if (!currentUser)
        return;

    // 3. Seleccionar evento (versi�n SFML).
    int selectedEvent = chooseEvent(event, window);
    if (selectedEvent < 0)
        return;

    // 4. Seleccionar segmento (versi�n SFML).
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

        // Si el segmento seleccionado est� lleno, buscar alternativas.
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
            sf::Text fullMsg("El segmento seleccionado est� lleno. No se pueden comprar m�s boletos.", font, 24);
            fullMsg.setFillColor(TEXT_COLOR_EV);
            fullMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(fullMsg);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

       
        selectedSegment = chooseSegment(segment, selectedEvent, window);
    }
    Seating& seating = *seatingPtr;

 
    int numTickets = buyTickets(currentUser, event, selectedEvent, seating, window);
    if (numTickets <= 0)
        return;

    // 8. Para cada boleto, permitir que el usuario elija el asiento haciendo clic.
    int* purchasedRows = new int[numTickets];
    char* purchasedCols = new char[numTickets];
    for (int i = 0; i < numTickets; i++)
    {
        
        std::pair<int, int> seat = getSeatSelection(window, seating);
        if (seat.first == -1 || seat.second == -1)
        {
            // Si se cierra la ventana o se devuelve una selecci�n inv�lida, abortar la compra.
            delete[] purchasedRows;
            delete[] purchasedCols;
            return;
        }
        
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

    // 10. Solicitar el n�mero de tarjeta (aqu� se usa un valor fijo; idealmente implementa una funci�n gr�fica).
    string cardNumber = askCardNumber(window);

    // 11. Mostrar la factura (aqu� se sigue usando la funci�n original, que imprime por consola).
    printInvoice(currentUser, event, selectedEvent, segments, selectedSegment,
        numTickets, ticketPrice, discountPercentage, totalCost,
        purchasedRows, purchasedCols, numTickets, cardNumber,window);

    // 12. Pausa para que el usuario vea la factura.
    sf::sleep(sf::seconds(1));

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

        // Fondo
        sf::RectangleShape background(sf::Vector2f(win.getSize().x, win.getSize().y));
        background.setFillColor(sf::Color(50, 50, 50));  // BG_COLOR

        // Encabezado
        sf::RectangleShape header(sf::Vector2f(win.getSize().x, 80.f));
        header.setFillColor(sf::Color(70, 70, 70));  // HEADER_COLOR
        header.setPosition(0.f, 0.f);

        sf::Text title("Registro de Usuario", font, 30);
        title.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setPosition((win.getSize().x - titleBounds.width) / 2.f, 20.f);

        // Texto de la consigna
        sf::Text promptText("Ingrese su numero de cedula (9 digitos):", font, 24);
        promptText.setFillColor(sf::Color::White);
        promptText.setPosition(50.f, 100.f);

        // Caja de entrada
        sf::RectangleShape inputBox(sf::Vector2f(300.f, 40.f));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(1.f);
        inputBox.setPosition(50.f, 160.f);

        // Texto para mostrar lo que el usuario escribe
        sf::Text inputText("", font, 24);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(55.f, 165.f);

        // Texto de error
        sf::Text errorText("", font, 20);
        errorText.setFillColor(sf::Color::Red);
        errorText.setPosition(50.f, 210.f);

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

            win.clear();
            win.draw(background);
            win.draw(header);
            win.draw(title);
            win.draw(promptText);
            win.draw(inputBox);
            win.draw(inputText);
            win.draw(errorText);
            win.display();
        }

        // Buscar el usuario por c�dula
        UserData* currentUser = user.searchUserById(idNumber);
        if (currentUser) {
            sf::Text welcomeText("Usuario encontrado. Bienvenido, " + currentUser->getName() + ".", font, 24);
            welcomeText.setFillColor(sf::Color::White);
            welcomeText.setPosition(50.f, 250.f);
            win.clear();
            win.draw(background);
            win.draw(header);
            win.draw(title);
            win.draw(welcomeText);
            win.display();
            sf::sleep(sf::seconds(2));
            return currentUser;
        }

        // Texto de registro
        sf::Text registerPrompt("Usuario no encontrado. �Desea registrarse?", font, 24);
        registerPrompt.setFillColor(sf::Color::White);
        registerPrompt.setPosition(50.f, 250.f);

        // Bot�n "S�"
        sf::RectangleShape siButton(sf::Vector2f(100.f, 40.f));
        siButton.setFillColor(sf::Color(255, 140, 0));  // HIGHLIGHT_COLOR
        siButton.setPosition(50.f, 320.f);

        sf::Text siText("S�", font, 24);
        siText.setFillColor(sf::Color::White);
        sf::FloatRect siBounds = siText.getLocalBounds();
        siText.setPosition(siButton.getPosition().x + (siButton.getSize().x - siBounds.width) / 2.f, siButton.getPosition().y + 5.f);

        // Bot�n "No"
        sf::RectangleShape noButton(sf::Vector2f(100.f, 40.f));
        noButton.setFillColor(sf::Color(90, 90, 90));  // BOX_COLOR
        noButton.setPosition(200.f, 320.f);

        sf::Text noText("No", font, 24);
        noText.setFillColor(sf::Color::White);
        sf::FloatRect noBounds = noText.getLocalBounds();
        noText.setPosition(noButton.getPosition().x + (noButton.getSize().x - noBounds.width) / 2.f, noButton.getPosition().y + 5.f);

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

            win.clear();
            win.draw(background);
            win.draw(header);
            win.draw(title);
            win.draw(registerPrompt);
            win.draw(siButton);
            win.draw(siText);
            win.draw(noButton);
            win.draw(noText);
            win.display();
        }

        if (!registerUser) {
            return nullptr;
        }

        // Registrar usuario
        user.createUser(user, idNumber, win);
        currentUser = user.searchUserById(idNumber);
        if (currentUser) {
            sf::Text successText("Usuario registrado. Bienvenido, " + currentUser->getName(), font, 24);
            successText.setFillColor(sf::Color::White);
            successText.setPosition(50.f, 250.f);
            win.clear();
            win.draw(background);
            win.draw(header);
            win.draw(title);
            win.draw(successText);
            win.display();
            sf::sleep(sf::seconds(2));
            return currentUser;
        }

        std::cout << "Error al registrar el usuario. Intente nuevamente.\n";
    }
}

int Sale::chooseSegment(Segment& segment, int selectedEvent, sf::RenderWindow& window)
{
    
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
    float startY = 100.f;    
    float spacing = 40.f;    

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

    // Bucle para esperar la selecci�n mediante clic
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
                // Resaltar la opci�n sobre la que pasa el mouse
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

        // Dibujar el t�tulo en la parte superior
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

Seating& Sale::ensureSeating(int selectedEvent, int selectedSegment, List<List<Segment>>& segments, std::map<std::tuple<int, int>, Seating>& seatingMap, sf::RenderWindow& win)   // Agregado el par�metro 'win'
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
        seating.displaySeats(win);  
        
    }
    else {
     
        cout << " " << endl;
    }


    return seating;
}

int Sale::buyTickets(UserData* currentUser, Event& event, int selectedEvent, Seating& seating, sf::RenderWindow& window) {
    
    Event& currentEvent = event.getEvents().getAt(selectedEvent);

    
    int availableSeats = 0;
    for (int i = 0; i < seating.getNumberOfRows(); i++)
        for (int j = 0; j < seating.getNumberOfColumns(); j++)
            if (!seating.getSeatPurchased()[i][j])
                availableSeats++;

    std::string userId = currentUser->getIdNumber();
    int currentTickets = currentEvent.getTicketsPurchasedByUser(userId);

    int maxTickets = 5 - currentTickets;
    if (maxTickets <= 0) {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            return 0;
        sf::Text msg("Ya has comprado el numero maximo de 5 boletos para este evento.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(msg);
        window.display();
        sf::sleep(sf::seconds(3));
        return 0;
    }

    
    if (availableSeats < maxTickets)
        maxTickets = availableSeats;

    // 5. Preparar la interfaz para solicitar la cantidad de boletos.
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return 0;
    }
    sf::Text prompt("�Cuantos boletos desea comprar? (max " + std::to_string(maxTickets) + "):", font, 24);
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
                            // Registrar la compra en el evento espec�fico
                            if (currentEvent.purchaseTickets(userId, numTickets, window)) {
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

        // Bot�n "Aplicar"
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

        // 3. Verificar el c�digo ingresado
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

std::string Sale::askCardNumber(sf::RenderWindow& window) {
    // Cargar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente." << std::endl;
        return "";
    }

    // Crear el texto de consigna
    sf::Text prompt("Ingrese el numero de su tarjeta (12 digitos):", font, 24);
    prompt.setFillColor(TEXT_COLOR_EV);
    prompt.setPosition(50.f, 50.f);

    // Definir la caja de entrada
    sf::RectangleShape inputBox(sf::Vector2f(300.f, 40.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50.f, 120.f);

    // Texto para mostrar lo que escribe el usuario
    sf::Text inputText("", font, 24);
    inputText.setFillColor(TEXT_COLOR_EV);
    inputText.setPosition(55.f, 125.f);

    // Texto para mensajes de error
    sf::Text errorText("", font, 20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50.f, 180.f);

    // Crear el bot�n "Aplicar" (similar al bot�n "Guardar" en createUser)
    sf::RectangleShape applyButton(sf::Vector2f(120.f, 40.f));
    applyButton.setFillColor(sf::Color(0, 180, 0)); // Color verde
    applyButton.setPosition(50.f, 240.f);

    sf::Text applyButtonText("Aplicar", font, 24);
    applyButtonText.setFillColor(sf::Color::White);
    {
        sf::FloatRect bounds = applyButtonText.getLocalBounds();
        applyButtonText.setPosition(
            applyButton.getPosition().x + (applyButton.getSize().x - bounds.width) / 2.f - bounds.left,
            applyButton.getPosition().y + (applyButton.getSize().y - bounds.height) / 2.f - bounds.top
        );
    }

    std::string cardNumber;
    bool valid = false;

    
    while (!valid && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Manejo de cierre de ventana
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            }
            // Capturar caracteres ingresados
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    // Solo se aceptan d�gitos y se limita a 12 caracteres
                    if (std::isdigit(c) && cardNumber.size() < 12) {
                        cardNumber.push_back(c);
                        inputText.setString(cardNumber);
                        errorText.setString("");
                    }
                }
            }
            // Manejar retroceso y validaci�n al presionar Enter
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace) {
                    if (!cardNumber.empty()) {
                        cardNumber.pop_back();
                        inputText.setString(cardNumber);
                        errorText.setString("");
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    // Validar al presionar Enter
                    if (cardNumber.length() == 12 &&
                        cardNumber.find_first_not_of("0123456789") == std::string::npos) {
                        valid = true;
                    }
                    else {
                        errorText.setString("Numero invalido. Ingrese 12 digitos numericos.");
                    }
                }
            }
           
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (applyButton.getGlobalBounds().contains(mousePos)) {
                    if (cardNumber.length() == 12 &&
                        cardNumber.find_first_not_of("0123456789") == std::string::npos) {
                        valid = true;
                    }
                    else {
                        errorText.setString("Numero invalido. Ingrese 12 digitos numericos.");
                    }
                }
            }
        }
        // Actualizar la pantalla
        window.clear(BG_COLOR_EV);
        window.draw(prompt);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(errorText);
        window.draw(applyButton);
        window.draw(applyButtonText);
        window.display();
    }
    return cardNumber;
}

void Sale::printInvoice(UserData* currentUser, Event& event, int selectedEvent,
    List<List<Segment>>& segments, int selectedSegment,
    int numTickets, float ticketPrice, float discountPercentage, float totalCost,
    int* purchasedRows, char* purchasedCols, int numPurchasedSeats,
    std::string cardNumber, sf::RenderWindow& window)
{
    // 1. Preparar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error al cargar la fuente para la factura." << std::endl;
        return;
    }

  
    std::vector<sf::Text> invoiceLines;
    float startY = 20.f;           
    const float lineSpacing = 30.f; 

    
    sf::Text header("==================== FACTURA ====================", font, 24);
    header.setFillColor(TEXT_COLOR_EV);
    float headerX = (window.getSize().x - header.getLocalBounds().width) / 2.f;
    float headerY = startY + 20.f;  
    header.setPosition(headerX, headerY);
    invoiceLines.push_back(header);
    startY = headerY + lineSpacing;

   
    auto centerText = [&window](sf::Text& text, float y) {
        float x = (window.getSize().x - text.getLocalBounds().width) / 2.f;
        text.setPosition(x, y);
        };

    // Datos del usuario
    sf::Text userLine("Usuario: " + currentUser->getName(), font, 20);
    userLine.setFillColor(TEXT_COLOR_EV);
    centerText(userLine, startY);
    invoiceLines.push_back(userLine);
    startY += lineSpacing;

    sf::Text cedulaLine("Cedula: " + currentUser->getIdNumber(), font, 20);
    cedulaLine.setFillColor(TEXT_COLOR_EV);
    centerText(cedulaLine, startY);
    invoiceLines.push_back(cedulaLine);
    startY += lineSpacing;

    // Datos del evento
    sf::Text eventLine("Evento: " + event.getEvents().getAt(selectedEvent).getName(), font, 20);
    eventLine.setFillColor(TEXT_COLOR_EV);
    centerText(eventLine, startY);
    invoiceLines.push_back(eventLine);
    startY += lineSpacing;

    sf::Text segmentLine("Segmento: " + segments.getAt(selectedEvent).getAt(selectedSegment).getName(), font, 20);
    segmentLine.setFillColor(TEXT_COLOR_EV);
    centerText(segmentLine, startY);
    invoiceLines.push_back(segmentLine);
    startY += lineSpacing;

    // Informaci�n de la tarjeta (mostrar solo los 4 �ltimos d�gitos)
    std::string cardMasked = "****-****-****-" + cardNumber.substr(cardNumber.length() - 4);
    sf::Text cardLine("Tarjeta: " + cardMasked, font, 20);
    cardLine.setFillColor(TEXT_COLOR_EV);
    centerText(cardLine, startY);
    invoiceLines.push_back(cardLine);
    startY += lineSpacing;

    // Informaci�n de los boletos
    std::ostringstream oss;
    oss << "Boletos: " << numTickets << " x $" << std::fixed << std::setprecision(2) << ticketPrice;
    sf::Text ticketsLine(oss.str(), font, 20);
    ticketsLine.setFillColor(TEXT_COLOR_EV);
    centerText(ticketsLine, startY);
    invoiceLines.push_back(ticketsLine);
    startY += lineSpacing;

    // Descuento (si se aplic�)
    if (discountPercentage > 0)
    {
        sf::Text discountLine("Descuento: " + std::to_string(discountPercentage) + "%", font, 20);
        discountLine.setFillColor(TEXT_COLOR_EV);
        centerText(discountLine, startY);
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
    centerText(seatsLine, startY);
    invoiceLines.push_back(seatsLine);
    startY += lineSpacing;

    // Total pagado
    std::ostringstream oss2;
    oss2 << "Total pagado: $" << std::fixed << std::setprecision(2) << totalCost;
    sf::Text totalLine(oss2.str(), font, 20);
    totalLine.setFillColor(TEXT_COLOR_EV);
    centerText(totalLine, startY);
    invoiceLines.push_back(totalLine);
    startY += lineSpacing;

    // Pie de p�gina
    sf::Text footer("=================================================", font, 24);
    footer.setFillColor(TEXT_COLOR_EV);
    centerText(footer, startY);
    invoiceLines.push_back(footer);

    // 3. Dibujar la factura en la ventana.
    window.clear(BG_COLOR_EV);
    for (const auto& line : invoiceLines)
    {
        window.draw(line);
    }

    
    sf::RectangleShape finalizeButton(sf::Vector2f(120.f, 40.f));
    finalizeButton.setFillColor(sf::Color::Green);
    float buttonX = (window.getSize().x - finalizeButton.getSize().x) / 2.f;
    float buttonY = startY + 40.f; // 40 p�xeles debajo del �ltimo rengl�n
    finalizeButton.setPosition(buttonX, buttonY);

    sf::Text finalizeText("Finalizar", font, 24);
    finalizeText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = finalizeText.getLocalBounds();
    finalizeText.setPosition(
        buttonX + (finalizeButton.getSize().x - textBounds.width) / 2.f - textBounds.left,
        buttonY + (finalizeButton.getSize().y - textBounds.height) / 2.f - textBounds.top
    );

    window.draw(finalizeButton);
    window.draw(finalizeText);
    window.display();

    // 5. Esperar que el usuario haga clic en el bot�n "Finalizar"
    bool clicked = false;
    while (window.isOpen() && !clicked)
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                if (finalizeButton.getGlobalBounds().contains(mousePos))
                {
                    clicked = true;
                    break;
                }
            }
        }
    }

    
    window.clear(BG_COLOR_EV);
    window.display();
}

std::pair<int, int> getSoldSeatSelection(sf::RenderWindow& window, Seating& seating)
{
    
    const float seatWidth = 30.f;
    const float seatHeight = 30.f;
    const float spacing = 10.f;
    const float marginX = 50.f;
    const float marginY = 150.f; 

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        return { -1, -1 };
    }

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

                if (x < marginX || y < marginY)
                    continue;

                int col = static_cast<int>((x - marginX) / (seatWidth + spacing));
                int row = static_cast<int>((y - marginY) / (seatHeight + spacing));

                float seatX = marginX + col * (seatWidth + spacing);
                float seatY = marginY + row * (seatHeight + spacing);
                if (x >= seatX && x <= seatX + seatWidth &&
                    y >= seatY && y <= seatY + seatHeight)
                {
                    if (row < 0 || row >= seating.getNumberOfRows() || col < 0 || col >= seating.getNumberOfColumns())
                        continue;

                    // Aqu� se espera que el asiento est� ocupado para poder cancelarlo
                    if (!seating.getSeatPurchased()[row][col])
                    {
                        sf::Text errorMsg("Asiento no vendido, elija otro.", font, 24);
                        errorMsg.setFillColor(sf::Color::Red);
                        errorMsg.setPosition(50.f, marginY + seating.getNumberOfRows() * (seatHeight + spacing) + 20.f);
                        window.clear(BG_COLOR_EV);
                        seating.displaySeats(window);
                        window.draw(errorMsg);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        continue;
                    }
                    return { row, col };
                }
            }
        }
        window.clear(BG_COLOR_EV);
        seating.displaySeats(window);
        window.display();
    }
    return { -1, -1 };
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
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        return;

    if (event.getEventCount() == 0)
    {
        // Si NO hay eventos disponibles, mostramos mensaje y el bot�n "Volver".
        sf::Text msg("No hay eventos disponibles para cancelar compras.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);

        // Bot�n "Volver"
        sf::RectangleShape volverButton(sf::Vector2f(120.f, 40.f));
        volverButton.setFillColor(sf::Color::Red);
        volverButton.setPosition(50.f, 120.f);

        sf::Text volverText("Volver", font, 24);
        volverText.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = volverText.getLocalBounds();
            volverText.setPosition(
                volverButton.getPosition().x + (volverButton.getSize().x - bounds.width) / 2.f - bounds.left,
                volverButton.getPosition().y + (volverButton.getSize().y - bounds.height) / 2.f - bounds.top
            );
        }

        // Bucle de eventos para esperar clic en "Volver" o cerrar ventana
        bool done = false;
        while (window.isOpen() && !done)
        {
            sf::Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                    if (volverButton.getGlobalBounds().contains(mousePos))
                    {
                        // Se presion� "Volver": simplemente retornamos.
                        return;
                    }
                }
            }

            window.clear(BG_COLOR_EV);
            window.draw(msg);
            window.draw(volverButton);
            window.draw(volverText);
            window.display();
        }
        return;
    }

    // 2. Solicitar c�dula mediante SFML (con un bot�n "Aceptar").
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

    // Bot�n "Aceptar"
    sf::RectangleShape aceptarButton(sf::Vector2f(120.f, 40.f));
    aceptarButton.setFillColor(sf::Color::Green);
    aceptarButton.setPosition(50.f, 180.f);

    sf::Text aceptarText("Aceptar", font, 24);
    aceptarText.setFillColor(sf::Color::White);
    {
        sf::FloatRect bounds = aceptarText.getLocalBounds();
        aceptarText.setPosition(
            aceptarButton.getPosition().x + (aceptarButton.getSize().x - bounds.width) / 2.f - bounds.left,
            aceptarButton.getPosition().y + (aceptarButton.getSize().y - bounds.height) / 2.f - bounds.top
        );
    }

    std::string idNumber;
    bool inputDone = false;
    while (!inputDone && window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            if (ev.type == sf::Event::TextEntered)
            {
                if (ev.text.unicode < 128)
                {
                    char c = static_cast<char>(ev.text.unicode);
                    if (isdigit(c) && idNumber.size() < 9)
                    {
                        idNumber.push_back(c);
                        inputText.setString(idNumber);
                    }
                }
            }
            if (ev.type == sf::Event::KeyPressed)
            {
                // Backspace para borrar
                if (ev.key.code == sf::Keyboard::Backspace && !idNumber.empty())
                {
                    idNumber.pop_back();
                    inputText.setString(idNumber);
                }
                // Tambi�n podr�amos permitir Enter aqu�, si se desea
                else if (ev.key.code == sf::Keyboard::Enter)
                {
                    if (idNumber.size() == 9)
                        inputDone = true;  // Cedula v�lida
                }
            }
            // Detecci�n de clic en el bot�n "Aceptar"
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                if (aceptarButton.getGlobalBounds().contains(mousePos))
                {
                    // Validar
                    if (idNumber.size() == 9)
                        inputDone = true;
                }
            }
        }

        // Render
        window.clear(BG_COLOR_EV);
        window.draw(prompt);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(aceptarButton);
        window.draw(aceptarText);
        window.display();
    }

    if (idNumber.size() != 9)
    {
        
        return;
    }

    // 3. Buscar usuario por c�dula.
    UserData* currentUser = user.searchUserById(idNumber);
    if (!currentUser)
    {
        // Mostrar mensaje y bot�n "Volver"
        sf::Text err("Usuario no encontrado.", font, 24);
        err.setFillColor(TEXT_COLOR_EV);
        err.setPosition(50.f, 50.f);

        sf::RectangleShape volverButton(sf::Vector2f(120.f, 40.f));
        volverButton.setFillColor(sf::Color::Red);
        volverButton.setPosition(50.f, 120.f);

        sf::Text volverText("Volver", font, 24);
        volverText.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = volverText.getLocalBounds();
            volverText.setPosition(
                volverButton.getPosition().x + (volverButton.getSize().x - bounds.width) / 2.f - bounds.left,
                volverButton.getPosition().y + (volverButton.getSize().y - bounds.height) / 2.f - bounds.top
            );
        }

        bool done = false;
        while (window.isOpen() && !done)
        {
            sf::Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                    if (volverButton.getGlobalBounds().contains(mousePos))
                    {
                        done = true; // Volvemos
                    }
                }
            }

            window.clear(BG_COLOR_EV);
            window.draw(err);
            window.draw(volverButton);
            window.draw(volverText);
            window.display();
        }
        return;
    }

    
    std::vector<sf::Text> eventOptions;
    for (int i = 1; i <= event.getEventCount(); i++)
    {
        std::string evName = event.getEvents().getAt(i).getName();
        sf::Text opt(std::to_string(i) + ". " + evName, font, 24);
        opt.setFillColor(TEXT_COLOR_EV);
        opt.setPosition(50.f, 100.f + (i - 1) * 40.f);
        eventOptions.push_back(opt);
    }

    int selectedEventIndex = -1;
    bool eventSelected = false;
    while (window.isOpen() && !eventSelected)
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < (int)eventOptions.size(); i++)
                {
                    if (eventOptions[i].getGlobalBounds().contains(mousePos))
                    {
                        selectedEventIndex = i + 1;  // i es 0-based, eventos son 1-based
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

    // 5. Obtener el evento espec�fico.
    Event& currentEvent = event.getEvents().getAt(selectedEventIndex);

    // 6. Verificar boletos comprados en el evento seleccionado.
    int purchasedTickets = currentEvent.getTicketsPurchasedByUser(idNumber);
    if (purchasedTickets == 0)
    {
        // Si el usuario no tiene boletos, mostramos un mensaje y bot�n "Volver".
        sf::Text msg("El usuario no tiene boletos comprados para este evento.", font, 24);
        msg.setFillColor(TEXT_COLOR_EV);
        msg.setPosition(50.f, 50.f);

        sf::RectangleShape volverButton(sf::Vector2f(120.f, 40.f));
        volverButton.setFillColor(sf::Color::Red);
        volverButton.setPosition(50.f, 120.f);

        sf::Text volverText("Volver", font, 24);
        volverText.setFillColor(sf::Color::White);
        {
            sf::FloatRect bounds = volverText.getLocalBounds();
            volverText.setPosition(
                volverButton.getPosition().x + (volverButton.getSize().x - bounds.width) / 2.f - bounds.left,
                volverButton.getPosition().y + (volverButton.getSize().y - bounds.height) / 2.f - bounds.top
            );
        }

        bool done = false;
        while (window.isOpen() && !done)
        {
            sf::Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                    if (volverButton.getGlobalBounds().contains(mousePos))
                    {
                        done = true;
                    }
                }
            }
            window.clear(BG_COLOR_EV);
            window.draw(msg);
            window.draw(volverButton);
            window.draw(volverText);
            window.display();
        }
        return;
    }

    // Mostrar cu�ntos boletos tiene
    sf::Text ticketInfo("Usted tiene " + std::to_string(purchasedTickets) + " boletos para este evento.", font, 24);
    ticketInfo.setFillColor(TEXT_COLOR_EV);
    ticketInfo.setPosition(50.f, 50.f);
    window.clear(BG_COLOR_EV);
    window.draw(ticketInfo);
    window.display();
    sf::sleep(sf::seconds(2));

    // 7. Solicitar el numero de boletos a cancelar con una caja de texto.
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

    // Bot�n "Aceptar" para confirmar cu�ntos boletos se van a cancelar
    sf::RectangleShape aceptarCancelButton(sf::Vector2f(120.f, 40.f));
    aceptarCancelButton.setFillColor(sf::Color::Green);
    aceptarCancelButton.setPosition(50.f, 180.f);

    sf::Text aceptarCancelText("Aceptar", font, 24);
    aceptarCancelText.setFillColor(sf::Color::White);
    {
        sf::FloatRect bounds = aceptarCancelText.getLocalBounds();
        aceptarCancelText.setPosition(
            aceptarCancelButton.getPosition().x + (aceptarCancelButton.getSize().x - bounds.width) / 2.f - bounds.left,
            aceptarCancelButton.getPosition().y + (aceptarCancelButton.getSize().y - bounds.height) / 2.f - bounds.top
        );
    }

    std::string cancelStr;
    bool cancelDone = false;
    int toCancel = 0;

    while (!cancelDone && window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            if (ev.type == sf::Event::TextEntered)
            {
                if (ev.text.unicode < 128)
                {
                    char c = static_cast<char>(ev.text.unicode);
                    if (std::isdigit(c))
                    {
                        cancelStr.push_back(c);
                        cancelInputText.setString(cancelStr);
                    }
                }
            }
            if (ev.type == sf::Event::KeyPressed)
            {
                if (ev.key.code == sf::Keyboard::Backspace && !cancelStr.empty())
                {
                    cancelStr.pop_back();
                    cancelInputText.setString(cancelStr);
                }
                if (ev.key.code == sf::Keyboard::Enter)
                {
                    // Intentar parsear
                    try
                    {
                        int num = std::stoi(cancelStr);
                        if (num >= 1 && num <= purchasedTickets)
                        {
                            toCancel = num;
                            cancelDone = true;
                        }
                        else
                        {
                            // Mostrar error temporal
                            sf::Text err("Ingrese un numero entre 1 y " + std::to_string(purchasedTickets), font, 24);
                            err.setFillColor(sf::Color::Red);
                            err.setPosition(50.f, 240.f);
                            window.clear(BG_COLOR_EV);
                            window.draw(cancelPrompt);
                            window.draw(cancelInputBox);
                            window.draw(cancelInputText);
                            window.draw(aceptarCancelButton);
                            window.draw(aceptarCancelText);
                            window.draw(err);
                            window.display();
                            sf::sleep(sf::seconds(2));
                            cancelStr.clear();
                            cancelInputText.setString("");
                        }
                    }
                    catch (...)
                    {
                        // Error de parseo
                        sf::Text err("Entrada invalida. Ingrese un numero.", font, 24);
                        err.setFillColor(sf::Color::Red);
                        err.setPosition(50.f, 240.f);
                        window.clear(BG_COLOR_EV);
                        window.draw(cancelPrompt);
                        window.draw(cancelInputBox);
                        window.draw(cancelInputText);
                        window.draw(aceptarCancelButton);
                        window.draw(aceptarCancelText);
                        window.draw(err);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        cancelStr.clear();
                        cancelInputText.setString("");
                    }
                }
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                if (aceptarCancelButton.getGlobalBounds().contains(mousePos))
                {
                    // Validar
                    try
                    {
                        int num = std::stoi(cancelStr);
                        if (num >= 1 && num <= purchasedTickets)
                        {
                            toCancel = num;
                            cancelDone = true;
                        }
                    }
                    catch (...) { /* Ignorar; se muestra error abajo si no cumple. */ }
                }
            }
        }

        window.clear(BG_COLOR_EV);
        window.draw(cancelPrompt);
        window.draw(cancelInputBox);
        window.draw(cancelInputText);
        window.draw(aceptarCancelButton);
        window.draw(aceptarCancelText);
        window.display();
    }

    // 8. Mostrar segmentos disponibles para el evento seleccionado.
    std::vector<sf::Text> segmentOptions;
    int numSegs = segment.getSegmentCount().getAt(selectedEventIndex);
    for (int i = 1; i <= numSegs; i++)
    {
        std::ostringstream oss;
        oss << i << ". "
            << segment.getSegmentsByEvent().getAt(selectedEventIndex).getAt(i).getName()
            << " - Precio: $"
            << segment.getSegmentsByEvent().getAt(selectedEventIndex).getAt(i).getPrice();
        sf::Text opt(oss.str(), font, 24);
        opt.setFillColor(TEXT_COLOR_EV);
        opt.setPosition(50.f, 100.f + (i - 1) * 40.f);
        segmentOptions.push_back(opt);
    }

    int chosenSegment = -1;
    bool segmentChosen = false;
    while (window.isOpen() && !segmentChosen)
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < (int)segmentOptions.size(); i++)
                {
                    if (segmentOptions[i].getGlobalBounds().contains(mousePos))
                    {
                        chosenSegment = i + 1; // 1-based
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

    // 9. Obtener la sala correspondiente al segmento seleccionado.
    std::tuple<int, int> seatingKey = std::make_tuple(selectedEventIndex, chosenSegment);
    Seating* pSeating = nullptr;
    if (seatingMap.find(seatingKey) != seatingMap.end())
        pSeating = &seatingMap[seatingKey];
    else
    {
        sf::Text errMsg("Error: No se pudo obtener la sala.", font, 24);
        errMsg.setFillColor(sf::Color::Red);
        errMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(errMsg);
        window.display();
        sf::sleep(sf::seconds(2));
        return;
    }

    // 10. Solicitar la liberaci�n de asientos de forma interactiva.
    for (int i = 0; i < toCancel; i++)
    {
        window.clear(BG_COLOR_EV);
        pSeating->displaySeats(window);

        sf::Text seatPrompt("Seleccione el asiento vendido a cancelar (boleto "
            + std::to_string(i + 1) + ")", font, 24);
        seatPrompt.setFillColor(TEXT_COLOR_EV);
        seatPrompt.setPosition(50.f, 20.f);
        window.draw(seatPrompt);
        window.display();

        std::pair<int, int> seat = getSoldSeatSelection(window, *pSeating);
        if (seat.first == -1 || seat.second == -1)
        {
            // Si se cierra la ventana o la selecci�n es inv�lida, abortamos.
            return;
        }

        // Liberar el asiento
        if (pSeating->freeSeat(seat.first, seat.second))
        {
            sf::Text freedMsg("Asiento liberado correctamente para boleto "
                + std::to_string(i + 1), font, 24);
            freedMsg.setFillColor(TEXT_COLOR_EV);
            freedMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(freedMsg);
            window.display();
            sf::sleep(sf::seconds(1));
        }
        else
        {
            sf::Text errMsg("Ese asiento no estaba vendido. Intente otro.", font, 24);
            errMsg.setFillColor(sf::Color::Red);
            errMsg.setPosition(50.f, 50.f);
            window.clear(BG_COLOR_EV);
            window.draw(errMsg);
            window.display();
            sf::sleep(sf::seconds(2));
            i--; // Repetir la selecci�n
        }
    }

    // 11. Actualizar la cantidad de boletos en el evento
    if (currentEvent.cancelTickets(idNumber, toCancel, window))
    {
        sf::Text successMsg("Se han cancelado los boletos correctamente.", font, 24);
        successMsg.setFillColor(TEXT_COLOR_EV);
        successMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(successMsg);
        window.display();
        sf::sleep(sf::seconds(2));
    }
    else
    {
        sf::Text errorMsg("No fue posible cancelar los boletos.", font, 24);
        errorMsg.setFillColor(sf::Color::Red);
        errorMsg.setPosition(50.f, 50.f);
        window.clear(BG_COLOR_EV);
        window.draw(errorMsg);
        window.display();
        sf::sleep(sf::seconds(2));
    }

    // 12. Mensaje final con bot�n "Volver"
    sf::Text finalMsg("Operacion finalizada. Presione 'Volver' para continuar...", font, 24);
    finalMsg.setFillColor(TEXT_COLOR_EV);
    finalMsg.setPosition(50.f, 50.f);

    sf::RectangleShape volverButton2(sf::Vector2f(120.f, 40.f));
    volverButton2.setFillColor(sf::Color::Red);
    volverButton2.setPosition(50.f, 120.f);

    sf::Text volverText2("Volver", font, 24);
    volverText2.setFillColor(sf::Color::White);
    {
        sf::FloatRect bounds = volverText2.getLocalBounds();
        volverText2.setPosition(
            volverButton2.getPosition().x + (volverButton2.getSize().x - bounds.width) / 2.f - bounds.left,
            volverButton2.getPosition().y + (volverButton2.getSize().y - bounds.height) / 2.f - bounds.top
        );
    }

    bool finish = false;
    while (window.isOpen() && !finish)
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                if (volverButton2.getGlobalBounds().contains(mousePos))
                {
                    finish = true; // Salimos
                }
            }
        }
        window.clear(BG_COLOR_EV);
        window.draw(finalMsg);
        window.draw(volverButton2);
        window.draw(volverText2);
        window.display();
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

    // Crear t�tulo de la pantalla
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

    // Bucle para esperar la selecci�n del usuario
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

void cancelSelectedSeats(sf::RenderWindow& window, Seating* pSeating, int toCancel, int* purchasedRows, char* purchasedCols, sf::Font& font)
{
  
    for (int i = 0; i < toCancel; i++)
    {
        // Mostrar la sala y esperar que el usuario haga clic sobre un asiento vendido.
        std::pair<int, int> seat = getSoldSeatSelection(window, *pSeating);
        if (seat.first == -1 || seat.second == -1)
        {
            // Si se cierra la ventana o se devuelve una selecci�n inv�lida, abortamos.
            delete[] purchasedRows;
            delete[] purchasedCols;
            return;
        }

        
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
            i--; // Repetir esta iteraci�n si ocurre un error.
        }
    }
}