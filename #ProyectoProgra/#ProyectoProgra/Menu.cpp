#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Menu.h"
#include <iostream>


// Alias para evitar conflictos entre las clases de SFML y las del proyecto
namespace sfml = sf;

#include "Menu.h"

// Colores usados en el menú
static const sfml::Color BG_COLOR(200, 200, 200);      // Fondo gris
static const sfml::Color HEADER_COLOR(160, 160, 160);  // Encabezado gris medio
static const sfml::Color BOX_COLOR(230, 230, 230);     // Botones (no usado en este ejemplo)
static const sfml::Color HIGHLIGHT_COLOR(255, 0, 0);   // Rojo para resaltar
static const sfml::Color TEXT_COLOR(sfml::Color::Black);// Texto negro

void Menu::showMenu() {
    // Crear la ventana principal si aún no existe
    if (!window) {
        window = new sfml::RenderWindow(sfml::VideoMode(1200, 800), "Sistema de Ventas de Entradas", sfml::Style::Close);
        window->setKeyRepeatEnabled(true);  // Habilita la repetición de teclas
    }

    // Cargar la fuente (asegúrate de que la ruta sea correcta)
    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // Sección del header: área de 100 píxeles dividida en dos filas
    const float headerHeight = 100.f;
    sfml::RectangleShape header(sfml::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    // Etiquetas para las 9 opciones del menú principal
    const int numOptions = 9;
    std::string optionLabels[numOptions] = {
        "Configurar Event",        // Opción 1
        "Configurar Descuentos",   // Opción 2
        "Vender Entrada",          // Opción 3
        "Consultar Ventas",        // Opción 4
        "Cancelar Compra",         // Opción 5
        "Acerca de",               // Opción 6
        "Preguntas y Respuestas",  // Opción 7
        "Salir",                   // Opción 8
        "Actualizar Reporte"       // Opción 9
    };

    // Dividir en dos filas:
    // - Primera fila: 5 opciones (índices 0 a 4)
    // - Segunda fila: 4 opciones (índices 5 a 8)
    const int row1Count = 5;
    const int row2Count = 4;
    sfml::Text headerText[numOptions];

    // Primera fila
    float row1Y_top = 0.f;
    float row1Height = headerHeight / 2.f; // 50 píxeles
    float row1Spacing = 1200.f / row1Count;
    for (int i = 0; i < row1Count; i++) {
        headerText[i].setFont(font);
        headerText[i].setCharacterSize(15);
        headerText[i].setFillColor(TEXT_COLOR);
        headerText[i].setString(optionLabels[i]);
        sfml::FloatRect textBounds = headerText[i].getLocalBounds();
        float posX = i * row1Spacing + (row1Spacing - textBounds.width) / 2.f - textBounds.left;
        float posY = row1Y_top + (row1Height - textBounds.height) / 2.f - textBounds.top;
        headerText[i].setPosition(posX, posY);
    }

    // Segunda fila
    float row2Y_top = headerHeight / 2.f;
    float row2Height = headerHeight / 2.f; // Otros 50 píxeles
    float row2Spacing = 1200.f / row2Count;
    for (int i = row1Count; i < numOptions; i++) {
        headerText[i].setFont(font);
        headerText[i].setCharacterSize(15);
        headerText[i].setFillColor(TEXT_COLOR);
        headerText[i].setString(optionLabels[i]);
        int indexInRow = i - row1Count;
        sfml::FloatRect textBounds = headerText[i].getLocalBounds();
        float posX = indexInRow * row2Spacing + (row2Spacing - textBounds.width) / 2.f - textBounds.left;
        float posY = row2Y_top + (row2Height - textBounds.height) / 2.f - textBounds.top;
        headerText[i].setPosition(posX, posY);
    }

    // Texto central de bienvenida
    sfml::Text welcomeText("Bienvenido\nSistema de ventas de entrada\nDesarrollado por:", font, 30);
    welcomeText.setFillColor(TEXT_COLOR);
    welcomeText.setStyle(sfml::Text::Bold);
    welcomeText.setPosition(200.f, 200.f);
    welcomeText.setLineSpacing(1.5f);

    // Bucle principal
    while (window->isOpen()) {
        sfml::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sfml::Event::Closed) {
                window->close();
                return;
            }
            // Resaltar opción al pasar el mouse
            if (event.type == sfml::Event::MouseMoved) {
                sfml::Vector2f mousePos(
                    static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y)
                );
                for (int i = 0; i < numOptions; i++) {
                    if (headerText[i].getGlobalBounds().contains(mousePos))
                        headerText[i].setFillColor(HIGHLIGHT_COLOR);
                    else
                        headerText[i].setFillColor(TEXT_COLOR);
                }
            }
            // Detectar clic en alguna opción
            if (event.type == sfml::Event::MouseButtonPressed &&
                event.mouseButton.button == sfml::Mouse::Left) {
                sfml::Vector2f mousePos(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );
                for (int i = 0; i < numOptions; i++) {
                    if (headerText[i].getGlobalBounds().contains(mousePos)) {
                        executeOption(i + 1);
                        break;
                    }
                }
            }
        }

        window->clear(BG_COLOR);
        window->draw(header);
        for (int i = 0; i < numOptions; i++) {
            window->draw(headerText[i]);
        }
        window->draw(welcomeText);
        window->display();
    }
}

