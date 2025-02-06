#include <SFML/Graphics.hpp>
#include "Menu.h"
#include <SFML/Window.hpp>
#include <iostream>

// Alias para evitar conflictos entre las clases de SFML y las del proyecto
namespace sfml = sf;

static const sfml::Color BG_COLOR(200, 200, 200);    // Fondo gris
static const sfml::Color HEADER_COLOR(160, 160, 160);  // Encabezado gris medio
static const sfml::Color BOX_COLOR(230, 230, 230);     // Color de los botones (no usado en este ejemplo)
static const sfml::Color HIGHLIGHT_COLOR = sfml::Color(255, 0, 0); // Rojo para resaltar (no usado en este ejemplo)
static const sfml::Color TEXT_COLOR = sfml::Color::Black;          // Texto negro
void Menu::showMenu() {
    // Crear la ventana principal si aún no existe
    if (!window) {
        window = new sfml::RenderWindow(sfml::VideoMode(1200, 800), "Sistema de Ventas de Entradas", sfml::Style::Close);
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

    // Primera fila: área vertical [0, headerHeight/2]
    float row1Y_top = 0.f;
    float row1Height = headerHeight / 2.f; // 50 píxeles
    float row1Spacing = 1200.f / row1Count;
    for (int i = 0; i < row1Count; i++) {
        headerText[i].setFont(font);
        headerText[i].setCharacterSize(15);
        headerText[i].setFillColor(TEXT_COLOR);
        headerText[i].setString(optionLabels[i]);
        // Centrar horizontalmente en el segmento asignado
        sfml::FloatRect textBounds = headerText[i].getLocalBounds();
        float posX = i * row1Spacing + (row1Spacing - textBounds.width) / 2.f - textBounds.left;
        float posY = row1Y_top + (row1Height - textBounds.height) / 2.f - textBounds.top;
        headerText[i].setPosition(posX, posY);
    }

    // Segunda fila: área vertical [headerHeight/2, headerHeight]
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

    // Bucle principal de la ventana
    while (window->isOpen()) {
        sfml::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sfml::Event::Closed) {
                window->close();
                return;
            }
            // Resaltar opción al pasar el mouse
            if (event.type == sfml::Event::MouseMoved) {
                sfml::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y));
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
                sfml::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y));
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