// Ejecuta la acción correspondiente a cada opción
void Menu::executeOption(int option) {
    switch (option) {
    case 1:
        // Configurar evento
        configureEvent(event, segment);
        break;
    case 2:
        // Configurar descuentos (ahora con submenú SFML)
        showDiscountMenuSFML(discount);
        break;
    case 3:
        // Vender entrada
        sellTicket(user);
        break;
    case 4:
        // Consultar ventas
        seating.checkSales(event, segment, seatingMap);
        break;
    case 5:
        // Cancelar compra
        cancelPurchase();
        break;
    case 6:
        // Acerca de
        showAbout();
        break;
    case 7:
        // Preguntas y Respuestas (FAQ)
        showFaqMenuSFML();
        break;
    case 8:
        // Salir
        std::cout << "Saliendo del programa. Gracias!" << std::endl;
        window->close();
        break;
    case 9:
        // Actualizar reporte
        std::cout << "Opción 9: Actualizar Reporte\n";
        updateReport();
        break;
    default:
        std::cout << "Opción inválida. Intente nuevamente.\n";
    }
}

// Menú SFML para configurar descuentos (extraído del primer código)
void Menu::showDiscountMenuSFML(Discount& discount)
{
    // Reutilizamos la ventana principal
    sf::RenderWindow& win = *window;
    // Cambiamos el título para el submenú de descuentos
    win.setTitle("Gestión de Descuentos");

    // Cargar la fuente (asegúrate de que la ruta sea correcta)
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente para el menú de descuentos.\n";
        return;
    }

    // Configuración del encabezado
    const float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    // Definición de las opciones del submenú (4 opciones)
    const int numOptions = 4;
    std::string optionLabels[numOptions] = {
        "Generar códigos",
        "Mostrar registro",
        "Eliminar código",
        "Volver al menú"
    };

    sf::Text options[numOptions];
    float spacing = 1200.f / numOptions;
    for (int i = 0; i < numOptions; i++)
    {
        options[i].setFont(font);
        options[i].setString(optionLabels[i]);
        options[i].setCharacterSize(20);
        options[i].setFillColor(TEXT_COLOR);
        sf::FloatRect bounds = options[i].getLocalBounds();
        float posX = i * spacing + (spacing - bounds.width) / 2.f - bounds.left;
        float posY = (headerHeight - bounds.height) / 2.f - bounds.top;
        options[i].setPosition(posX, posY);
    }

    // Título del submenú
    sf::Text titleText("Gestión de Descuentos", font, 30);
    titleText.setFillColor(TEXT_COLOR);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((1200.f - titleBounds.width) / 2.f, headerHeight + 50.f);

    bool inDiscountMenu = true;
    while (inDiscountMenu && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
                return;
            }
            // Cambio de color al pasar el mouse sobre las opciones
            if (ev.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseMove.x),
                    static_cast<float>(ev.mouseMove.y));
                for (int i = 0; i < numOptions; i++)
                {
                    if (options[i].getGlobalBounds().contains(mousePos))
                        options[i].setFillColor(HIGHLIGHT_COLOR);
                    else
                        options[i].setFillColor(TEXT_COLOR);
                }
            }
            // Procesar clics en las opciones
            if (ev.type == sf::Event::MouseButtonPressed &&
                ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < numOptions; i++)
                {
                    if (options[i].getGlobalBounds().contains(mousePos))
                    {
                        switch (i)
                        {
                        case 0: // Generar códigos
                            generateCodesSFML(discount);
                            break;
                        case 1: // Mostrar registro
                            showDiscountCodesSFML(discount);
                            break;
                        case 2: // Eliminar código
                            deleteDiscountSFML(discount);
                            break;
                        case 3: // Volver al menú principal
                            inDiscountMenu = false;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
        win.clear(BG_COLOR);
        win.draw(header);
        for (int i = 0; i < numOptions; i++)
            win.draw(options[i]);
        win.draw(titleText);
        win.display();
    }
    // Al salir del submenú, restablecemos el título del menú principal
    win.setTitle("Sistema de Ventas de Entradas");
}


// ------------------------------------------------------------------------------------

int Menu::validateChoice(int& choice, int& size)
{
    using namespace std;
    while (true)
    {
        cout << "Seleccione una opcion: " << endl;
        if (cin >> choice && choice >= 1 && choice <= size)
        {
            return choice;
        }
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Entrada invalida." << endl;
    }
}

void Menu::configureDiscounts(Discount& discount)
{
    discount.configureDiscounts();
}

// ---------------------------------------------------------
void Menu::configureEvent(Event& event, Segment& segment) {
    sfml::RenderWindow& win = *window;  // Alias para simplificar la sintaxis

    // Cargar la fuente
    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // Crear el área del encabezado
    float headerHeight = 100.f;
    sfml::RectangleShape header(sfml::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    // Definir las 4 opciones del menú de configuración
    const int numOptions = 4;
    std::string optionLabels[numOptions] = {
        "Agregar nuevo evento",
        "Modificar segmentos",
        "Mostrar eventos actuales",
        "Inicio"
    };

    sfml::Text options[numOptions];
    float spacing = 1200.f / numOptions;
    for (int i = 0; i < numOptions; i++) {
        options[i].setFont(font);
        options[i].setString(optionLabels[i]);
        options[i].setCharacterSize(20);
        options[i].setFillColor(TEXT_COLOR);
        sfml::FloatRect textBounds = options[i].getLocalBounds();
        float posX = i * spacing + (spacing - textBounds.width) / 2.f - textBounds.left;
        float posY = (headerHeight - textBounds.height) / 2.f - textBounds.top;
        options[i].setPosition(posX, posY);
    }

    sfml::Text titleText("Configuracion de Evento", font, 30);
    titleText.setFillColor(TEXT_COLOR);
    sfml::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((1200.f - titleBounds.width) / 2.f, headerHeight + 50.f);

    bool inConfigScreen = true;
    while (inConfigScreen) {
        sfml::Event ev;
        while (win.pollEvent(ev)) {
            if (ev.type == sfml::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sfml::Event::MouseMoved) {
                sfml::Vector2f mousePos(static_cast<float>(ev.mouseMove.x),
                    static_cast<float>(ev.mouseMove.y));
                for (int i = 0; i < numOptions; i++) {
                    if (options[i].getGlobalBounds().contains(mousePos))
                        options[i].setFillColor(HIGHLIGHT_COLOR);
                    else
                        options[i].setFillColor(TEXT_COLOR);
                }
            }
            if (ev.type == sfml::Event::MouseButtonPressed && ev.mouseButton.button == sfml::Mouse::Left) {
                sfml::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < numOptions; i++) {
                    if (options[i].getGlobalBounds().contains(mousePos)) {
                        switch (i) {
                        case 0:
                            // Crear un evento nuevo
                            saveEvent(event, segment);
                            seatingMap.clear();
                            break;
                        case 1:
                            // Modificar/Eliminar segmentos
                            modifyOrDeleteSegment(*this, event, segment, seatingMap);
                            break;
                        case 2:
                            // Mostrar eventos actuales
                            listEventAndSegments(event, segment);
                            break;
                        case 3:
                            // Volver al menú principal
                            inConfigScreen = false;
                            break;
                        }
                    }
                }
            }
        }

        win.clear(BG_COLOR);
        win.draw(header);
        for (int i = 0; i < numOptions; i++) {
            win.draw(options[i]);
        }
        win.draw(titleText);
        win.display();
    }
}

void Menu::listEventAndSegments(Event& event, Segment& segment)
{
    using namespace std;
    system("cls");

    if (event.getEventCount() == 0)
    {
        cout << "No hay eventos guardados." << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    cout << "\nLista de eventos y segmentos:\n" << endl;

    List<List<Segment>>& segmentsByEvent = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();

    for (int i = 1; i <= event.getEventCount(); i++)
    {
        cout << "Evento #" << i << ":\n";
        cout << "Nombre: " << event.getEvents().getAt(i).getName() << "\n";
        cout << "Fecha: " << event.getEvents().getAt(i).getDate() << "\n";
        cout << "Descripcion: " << event.getEvents().getAt(i).getDescription() << "\n";

        cout << "Segmentos asociados:\n";

        if (segmentCounts.getAt(i) == 0 || segmentsByEvent.getAt(i).getHead() == nullptr)
        {
            cout << " No tiene segmentos asignados.\n";
            cout << "--------------------------\n";
            continue;
        }

        for (int j = 1; j <= segmentCounts.getAt(i); j++)
        {
            cout << "  Segmento #" << j << ":\n";
            cout << "    Nombre: "
                << segmentsByEvent.getAt(i).getAt(j).getName() << "\n";
            cout << "    Filas: "
                << segmentsByEvent.getAt(i).getAt(j).getRows() << "\n";
            cout << "    Asientos por fila: "
                << segmentsByEvent.getAt(i).getAt(j).getSeats() << "\n";
            cout << "    Precio: "
                << segmentsByEvent.getAt(i).getAt(j).getPrice() << "\n";
        }
        cout << "--------------------------\n";
    }

    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Menu::sellTicket(User& user)
{
    sale.sell(user, event, segment, seatingMap, discount);
}

// CORREGIDO: ahora se pasa la ventana al método de Event
void Menu::saveEvent(Event& event, Segment& segment)
{
    sfml::RenderWindow& win = *window;
    event.saveEvent(win, segment);
}

void Menu::showAbout()
{
    using namespace std;
    system("cls");
    cout << "\n=========================================\n";
    cout << "               ACERCA DE                 \n";
    cout << "=========================================\n";
    cout << "Integrantes del Proyecto:\n";
    cout << "  - Harold Avila Hernandez\n";
    cout << "  - Cristhian Cordero Varela\n";
    cout << "=========================================\n";
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


int Menu::readIntInRange(int min, int max, const std::string& prompt) {
    using namespace std;
    int opt;
    while (true) {
        if (cin >> opt && opt >= min && opt <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return opt;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << prompt;
    }
}

void Menu::modifyOrDeleteSegment(Menu& menu, Event& event, Segment& segment,
    std::map<std::tuple<int, int>, Seating>& seatingMap)
{
    using namespace std;
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles." << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    cout << "Eventos disponibles:" << endl;
    for (int i = 1; i <= event.getEventCount(); i++) {
        cout << i << ". " << event.getEvents().getAt(i).getName() << endl;
    }
    cout << "\nIngrese el numero de evento: ";
    int eventIndex = readIntInRange(1, event.getEventCount());

    List<List<Segment>>& segmentsByEvent = segment.getSegmentsByEvent();
    List<int>& segmentCount = segment.getSegmentCount();
    int numSegments = segmentCount.getAt(eventIndex);

    if (numSegments == 0) {
        cout << "\nEl evento seleccionado no tiene segmentos." << endl;
        cout << "Desea registrar un nuevo segmento? (1. Si, 2. No): ";
        int registerOption = readIntInRange(1, 2);
        if (registerOption == 1) {
            segment.saveSegments(segment, event.getEventCount(), eventIndex);
            return;
        }
        else {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return;
        }
    }

    cout << "\nSegmentos disponibles para el evento " << eventIndex << ":" << endl;
    for (int j = 1; j <= numSegments; j++) {
        Segment seg = segmentsByEvent.getAt(eventIndex).getAt(j);
        cout << j << ". " << seg.getName() << endl
            << "   - Filas: " << seg.getRows() << endl
            << "   - Asientos por fila: " << seg.getSeats() << endl
            << "   - Precio: " << seg.getPrice() << endl;
    }
    cout << "\nSeleccione el numero del segmento que desea modificar o eliminar: ";
    int segmentIndex = readIntInRange(1, numSegments);

    std::tuple<int, int> key = std::make_tuple(eventIndex, segmentIndex);

    auto seatsSold = [&]() -> bool {
        if (seatingMap.find(key) != seatingMap.end()) {
            Seating& room = seatingMap[key];
            for (int i = 0; i < room.getNumberOfRows(); i++) {
                for (int j = 0; j < room.getNumberOfColumns(); j++) {
                    // Ajusta según la implementación real de tu Seating
                    // if (room.getSeatPurchased()[i][j]) return true;
                }
            }
        }
        return false;
        };

    cout << "\nSeleccione una opcion:" << endl;
    cout << "1. Actualizar segmento" << endl;
    cout << "2. Eliminar segmento" << endl;
    int action = readIntInRange(1, 2);

    if (action == 1) {
        if (seatsSold()) {
            cout << "\nEl segmento tiene entradas vendidas; solo se permite actualizar el precio." << endl;
            cout << "Ingrese el nuevo precio: ";
            float newPrice;
            while (!(cin >> newPrice) || newPrice <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Precio invalido, reingrese: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            segmentsByEvent.getAt(eventIndex).getAt(segmentIndex).setPrice(newPrice);
            if (seatingMap.find(key) != seatingMap.end())
                seatingMap[key].setCost(newPrice);
            cout << "Precio actualizado." << endl;
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return;
        }

        cout << "\nIngrese la nueva cantidad de filas: ";
        int newRows = readIntInRange(1, 1000);
        cout << "Ingrese la nueva cantidad de asientos por fila: ";
        int newSeats = readIntInRange(1, 1000);
        cout << "Ingrese el nuevo precio: ";
        float newPrice;
        while (!(cin >> newPrice) || newPrice <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Precio invalido, reingrese: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Segment& seg = segmentsByEvent.getAt(eventIndex).getAt(segmentIndex);
        seg.setRows(newRows);
        seg.setSeats(newSeats);
        seg.setPrice(newPrice);
        if (seatingMap.find(key) != seatingMap.end())
            seatingMap.erase(key);

        cout << "Segmento actualizado exitosamente." << endl;
        cout << "\nPresione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    if (action == 2) {
        if (seatsSold()) {
            cout << "\nNo se puede eliminar el segmento, ya tiene entradas vendidas." << endl;
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return;
        }

        segmentsByEvent.getAt(eventIndex).deletePosition(segmentIndex);
        int currentCount = segmentCount.getAt(eventIndex);
        segmentCount.getAt(eventIndex) = currentCount - 1;
        if (seatingMap.find(key) != seatingMap.end())
            seatingMap.erase(key);

        cout << "\nSegmento eliminado exitosamente." << endl;
        cout << "\nPresione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
}

void Menu::cancelPurchase()
{
    sale.cancelPurchase(user, event, segment, seatingMap);
}

void Menu::updateReport()
{
    using namespace std;
    system("cls");

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles para generar el reporte.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    string fileName = "reporte_eventos_actualizado.txt";

    // Opción 9: Actualizar reporte
    EventReport report;
    report.generateGlobalReport(event, segment, seatingMap, fileName);

    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Menu::generateCodesSFML(Discount& discount)
{
    // Reutilizamos la ventana principal
    sf::RenderWindow& win = *window;
    // Título de la ventana para este formulario
    win.setTitle("Generar Códigos de Descuento");

    // Cargar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // (Opcional) Un header de 100 px
    const float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    // Título principal
    sf::Text titleText("Generar Códigos de Descuento", font, 30);
    titleText.setFillColor(TEXT_COLOR);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((1200.f - titleBounds.width) / 2.f, headerHeight + 50.f);

    // Etiquetas para inputs
    sf::Text labelPercentage("Porcentaje de descuento:", font, 20);
    labelPercentage.setFillColor(TEXT_COLOR);
    labelPercentage.setPosition(300.f, 250.f);

    sf::Text labelCount("Cantidad de codigos:", font, 20);
    labelCount.setFillColor(TEXT_COLOR);
    labelCount.setPosition(300.f, 350.f);

    // Cadenas de entrada
    std::string percentageString;
    std::string countString;

    // Textos de entrada para mostrar lo que va digitando el usuario
    sf::Text percentageInputText("", font, 20);
    percentageInputText.setFillColor(TEXT_COLOR);
    percentageInputText.setPosition(300.f, 280.f);

    sf::Text countInputText("", font, 20);
    countInputText.setFillColor(TEXT_COLOR);
    countInputText.setPosition(300.f, 380.f);

    // Cajas de texto
    sf::RectangleShape percentageBox(sf::Vector2f(300.f, 30.f));
    percentageBox.setFillColor(sf::Color::White);
    percentageBox.setOutlineColor(sf::Color::Black);
    percentageBox.setOutlineThickness(1.f);
    percentageBox.setPosition(300.f, 280.f);

    sf::RectangleShape countBox(sf::Vector2f(300.f, 30.f));
    countBox.setFillColor(sf::Color::White);
    countBox.setOutlineColor(sf::Color::Black);
    countBox.setOutlineThickness(1.f);
    countBox.setPosition(300.f, 380.f);

    // Banderas de foco
    bool percentageActive = false;
    bool countActive = false;

    // Mensaje de error
    sf::Text errorMessage("", font, 20);
    errorMessage.setFillColor(sf::Color::Red);
    errorMessage.setPosition(300.f, 450.f);

    // Botón "Generar"
    sf::RectangleShape generateButton(sf::Vector2f(120.f, 40.f));
    generateButton.setFillColor(sf::Color(0, 180, 0));
    generateButton.setPosition(300.f, 500.f);

    sf::Text generateButtonText("Generar", font, 20);
    generateButtonText.setFillColor(sf::Color::White);
    {
        sf::FloatRect btnTextBounds = generateButtonText.getLocalBounds();
        float btnPosX = generateButton.getPosition().x
            + (generateButton.getSize().x - btnTextBounds.width) / 2.f
            - btnTextBounds.left;
        float btnPosY = generateButton.getPosition().y
            + (generateButton.getSize().y - btnTextBounds.height) / 2.f
            - btnTextBounds.top;
        generateButtonText.setPosition(btnPosX, btnPosY);
    }

    // Botón "Volver"
    sf::RectangleShape backButton(sf::Vector2f(120.f, 40.f));
    backButton.setFillColor(sf::Color(150, 150, 150));
    backButton.setPosition(450.f, 500.f);

    sf::Text backButtonText("Volver", font, 20);
    backButtonText.setFillColor(sf::Color::White);
    {
        sf::FloatRect btnTextBounds = backButtonText.getLocalBounds();
        float btnPosX = backButton.getPosition().x
            + (backButton.getSize().x - btnTextBounds.width) / 2.f
            - btnTextBounds.left;
        float btnPosY = backButton.getPosition().y
            + (backButton.getSize().y - btnTextBounds.height) / 2.f
            - btnTextBounds.top;
        backButtonText.setPosition(btnPosX, btnPosY);
    }

    // Área para mostrar los códigos generados
    sf::Text codesText("", font, 18);
    codesText.setFillColor(TEXT_COLOR);
    codesText.setPosition(650.f, 250.f);

    bool inGenerateScreen = true;
    while (inGenerateScreen && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
                return;
            }
            // Cambio de color si se usan sub-opciones? (Ya no hay submenú, se omite)
            // ...

            // Detectar clic sobre las cajas y los botones
            if (ev.type == sf::Event::MouseButtonPressed &&
                ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                // Foco en las cajas
                if (percentageBox.getGlobalBounds().contains(mousePos))
                {
                    percentageActive = true;
                    countActive = false;
                }
                else if (countBox.getGlobalBounds().contains(mousePos))
                {
                    percentageActive = false;
                    countActive = true;
                }
                else
                {
                    // Si no se clickeó dentro de las cajas, se desactivan
                    percentageActive = false;
                    countActive = false;
                }

                // Botón "Generar"
                if (generateButton.getGlobalBounds().contains(mousePos))
                {
                    errorMessage.setString("");
                    float percentageValue = 0.f;
                    if (!percentageString.empty())
                    {
                        try {
                            percentageValue = std::stof(percentageString);
                        }
                        catch (...) {
                            errorMessage.setString("Porcentaje inválido.");
                        }
                    }
                    else
                        errorMessage.setString("Ingrese un porcentaje.");

                    int countValue = 0;
                    if (errorMessage.getString().isEmpty())
                    {
                        if (!countString.empty())
                        {
                            try {
                                countValue = std::stoi(countString);
                                if (countValue <= 0)
                                    errorMessage.setString("Cantidad debe ser mayor a 0.");
                            }
                            catch (...)
                            {
                                errorMessage.setString("Cantidad inválida.");
                            }
                        }
                        else
                            errorMessage.setString("Ingrese cantidad.");
                    }

                    if (errorMessage.getString().isEmpty())
                    {
                        // Llamamos a discount.configure(...) con los datos
                        discount.configure(percentageValue, countValue);
                        // Se actualiza el área de texto con los códigos generados
                        codesText.setString(discount.getCodesString());
                        // Limpiar campos (si lo deseas)
                        percentageString.clear();
                        countString.clear();
                    }
                }

                // Botón "Volver"
                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    inGenerateScreen = false;
                }
            }

            // Manejo de texto en las cajas
            if (ev.type == sf::Event::TextEntered)
            {
                if (ev.text.unicode == 8) // Backspace
                {
                    if (percentageActive && !percentageString.empty())
                        percentageString.pop_back();
                    else if (countActive && !countString.empty())
                        countString.pop_back();
                }
                else if (ev.text.unicode >= 32 && ev.text.unicode < 127)
                {
                    // Carácter ASCII imprimible
                    char c = static_cast<char>(ev.text.unicode);
                    if (percentageActive)
                    {
                        if ((c >= '0' && c <= '9') || c == '.')
                            percentageString.push_back(c);
                    }
                    else if (countActive)
                    {
                        if (c >= '0' && c <= '9')
                            countString.push_back(c);
                    }
                }
            }
        }

        // Actualizar los textos de los inputs
        percentageInputText.setString(percentageString);
        countInputText.setString(countString);

        // Resaltar cajas activas
        percentageBox.setOutlineColor(percentageActive ? sf::Color::Blue : sf::Color::Black);
        countBox.setOutlineColor(countActive ? sf::Color::Blue : sf::Color::Black);

        // Render
        win.clear(BG_COLOR);
        win.draw(header);
        win.draw(titleText);

        // Etiquetas
        win.draw(labelPercentage);
        win.draw(labelCount);

        // Cajas de texto y su contenido
        win.draw(percentageBox);
        win.draw(countBox);
        win.draw(percentageInputText);
        win.draw(countInputText);

        // Mensaje de error
        win.draw(errorMessage);

        // Botón "Generar"
        win.draw(generateButton);
        win.draw(generateButtonText);

        // Botón "Volver"
        win.draw(backButton);
        win.draw(backButtonText);

        // Texto con la lista de códigos
        win.draw(codesText);

        win.display();
    }
    // Al salir, restablecemos título
    win.setTitle("Sistema de Ventas de Entradas");
}

void Menu::showDiscountCodesSFML(Discount& discount)
{
    // Reutilizamos la ventana principal
    sf::RenderWindow& win = *window;
    win.setTitle("Registro de Códigos de Descuento");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // Obtener la cadena con los códigos
    std::string codesStr = discount.getCodesString();

    sf::Text codesText(codesStr, font, 20);
    codesText.setFillColor(sf::Color::Black);
    codesText.setPosition(50.f, 150.f);

    // Botón "Volver"
    sf::RectangleShape backButton(sf::Vector2f(120.f, 40.f));
    backButton.setFillColor(sf::Color(150, 150, 150));
    backButton.setPosition(50.f, 50.f);

    sf::Text backText("Volver", font, 20);
    backText.setFillColor(sf::Color::White);
    {
        sf::FloatRect btnBounds = backText.getLocalBounds();
        float btnX = backButton.getPosition().x + (backButton.getSize().x - btnBounds.width) / 2.f - btnBounds.left;
        float btnY = backButton.getPosition().y + (backButton.getSize().y - btnBounds.height) / 2.f - btnBounds.top;
        backText.setPosition(btnX, btnY);
    }

    bool inShowScreen = true;
    while (inShowScreen && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed &&
                ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    inShowScreen = false;
                }
            }
        }
        win.clear(sf::Color(200, 200, 200));
        win.draw(backButton);
        win.draw(backText);
        win.draw(codesText);
        win.display();
    }
    // Restablecer el título al salir
    win.setTitle("Sistema de Ventas de Entradas");
}

void Menu::deleteDiscountSFML(Discount& discount)
{
    sf::RenderWindow& win = *window;
    win.setTitle("Eliminar Código de Descuento");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // Texto de instrucción
    sf::Text promptText("Digite el número del código a eliminar:", font, 20);
    promptText.setFillColor(sf::Color::Black);
    promptText.setPosition(50.f, 30.f);

    // Área de códigos
    std::string codesStr = discount.getCodesString();
    sf::Text codesText(codesStr, font, 18);
    codesText.setFillColor(sf::Color::Black);
    codesText.setPosition(50.f, 150.f);

    // Cuadro de entrada para el número
    sf::RectangleShape inputBox(sf::Vector2f(200.f, 30.f));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);
    inputBox.setPosition(50.f, 100.f);

    sf::Text inputText("", font, 20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(55.f, 105.f);

    std::string indexString;

    // Mensaje de error
    sf::Text errorMessage("", font, 18);
    errorMessage.setFillColor(sf::Color::Red);
    errorMessage.setPosition(50.f, 70.f);

    // Botón "Eliminar"
    sf::RectangleShape deleteButton(sf::Vector2f(120.f, 40.f));
    deleteButton.setFillColor(sf::Color(200, 0, 0));
    deleteButton.setPosition(300.f, 100.f);

    sf::Text deleteButtonText("Eliminar", font, 20);
    deleteButtonText.setFillColor(sf::Color::White);
    {
        sf::FloatRect btnTextBounds = deleteButtonText.getLocalBounds();
        float btnPosX = deleteButton.getPosition().x + (deleteButton.getSize().x - btnTextBounds.width) / 2.f - btnTextBounds.left;
        float btnPosY = deleteButton.getPosition().y + (deleteButton.getSize().y - btnTextBounds.height) / 2.f - btnTextBounds.top;
        deleteButtonText.setPosition(btnPosX, btnPosY);
    }

    // Botón "Volver"
    sf::RectangleShape backButton(sf::Vector2f(120.f, 40.f));
    backButton.setFillColor(sf::Color(150, 150, 150));
    backButton.setPosition(450.f, 100.f);

    sf::Text backButtonText("Volver", font, 20);
    backButtonText.setFillColor(sf::Color::White);
    {
        sf::FloatRect btnTextBounds = backButtonText.getLocalBounds();
        float btnPosX = backButton.getPosition().x + (backButton.getSize().x - btnTextBounds.width) / 2.f - btnTextBounds.left;
        float btnPosY = backButton.getPosition().y + (backButton.getSize().y - btnTextBounds.height) / 2.f - btnTextBounds.top;
        backButtonText.setPosition(btnPosX, btnPosY);
    }

    bool inDeleteScreen = true;
    bool inputActive = false;

    while (inDeleteScreen && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                if (inputBox.getGlobalBounds().contains(mousePos))
                {
                    inputActive = true;
                }
                else
                {
                    inputActive = false;
                }
                if (deleteButton.getGlobalBounds().contains(mousePos))
                {
                    errorMessage.setString("");
                    int index;
                    try {
                        index = std::stoi(indexString);
                    }
                    catch (...) {
                        errorMessage.setString("Ingrese un número válido.");
                        continue;
                    }
                    if (discount.deleteDiscountAtIndex(index))
                    {
                        codesText.setString(discount.getCodesString());
                        indexString.clear();
                        inputText.setString("");
                    }
                    else
                    {
                        errorMessage.setString("No se pudo eliminar el código.");
                    }
                }
                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    inDeleteScreen = false;
                }
            }
            if (ev.type == sf::Event::TextEntered)
            {
                if (inputActive)
                {
                    if (ev.text.unicode == 8)
                    {
                        if (!indexString.empty())
                            indexString.pop_back();
                    }
                    else if (ev.text.unicode >= 48 && ev.text.unicode <= 57)
                    {
                        char c = static_cast<char>(ev.text.unicode);
                        indexString.push_back(c);
                    }
                    inputText.setString(indexString);
                }
            }
        }
        win.clear(BG_COLOR);
        win.draw(promptText);
        win.draw(codesText);
        win.draw(inputBox);
        win.draw(inputText);
        win.draw(errorMessage);
        win.draw(deleteButton);
        win.draw(deleteButtonText);
        win.draw(backButton);
        win.draw(backButtonText);
        win.display();
    }
    // Al salir del submenú, restablecer el título principal
    win.setTitle("Sistema de Ventas de Entradas");
}

void Menu::sellTicketSFML()
{
    // Cambiar el título de la ventana
    window->setTitle("Vender Entrada - Sistema de Ventas");

    // Cargar fuente (asegúrate de que la ruta sea correcta)
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente." << std::endl;
        return;
    }

    // Variables para almacenar la entrada del usuario
    std::string cedula = "";
    int selectedEvent = -1;
    int selectedSegment = -1;
    std::string ticketCountStr = "";
    int ticketCount = 0;

    // Estado de la interfaz:
    // 0: Ingresar cédula
    // 1: Seleccionar evento
    // 2: Seleccionar segmento (del evento escogido)
    // 3: Ingresar cantidad de boletos
    // 4: Confirmación final
    int currentStep = 0;

    // Textos y elementos gráficos
    sf::Text promptText("", font, 24);
    promptText.setFillColor(sf::Color::Black);
    promptText.setPosition(50.f, 20.f);

    // Para mostrar opciones (eventos o segmentos) se almacenarán en un vector
    std::vector<sf::Text> optionsTexts;

    // Bucle principal de la venta
    bool saleCompleted = false;
    while (window->isOpen() && !saleCompleted)
    {
        sf::Event ev;
        while (window->pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window->close();
                return;
            }

            // Procesar entrada de texto para campos
            if (ev.type == sf::Event::TextEntered)
            {
                // Para evitar caracteres no imprimibles
                if (ev.text.unicode < 128)
                {
                    char c = static_cast<char>(ev.text.unicode);
                    if (currentStep == 0) // Ingreso de cédula
                    {
                        if (c == 8) // Backspace
                        {
                            if (!cedula.empty())
                                cedula.pop_back();
                        }
                        else if (std::isdigit(c))
                        {
                            cedula.push_back(c);
                        }
                    }
                    else if (currentStep == 3) // Ingreso de cantidad de boletos
                    {
                        if (c == 8) // Backspace
                        {
                            if (!ticketCountStr.empty())
                                ticketCountStr.pop_back();
                        }
                        else if (std::isdigit(c))
                        {
                            ticketCountStr.push_back(c);
                        }
                    }
                }
            }

            // Procesar pulsación de teclas
            if (ev.type == sf::Event::KeyPressed)
            {
                if (ev.key.code == sf::Keyboard::Enter)
                {
                    if (currentStep == 0)
                    {
                        // Si se ingresó una cédula (mínimo 9 dígitos)
                        if (cedula.size() >= 9)
                        {
                            currentStep = 1; // Pasar a selección de evento
                            optionsTexts.clear();
                            // Cargar los eventos disponibles:
                            int numEvents = event.getEventCount();
                            for (int i = 1; i <= numEvents; i++)
                            {
                                std::string evName = event.getEvents().getAt(i).getName();
                                sf::Text opt(evName, font, 24);
                                opt.setFillColor(sf::Color::Black);
                                opt.setPosition(50.f, 100.f + i * 40.f);
                                optionsTexts.push_back(opt);
                            }
                        }
                    }
                    else if (currentStep == 3)
                    {
                        if (!ticketCountStr.empty())
                        {
                            ticketCount = std::stoi(ticketCountStr);
                            currentStep = 4; // Confirmación final
                        }
                    }
                    else if (currentStep == 4)
                    {
                        // Se confirma la venta (en este ejemplo simplemente mostramos mensaje)
                        saleCompleted = true;
                    }
                }
            }

            // Procesar clics del mouse para seleccionar opciones
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                if (currentStep == 1) // Selección de evento
                {
                    for (size_t i = 0; i < optionsTexts.size(); i++)
                    {
                        if (optionsTexts[i].getGlobalBounds().contains(mousePos))
                        {
                            selectedEvent = static_cast<int>(i) + 1;
                            currentStep = 2;
                            optionsTexts.clear();
                            // Cargar segmentos del evento seleccionado
                            int segCount = segment.getSegmentCount().getAt(selectedEvent);
                            for (int j = 1; j <= segCount; j++)
                            {
                                std::string segName = segment.getSegmentsByEvent().getAt(selectedEvent).getAt(j).getName();
                                sf::Text opt(segName, font, 24);
                                opt.setFillColor(sf::Color::Black);
                                opt.setPosition(50.f, 100.f + j * 40.f);
                                optionsTexts.push_back(opt);
                            }
                            break;
                        }
                    }
                }
                else if (currentStep == 2) // Selección de segmento
                {
                    for (size_t i = 0; i < optionsTexts.size(); i++)
                    {
                        if (optionsTexts[i].getGlobalBounds().contains(mousePos))
                        {
                            selectedSegment = static_cast<int>(i) + 1;
                            currentStep = 3;
                            break;
                        }
                    }
                }
            }
        } // Fin del pollEvent

        // Renderizado de la pantalla según el paso
        window->clear(sf::Color(200, 200, 200));
        if (currentStep == 0)
        {
            promptText.setString("Ingrese su numero de cedula (9 o mas digitos):");
            window->draw(promptText);
            sf::Text inputText(cedula, font, 24);
            inputText.setFillColor(sf::Color::Black);
            inputText.setPosition(50.f, 80.f);
            window->draw(inputText);
        }
        else if (currentStep == 1)
        {
            promptText.setString("Seleccione un evento:");
            window->draw(promptText);
            for (auto& opt : optionsTexts)
            {
                window->draw(opt);
            }
        }
        else if (currentStep == 2)
        {
            promptText.setString("Seleccione un segmento:");
            window->draw(promptText);
            for (auto& opt : optionsTexts)
            {
                window->draw(opt);
            }
        }
        else if (currentStep == 3)
        {
            promptText.setString("Ingrese la cantidad de boletos a comprar:");
            window->draw(promptText);
            sf::Text ticketText(ticketCountStr, font, 24);
            ticketText.setFillColor(sf::Color::Black);
            ticketText.setPosition(50.f, 80.f);
            window->draw(ticketText);
        }
        else if (currentStep == 4)
        {
            // Pantalla de confirmación
            std::stringstream ss;
            ss << "Resumen de Venta:\n";
            ss << "Cedula: " << cedula << "\n";
            ss << "Evento: " << event.getEvents().getAt(selectedEvent).getName() << "\n";
            ss << "Segmento: " << segment.getSegmentsByEvent().getAt(selectedEvent).getAt(selectedSegment).getName() << "\n";
            ss << "Cantidad de boletos: " << ticketCount << "\n\n";
            ss << "Presione Enter para confirmar la venta.";
            promptText.setString(ss.str());
            window->draw(promptText);
        }
        window->display();
    } // Fin del while

    // Una vez confirmada la venta, se podría invocar la función de venta real (por ejemplo, sale.sell(...))
    // Aquí, por simplicidad, solo mostramos una pantalla de "Venta completada".
    window->clear(sf::Color(200, 200, 200));
    sf::Text successText("Venta completada con exito!", font, 30);
    successText.setFillColor(sf::Color::Green);
    successText.setPosition(50.f, 100.f);
    window->draw(successText);
    window->display();
    sf::sleep(sf::seconds(3));
}