// Ejecuta la acción correspondiente a cada opción (sin llamadas recursivas a showMenu)
void Menu::executeOption(int option) {
    switch (option) {
    case 1:
        // Configurar evento
        configureEvent(event, segment);
        break;
    case 2:
        // Configurar descuentos
        manageCodes(discount);
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
        showFaq();
        break;
    case 8:
        // Salir: cierra la ventana principal
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
int Menu::validateChoice(int& choice, int& size)
{
    while (true)
    {
        cout << "Seleccione una opcion: " << endl;
        if (cin >> choice && choice >= 1 && choice <= size)
        {
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida." << endl;
    }
}
void Menu::configureDiscounts(Discount& discount)
{
    discount.configureDiscounts();
}

void Menu::configureEvent(Event& event, Segment& segment) {
    // Se utiliza la ventana ya creada (asumimos que 'window' ya apunta a la ventana principal)
    sf::RenderWindow& win = *window;  // Alias para simplificar la sintaxis

    // Cargar la fuente (asegúrate de que la ruta sea correcta)
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    // Definir algunos colores
    const sf::Color BG_COLOR(200, 200, 200);       // Fondo gris claro
    const sf::Color HEADER_COLOR(160, 160, 160);     // Encabezado gris medio
    const sf::Color TEXT_COLOR = sf::Color::Black;   // Texto en negro
    const sf::Color HIGHLIGHT_COLOR = sf::Color::Red;  // Rojo para resaltar la opción

    // Crear el área del encabezado con una altura de 100 píxeles
    float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    // Definir las 4 opciones del menú de configuración (similares al menú de consola)
    const int numOptions = 4;
    std::string optionLabels[numOptions] = {
        "Agregar nuevo evento",       // Opción 1
        "Modificar segmentos",         // Opción 2
        "Mostrar eventos actuales",    // Opción 3
        "Inicio"                       // Opción 4: volver al menú principal
    };

    // Crear los objetos de texto para cada opción y centrarlos horizontalmente
    sf::Text options[numOptions];
    float spacing = 1200.f / numOptions; // Espacio asignado a cada opción
    for (int i = 0; i < numOptions; i++) {
        options[i].setFont(font);
        options[i].setString(optionLabels[i]);
        options[i].setCharacterSize(20);
        options[i].setFillColor(TEXT_COLOR);
        sf::FloatRect textBounds = options[i].getLocalBounds();
        float posX = i * spacing + (spacing - textBounds.width) / 2.f - textBounds.left;
        float posY = (headerHeight - textBounds.height) / 2.f - textBounds.top;
        options[i].setPosition(posX, posY);
    }

    // Texto de título para la pantalla de configuración
    sf::Text titleText("Configuracion de Evento", font, 30);
    titleText.setFillColor(TEXT_COLOR);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((1200.f - titleBounds.width) / 2.f, headerHeight + 50.f);

    // Bucle de la pantalla de configuración (se reutiliza la misma ventana)
    bool inConfigScreen = true;
    while (inConfigScreen) {
        sf::Event ev;
        while (win.pollEvent(ev)) {
            // Si se cierra la ventana, se propaga el cierre a toda la aplicación
            if (ev.type == sf::Event::Closed) {
                win.close();
                return;
            }
            // Resaltar la opción sobre la que se mueve el mouse
            if (ev.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseMove.x),
                    static_cast<float>(ev.mouseMove.y));
                for (int i = 0; i < numOptions; i++) {
                    if (options[i].getGlobalBounds().contains(mousePos))
                        options[i].setFillColor(HIGHLIGHT_COLOR);
                    else
                        options[i].setFillColor(TEXT_COLOR);
                }
            }
            // Detectar clic izquierdo sobre alguna opción
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                for (int i = 0; i < numOptions; i++) {
                    if (options[i].getGlobalBounds().contains(mousePos)) {
                        switch (i) {
                        case 0: // "Agregar nuevo evento"
                            saveEvent(event, segment);
                            seatingMap.clear();
                            break;
                        case 1: // "Modificar segmentos"
                            modifyOrDeleteSegment(*this, event, segment, seatingMap);
                            break;
                        case 2: // "Mostrar eventos actuales"
                            listEventAndSegments(event, segment);
                            break;
                        case 3: // "Inicio": salir de la pantalla de configuración
                            inConfigScreen = false;
                            break;
                        }
                    }
                }
            }
        }

        // Limpiar la ventana, dibujar los elementos de la pantalla de configuración y mostrarlos
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

void Menu::manageCodes(Discount& discount)
{
    while (true)
    {
        system("cls");
        int choice, size = 4;
        cout << "\n+=======================+" << endl;
        cout << "|     GESTION DE CODIGOS|" << endl;
        cout << "+=======================+" << endl;
        cout << "1. Generar codigos" << endl;
        cout << "2. Mostrar registro" << endl;
        cout << "3. Eliminar un codigo" << endl;
        cout << "4. Volver al menu principal" << endl;

        validateChoice(choice, size);

        if (choice == 1)
        {
            configureDiscounts(discount);
            continue;
        }

        if (choice == 2)
        {
            discount.showCodes();
            continue;
        }

        if (choice == 3)
        {
            discount.deleteDiscount();
            continue;
        }

        if (choice == 4)
        {
            return;
        }
    }
}

void Menu::sellTicket(User& user)
{
    sale.sell(user, event, segment, seatingMap, discount);
}



void Menu::saveEvent(Event& event, Segment& segment)
{

    event.saveEvent(event, segment);
}

void Menu::showAbout()
{
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

void Menu::showFaq()
{

    faq.showFaq();
}

int Menu::readIntInRange(int min, int max, const string& prompt) {
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
                    if (room.getSeatPurchased()[i][j])
                        return true;
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
    system("cls");

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles para generar el reporte.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

 
    string fileName = "reporte_eventos_actualizado.txt";

 
    EventReport report;
   
    report.generateGlobalReport(event, segment, seatingMap, fileName);

    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