void Menu::showFaqMenuSFML()
{
    // Reutilizamos la ventana principal
    sf::RenderWindow& win = *window;
    win.setTitle("Preguntas y Respuestas");

    // Cargar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente para el submenú de FAQ.\n";
        return;
    }

    // Definimos las 5 opciones
    const int NUM_OPTIONS = 5;
    std::string optionLabels[NUM_OPTIONS] = {
        "Reglas del evento.",
        "Como compro entradas?.",
        "Aplicar descuentos.",
        "Mostrar compras realizadas.",
        "Volver al menu."
    };

    // Creamos el "header" de 100 px
    float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    // Texto título del submenú
    sf::Text titleText("Preguntas y Respuestas", font, 30);
    titleText.setFillColor(TEXT_COLOR);
    // Centrar el texto en pantalla
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((1200.f - titleBounds.width) / 2.f, headerHeight + 50.f);

    // Creamos un array de textos (botones) para las opciones
    sf::Text options[NUM_OPTIONS];
    float spacing = 1200.f / NUM_OPTIONS;  // Para distribuir horizontalmente
    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        options[i].setFont(font);
        options[i].setString(optionLabels[i]);
        options[i].setCharacterSize(20);
        options[i].setFillColor(TEXT_COLOR);

        sf::FloatRect bounds = options[i].getLocalBounds();
        float posX = i * spacing + (spacing - bounds.width) / 2.f - bounds.left;
        float posY = (headerHeight - bounds.height) / 2.f - bounds.top;
        options[i].setPosition(posX, posY);
    }

    // Variable para controlar el ciclo del submenú
    bool inFaqMenu = true;

    // Variable para guardar qué contenido de FAQ se mostrará ( -1 = ninguno, 0..3 = secciones )
    int currentFaqIndex = -1;

    // Creamos un objeto Faq para obtener el contenido desde faq.cpp
    Faq faq;
    std::string faqContents[4] = {
        faq.getRulesContent(),       // Índice 0: Reglas del evento
        faq.getHowToBuyContent(),      // Índice 1: Cómo comprar entradas
        faq.getDiscountsContent(),     // Índice 2: Aplicacion de descuentos
        faq.getViewSalesContent()      // Índice 3: Cómo ver las ventas realizadas
    };

    // Texto que mostrará el contenido de la FAQ
    sf::Text faqContent("", font, 20);
    faqContent.setFillColor(TEXT_COLOR);
    faqContent.setPosition(100.f, 200.f);

    while (inFaqMenu && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
                return; // Cierra el programa
            }

            // Resaltar opciones al pasar el mouse
            if (ev.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseMove.x),
                    static_cast<float>(ev.mouseMove.y));
                for (int i = 0; i < NUM_OPTIONS; i++)
                {
                    if (options[i].getGlobalBounds().contains(mousePos))
                        options[i].setFillColor(HIGHLIGHT_COLOR);
                    else
                        options[i].setFillColor(TEXT_COLOR);
                }
            }

            // Clic con el mouse
            if (ev.type == sf::Event::MouseButtonPressed &&
                ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < NUM_OPTIONS; i++)
                {
                    if (options[i].getGlobalBounds().contains(mousePos))
                    {
                        // Si se selecciona "Volver al menú" (índice 4)
                        if (i == 4)
                        {
                            inFaqMenu = false;
                        }
                        else
                        {
                            // Se muestra el contenido correspondiente de la FAQ
                            currentFaqIndex = i;  // 0..3
                        }
                    }
                }
            }
        }

        // Actualizar el texto de la FAQ según la sección seleccionada
        if (currentFaqIndex >= 0 && currentFaqIndex < 4)
        {
            faqContent.setString(faqContents[currentFaqIndex]);
        }
        else
        {
            faqContent.setString("");
        }

        // Renderizado
        win.clear(BG_COLOR);
        // Dibujar el header
        win.draw(header);
        // Dibujar las opciones en la parte superior
        for (int i = 0; i < NUM_OPTIONS; i++)
            win.draw(options[i]);
        // Dibujar el título
        win.draw(titleText);
        // Dibujar el contenido de la FAQ
        win.draw(faqContent);

        win.display();
    }

    // Al salir, restablecemos el título de la ventana
    win.setTitle("Sistema de Ventas de Entradas");
}