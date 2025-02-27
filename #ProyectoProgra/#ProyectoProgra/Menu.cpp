﻿#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Menu.h"
#include <iostream>

namespace sfml = sf;

#include "Menu.h"

static const sfml::Color BG_COLOR(200, 200, 200);      
static const sfml::Color HEADER_COLOR(70, 70, 70);  
static const sfml::Color BOX_COLOR(90, 90, 90);     
static const sfml::Color HIGHLIGHT_COLOR(255, 140, 0); 
static const sfml::Color TEXT_COLOR(sfml::Color::Black); 

void Menu::showMenu() {
    
    if (!window) {
     
        window = new sfml::RenderWindow(sfml::VideoMode(1200, 600), "Sistema de Ventas de Entradas", sfml::Style::Close);
        window->setKeyRepeatEnabled(true);  
    }

   
    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

   
    sfml::Texture logoTexture;
    if (!logoTexture.loadFromFile(R"(C:\Users\Ericka\Desktop\CURSOS\Programacion I Verano\trabajando con sfml\Proyecto_Progra-1\#ProyectoProgra\#ProyectoProgra\resourses\Logo-UNA-Rojo_FondoTransparente.png)")) {
        std::cerr << "Error: No se pudo cargar la imagen del logo.\n";

    }
    sfml::Sprite logoSprite;
    logoSprite.setTexture(logoTexture);
    
    logoSprite.setScale(0.2f, 0.2f);

    
    const float headerHeight = 100.f;
    sfml::RectangleShape header(sfml::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

  
    const int numOptions = 9;
    std::string optionLabels[numOptions] = {
        "Configurar Evento",
        "Configurar Descuentos",
        "Vender Entrada",
        "Consultar Ventas",
        "Cancelar Compra",
        "Acerca de",
        "Preguntas y Respuestas",
        "Salir",
        "Actualizar Reporte"
    };

    sfml::Text headerText[numOptions];

  
    const int row1Count = 5;
    const int row2Count = 4;
    float row1Spacing = 1200.f / row1Count;
    float row2Spacing = 1200.f / row2Count;

    for (int i = 0; i < numOptions; i++) {
        headerText[i].setFont(font);
        headerText[i].setCharacterSize(15);
        headerText[i].setFillColor(TEXT_COLOR);
        headerText[i].setString(optionLabels[i]);

        sfml::FloatRect textBounds = headerText[i].getLocalBounds();
        float posX, posY;
        if (i < row1Count) {
            posX = i * row1Spacing + (row1Spacing - textBounds.width) / 2.f - textBounds.left;
            posY = (headerHeight / 2.f - textBounds.height) / 2.f - textBounds.top;
        }
        else {
            posX = (i - row1Count) * row2Spacing + (row2Spacing - textBounds.width) / 2.f - textBounds.left;
            posY = (headerHeight / 2.f) + (headerHeight / 2.f - textBounds.height) / 2.f - textBounds.top;
        }
        headerText[i].setPosition(posX, posY);
    }

    
    sfml::Text welcomeText("Bienvenido\nSistema de ventas de entrada\nDesarrollado por:\nHarold Avila.\nCristian Cordero.", font, 30);
    welcomeText.setFillColor(TEXT_COLOR);
    welcomeText.setStyle(sfml::Text::Bold);
    welcomeText.setPosition(200.f, 200.f);
    welcomeText.setLineSpacing(1.5f);

    
    while (window->isOpen()) {
        sfml::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sfml::Event::Closed) {
                window->close();
                return;
            }
            
            if (event.type == sfml::Event::MouseMoved) {
                sfml::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
                for (int i = 0; i < numOptions; i++) {
                    if (headerText[i].getGlobalBounds().contains(mousePos))
                        headerText[i].setFillColor(HIGHLIGHT_COLOR);
                    else
                        headerText[i].setFillColor(TEXT_COLOR);
                }
            }
            
            if (event.type == sfml::Event::MouseButtonPressed &&
                event.mouseButton.button == sfml::Mouse::Left) {
                sfml::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                for (int i = 0; i < numOptions; i++) {
                    if (headerText[i].getGlobalBounds().contains(mousePos)) {
                        executeOption(i + 1, *window);
                        break;
                    }
                }
            }
        }

        sf::Vector2u winSize = window->getSize();
        sf::FloatRect logoGlobalBounds = logoSprite.getGlobalBounds();
        logoSprite.setPosition(winSize.x - logoGlobalBounds.width - 10, winSize.y - logoGlobalBounds.height - 30);

        window->clear(BG_COLOR);
        window->draw(header);
        for (int i = 0; i < numOptions; i++) {
            window->draw(headerText[i]);
        }
        window->draw(welcomeText);
        
        window->draw(logoSprite);
        window->display();
    }
}

void Menu::executeOption(int option, sfml::RenderWindow& window) {
   
    switch (option) {
    case 1:
      
        configureEvent(event, segment);
        break;
    case 2:
     
        showDiscountMenuSFML(discount);
        break;
    case 3:
        
        sellTicket(user, window);  
        break;
    case 4:
        
        seating.checkSales(event, segment, seatingMap,window);
        break;
    case 5:
        
        sale.cancelPurchase(user, event, segment, seatingMap, window);
        break;
    case 6:
     
        showAbout();
        break;
    case 7:
       
        showFaqMenuSFML();
        break;
    case 8:
        window.close();
        break;
    case 9:
        updateReport();
        break;
    default:
        std::cout << "Opción inválida. Intente nuevamente.\n";
    }
}

void Menu::showDiscountMenuSFML(Discount& discount)
{
   
    sf::RenderWindow& win = *window;
    
    win.setTitle("Gestión de Descuentos");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente para el menú de descuentos.\n";
        return;
    }

  
    const float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    
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
                        case 0: 
                            generateCodesSFML(discount);
                            break;
                        case 1: 
                            showDiscountCodesSFML(discount);
                            break;
                        case 2: 
                            deleteDiscountSFML(discount);
                            break;
                        case 3: 
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
  
    win.setTitle("Sistema de Ventas de Entradas");
}

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

void Menu::configureEvent(Event& event, Segment& segment) {
    sfml::RenderWindow& win = *window;  

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
                            
                            saveEvent(event, segment);
                            seatingMap.clear();
                            break;
                        case 1:
                            
                            modifyOrDeleteSegment(*this, event, segment, seatingMap);
                            break;
                        case 2:
                           
                            listEventAndSegments(event, segment);
                            break;
                        case 3:
                            
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
    
    sf::RenderWindow& win = *window;
    win.setTitle("Lista de Eventos y Segmentos");

    
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente en listEventAndSegments.\n";
        return;
    }

    
    float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    
    sf::Text headerTitle("Eventos y Segmentos", font, 25);
    headerTitle.setFillColor(TEXT_COLOR);
    {
        sf::FloatRect bounds = headerTitle.getLocalBounds();
        float posX = 20.f;
        float posY = (headerHeight - bounds.height) / 2.f - bounds.top;
        headerTitle.setPosition(posX, posY);
    }

    
    sf::RectangleShape backButton(sf::Vector2f(120.f, 40.f));
    backButton.setFillColor(sf::Color(150, 150, 150));
    backButton.setPosition(50.f, headerHeight + 20.f); 

    sf::Text backButtonText("Volver", font, 20);
    backButtonText.setFillColor(sf::Color::White);
    {
        sf::FloatRect btnBounds = backButtonText.getLocalBounds();
        float btnX = backButton.getPosition().x + (backButton.getSize().x - btnBounds.width) / 2.f - btnBounds.left;
        float btnY = backButton.getPosition().y + (backButton.getSize().y - btnBounds.height) / 2.f - btnBounds.top;
        backButtonText.setPosition(btnX, btnY);
    }

    
    std::string info;
    if (event.getEventCount() == 0)
    {
        info = "No hay eventos guardados.\n\nPresione \"Volver\" para regresar...";
    }
    else
    {
        List<List<Segment>>& segmentsByEvent = segment.getSegmentsByEvent();
        List<int>& segmentCounts = segment.getSegmentCount();

        info += "Lista de eventos y segmentos:\n\n";
        for (int i = 1; i <= event.getEventCount(); i++)
        {
            info += "Evento #" + std::to_string(i) + ":\n";
            info += "  Nombre: " + event.getEvents().getAt(i).getName() + "\n";
            info += "  Fecha: " + event.getEvents().getAt(i).getDate() + "\n";
            info += "  Descripcion: " + event.getEvents().getAt(i).getDescription() + "\n";
            info += "  Segmentos asociados:\n";

            if (segmentCounts.getAt(i) == 0 || segmentsByEvent.getAt(i).getHead() == nullptr)
            {
                info += "    - No tiene segmentos asignados.\n";
                info += "--------------------------\n\n";
                continue;
            }

            for (int j = 1; j <= segmentCounts.getAt(i); j++)
            {
                Segment seg = segmentsByEvent.getAt(i).getAt(j);
                info += "    Segmento #" + std::to_string(j) + ":\n";
                info += "      Nombre: " + seg.getName() + "\n";
                info += "      Filas: " + std::to_string(seg.getRows()) + "\n";
                info += "      Asientos por fila: " + std::to_string(seg.getSeats()) + "\n";
                std::ostringstream precioStream;
                precioStream << std::fixed << std::setprecision(2) << seg.getPrice();
                info += "      Precio: $" + precioStream.str() + "\n";
            }
            info += "--------------------------\n\n";
        }
        info += "\n";
    }

    
    float infoStartY = headerHeight + 80.f;
    float availableHeight = static_cast<float>(win.getSize().y) - infoStartY;

    
    sf::Text infoText(info, font, 18);
    infoText.setFillColor(TEXT_COLOR);
    infoText.setPosition(50.f, 0.f); 

    // Variables para el desplazamiento vertical
    float scrollOffset = 0.f;
    const float scrollSpeed = 20.f;
    float textHeight = infoText.getLocalBounds().height;

    
    float bottomMargin = 20.f;
    float minOffset = (textHeight > availableHeight) ? (availableHeight - textHeight + bottomMargin) : 0.f;

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
            
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                    static_cast<float>(ev.mouseButton.y));
                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    inShowScreen = false;
                }
            }
            
            if (ev.type == sf::Event::MouseWheelScrolled)
            {
                scrollOffset -= ev.mouseWheelScroll.delta * scrollSpeed;
                if (scrollOffset < minOffset)
                    scrollOffset = minOffset;
                if (scrollOffset > 0)
                    scrollOffset = 0;
            }
        }

        win.clear(BG_COLOR);
        
        win.setView(win.getDefaultView());
        win.draw(header);
        win.draw(headerTitle);
        win.draw(backButton);
        win.draw(backButtonText);

        
        sf::View infoView(sf::FloatRect(0, 0, static_cast<float>(win.getSize().x), availableHeight));
        infoView.setViewport(sf::FloatRect(0.f, infoStartY / win.getSize().y, 1.f, availableHeight / win.getSize().y));
        infoView.setCenter(win.getSize().x / 2.f, availableHeight / 2.f - scrollOffset);
        win.setView(infoView);
        win.draw(infoText);

        win.setView(win.getDefaultView());
        win.display();
    }

    
    win.setTitle("Sistema de Ventas de Entradas");
}

void Menu::sellTicket(User& user, sfml::RenderWindow& window)
{
    
    sale.sell(user, event, segment, seatingMap, discount, window);
}

void Menu::saveEvent(Event& event, Segment& segment)
{
    sfml::RenderWindow& win = *window;
    event.saveEvent(win, segment);
}

void Menu::showAbout() {
    if (!window)
        return;

    window->setTitle("Acerca de - Sistema de Ventas de Entradas");

  
    Music moratMusic;
    std::string musicPath = "C:/Users/Ericka/Desktop/CURSOS/Programacion I Verano/trabajando con sfml/Proyecto_Progra-1/#ProyectoProgra/#ProyectoProgra/resourses/umbrella.ogg";
    if (!moratMusic.loadMusic(musicPath)) {
        std::cerr << "Error al cargar la canción de Morat." << std::endl;
        return;
    }
    
    moratMusic.play();

    // Cargar la fuente
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente." << std::endl;
        return;
    }

    
    sf::RectangleShape header(sf::Vector2f(window->getSize().x, 100.f));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    
    sf::Text titleText("ACERCA DE", font, 40);
    titleText.setFillColor(TEXT_COLOR);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((window->getSize().x - titleBounds.width) / 2.f,
        (100.f - titleBounds.height) / 2.f);

    
    std::string aboutInfo =
        "Integrantes del Proyecto:\n\n"
        "  - Harold Avila Hernandez.\n"
        "  - Cristhian Cordero Varela.\n\n"
        "Presione cualquier tecla o haga clic para volver al menú.";
    sf::Text aboutText(aboutInfo, font, 30);
    aboutText.setFillColor(TEXT_COLOR);
    aboutText.setPosition(100.f, 150.f);

    // Bucle de la ventana "Acerca de"
    bool exitAbout = false;
    while (window->isOpen() && !exitAbout) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                return;
            }
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                exitAbout = true;
            }
        }

        
        window->clear(BG_COLOR);
        window->draw(header);
        window->draw(titleText);
        window->draw(aboutText);
        window->display();
    }

   
    window->setTitle("Sistema de Ventas de Entradas");
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

    sf::RenderWindow& win = *window;
    win.setTitle("Modificar o Eliminar Segmentos");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error al cargar la fuente en modifyOrDeleteSegment.\n";
        return;
    }

    sf::RectangleShape backButton(sf::Vector2f(120.f, 40.f));
    backButton.setFillColor(sf::Color(150, 150, 150));
  
    backButton.setPosition(1200.f - 140.f, 20.f);

    sf::Text backButtonText("Volver", font, 20);
    backButtonText.setFillColor(sf::Color::White);
    {
        sf::FloatRect btnB = backButtonText.getLocalBounds();
        float btnX = backButton.getPosition().x + (backButton.getSize().x - btnB.width) / 2.f - btnB.left;
        float btnY = backButton.getPosition().y + (backButton.getSize().y - btnB.height) / 2.f - btnB.top;
        backButtonText.setPosition(btnX, btnY);
    }


    float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    sf::Text headerTitle("Seleccionar Evento a Modificar/Eliminar Segmentos", font, 25);
    headerTitle.setFillColor(TEXT_COLOR);
    {
        sf::FloatRect b = headerTitle.getLocalBounds();
        float posX = 20.f;
        float posY = (headerHeight - b.height) / 2.f - b.top;
        headerTitle.setPosition(posX, posY);
    }
    // Verificar si hay eventos antes de continuar
    if (event.getEventCount() == 0) {
        sf::Text noEventsText("No hay eventos disponibles.", font, 24);
        noEventsText.setFillColor(sf::Color::Black);
        noEventsText.setPosition(50.f, headerHeight + 50.f);

        sf::Clock clock; // Reloj para contar el tiempo
        while (clock.getElapsedTime().asSeconds() < 1.0f && win.isOpen()) { 
            sf::Event ev;
            while (win.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) {
                    win.close();
                    return;
                }
            }
            win.clear(BG_COLOR);
            win.draw(header);
            win.draw(headerTitle);
            win.draw(noEventsText);
            win.display();
        }
        return; // Salir de la función después de 1 segundo
    }


    sf::Text chooseEventText("Elige un evento:", font, 20);
    chooseEventText.setFillColor(sf::Color::Black);
    chooseEventText.setPosition(50.f, headerHeight + 50.f);

   
    std::vector<sf::Text> eventTexts;
    {
        float eventStartY = headerHeight + 100.f;
        for (int i = 1; i <= event.getEventCount(); i++)
        {
            std::string evtName = std::to_string(i) + ". " + event.getEvents().getAt(i).getName();
            sf::Text t(evtName, font, 20);
            t.setFillColor(sf::Color::Black);
            float x = 50.f;
            float y = eventStartY + (i - 1) * 50.f;
            t.setPosition(x, y);
            eventTexts.push_back(t);
        }
    }

    bool pickingEvent = true;
    int chosenEventIndex = -1;
    while (pickingEvent && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseMove.x), static_cast<float>(ev.mouseMove.y));
                for (auto& t : eventTexts)
                {
                    if (t.getGlobalBounds().contains(mousePos))
                        t.setFillColor(HIGHLIGHT_COLOR);
                    else
                        t.setFillColor(sf::Color::Black);
                }
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x), static_cast<float>(ev.mouseButton.y));
                
                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    return;
                }
                for (int i = 0; i < (int)eventTexts.size(); i++)
                {
                    if (eventTexts[i].getGlobalBounds().contains(mousePos))
                    {
                        chosenEventIndex = i + 1;
                        pickingEvent = false;
                        break;
                    }
                }
            }
        }
        win.clear(BG_COLOR);
        win.draw(header);
        win.draw(headerTitle);
        win.draw(chooseEventText);
        // Dibujar el botón "Volver" en la parte superior
        win.draw(backButton);
        win.draw(backButtonText);
        for (auto& t : eventTexts)
            win.draw(t);
        win.display();
    }

    if (chosenEventIndex < 1) {
        return;
    }

    
    List<List<Segment>>& segmentsByEvent = segment.getSegmentsByEvent();
    List<int>& segmentCount = segment.getSegmentCount();
    int numSegments = segmentCount.getAt(chosenEventIndex);

    if (numSegments == 0)
    {
        sf::Text noSegmentsText("El evento seleccionado no tiene segmentos.\nDesea registrar un nuevo segmento?\n(Click en Si/No)",
            font, 20);
        noSegmentsText.setFillColor(sf::Color::Black);
        noSegmentsText.setPosition(50.f, 200.f);

        sf::RectangleShape yesButton(sf::Vector2f(60.f, 40.f));
        yesButton.setFillColor(sf::Color(0, 180, 0));
        yesButton.setPosition(50.f, 300.f);

        sf::Text yesText("Si", font, 20);
        yesText.setFillColor(sf::Color::White);
        {
            sf::FloatRect b = yesText.getLocalBounds();
            float x = yesButton.getPosition().x + (yesButton.getSize().x - b.width) / 2.f - b.left;
            float y = yesButton.getPosition().y + (yesButton.getSize().y - b.height) / 2.f - b.top;
            yesText.setPosition(x, y);
        }

        sf::RectangleShape noButton(sf::Vector2f(60.f, 40.f));
        noButton.setFillColor(sf::Color(150, 150, 150));
        noButton.setPosition(120.f, 300.f);

        sf::Text noText("No", font, 20);
        noText.setFillColor(sf::Color::White);
        {
            sf::FloatRect b = noText.getLocalBounds();
            float x = noButton.getPosition().x + (noButton.getSize().x - b.width) / 2.f - b.left;
            float y = noButton.getPosition().y + (noButton.getSize().y - b.height) / 2.f - b.top;
            noText.setPosition(x, y);
        }

        bool waitingResponse = true;
        while (waitingResponse && win.isOpen())
        {
            sf::Event ev;
            while (win.pollEvent(ev))
            {
                if (ev.type == sf::Event::Closed) {
                    win.close();
                    return;
                }
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);
                    if (yesButton.getGlobalBounds().contains(mousePos)) {
                        segment.saveSegments(win, segment, event.getEventCount(), chosenEventIndex);
                        waitingResponse = false;
                    }
                    else if (noButton.getGlobalBounds().contains(mousePos)) {
                        waitingResponse = false;
                    }
                }
            }
            win.clear(BG_COLOR);
            win.draw(header);
            win.draw(headerTitle);
            win.draw(noSegmentsText);
            win.draw(yesButton);
            win.draw(yesText);
            win.draw(noButton);
            win.draw(noText);
            
            win.draw(backButton);
            win.draw(backButtonText);
            win.display();
        }
        return;
    }

   
    sf::Text chooseSegmentText("Segmentos para el evento " + std::to_string(chosenEventIndex) +
        ":\nElige uno para Modificar/Eliminar", font, 20);
    chooseSegmentText.setFillColor(sf::Color::Black);
    chooseSegmentText.setPosition(50.f, 110.f);

    std::vector<sf::Text> segmentTexts;
    {
        float segmentStartY = chooseSegmentText.getPosition().y + chooseSegmentText.getLocalBounds().height + 20.f;
        for (int j = 1; j <= numSegments; j++)
        {
            Segment seg = segmentsByEvent.getAt(chosenEventIndex).getAt(j);
            std::string segStr = std::to_string(j) + ". " + seg.getName() +
                "  (Filas:" + std::to_string(seg.getRows()) +
                ", Asientos:" + std::to_string(seg.getSeats()) +
                ", Precio:" + std::to_string(seg.getPrice()) + ")";
            sf::Text t(segStr, font, 20);
            t.setFillColor(sf::Color::Black);
            t.setPosition(50.f, segmentStartY + (j - 1) * 50.f);
            segmentTexts.push_back(t);
        }
    }

    bool pickingSegment = true;
    int chosenSegmentIndex = -1;
    while (pickingSegment && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseMoved)
            {
                sf::Vector2f m(ev.mouseMove.x, ev.mouseMove.y);
                for (auto& txt : segmentTexts)
                {
                    if (txt.getGlobalBounds().contains(m))
                        txt.setFillColor(HIGHLIGHT_COLOR);
                    else
                        txt.setFillColor(sf::Color::Black);
                }
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);
                if (backButton.getGlobalBounds().contains(m)) {
                    return;
                }
                for (int i = 0; i < (int)segmentTexts.size(); i++)
                {
                    if (segmentTexts[i].getGlobalBounds().contains(m))
                    {
                        chosenSegmentIndex = i + 1;
                        pickingSegment = false;
                        break;
                    }
                }
            }
        }
        win.clear(BG_COLOR);
        win.draw(header);
        win.draw(headerTitle);
        win.draw(chooseSegmentText);
       
        win.draw(backButton);
        win.draw(backButtonText);
        for (auto& txt : segmentTexts)
        {
            win.draw(txt);
        }
        win.display();
    }
    if (chosenSegmentIndex < 1) {
        return;
    }

    
    std::tuple<int, int> key = std::make_tuple(chosenEventIndex, chosenSegmentIndex);
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

    bool seatsSoldFlag = seatsSold();
    sf::Text actionPrompt;
    actionPrompt.setFont(font);
    actionPrompt.setCharacterSize(20);
    actionPrompt.setFillColor(sf::Color::Black);
    actionPrompt.setPosition(50.f, 420.f);
    if (seatsSoldFlag)
        actionPrompt.setString("El segmento tiene entradas vendidas,\nsolo puede actualizar el precio.");
    else
        actionPrompt.setString("Seleccione la accion a realizar\nEvento: " + std::to_string(chosenEventIndex) +
            ", Segmento: " + std::to_string(chosenSegmentIndex));

    
    sf::RectangleShape updateButton(sf::Vector2f(150.f, 40.f));
    updateButton.setFillColor(sf::Color(0, 180, 0));
    updateButton.setPosition(50.f, 480.f);

    sf::Text updateText("Actualizar", font, 20);
    updateText.setFillColor(sf::Color::White);
    {
        sf::FloatRect b = updateText.getLocalBounds();
        float x = updateButton.getPosition().x + (updateButton.getSize().x - b.width) / 2.f - b.left;
        float y = updateButton.getPosition().y + (updateButton.getSize().y - b.height) / 2.f - b.top;
        updateText.setPosition(x, y);
    }

    // Sólo se muestra la opción "Eliminar" si NO hay asientos vendidos.
    sf::RectangleShape deleteButton(sf::Vector2f(150.f, 40.f));
    sf::Text deleteText("Eliminar", font, 20);
    if (!seatsSoldFlag)
    {
        deleteButton.setFillColor(sf::Color(180, 0, 0));
        deleteButton.setPosition(250.f, 480.f);

        deleteText.setFillColor(sf::Color::White);
        {
            sf::FloatRect b = deleteText.getLocalBounds();
            float x = deleteButton.getPosition().x + (deleteButton.getSize().x - b.width) / 2.f - b.left;
            float y = deleteButton.getPosition().y + (deleteButton.getSize().y - b.height) / 2.f - b.top;
            deleteText.setPosition(x, y);
        }
    }

    bool pickingAction = true;
    int actionChosen = -1; 
    while (pickingAction && win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed) {
                win.close();
                return;
            }
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);
                // Si se hace clic en el botón "Volver" (siempre visible en la parte superior)
                if (backButton.getGlobalBounds().contains(m)) {
                    return;
                }
                if (updateButton.getGlobalBounds().contains(m))
                {
                    actionChosen = 1;
                    pickingAction = false;
                }
                // Solo procesamos la opción "Eliminar" si no hay entradas vendidas.
                else if (!seatsSoldFlag && deleteButton.getGlobalBounds().contains(m))
                {
                    actionChosen = 2;
                    pickingAction = false;
                }
            }
        }
        win.clear(BG_COLOR);
        win.draw(header);
        win.draw(headerTitle);
        win.draw(actionPrompt);
        win.draw(updateButton);
        win.draw(updateText);
        // Dibujar "Eliminar" solo si es posible
        if (!seatsSoldFlag)
        {
            win.draw(deleteButton);
            win.draw(deleteText);
        }
        // Dibujar el botón "Volver" superior
        win.draw(backButton);
        win.draw(backButtonText);
        win.display();
    }
    if (actionChosen < 1) {
        return;
    }

    
    Segment& segRef = segmentsByEvent.getAt(chosenEventIndex).getAt(chosenSegmentIndex);

   
    if (actionChosen == 1)
    {
        if (seatsSoldFlag)
        {
            // Si hay entradas vendidas, solo se permite actualizar el precio.
            float newPrice = 0.0f;
            bool inputDone = false;
            std::string priceStr;
            sf::RectangleShape priceBox(sf::Vector2f(200.f, 30.f));
            priceBox.setFillColor(sf::Color::White);
            priceBox.setOutlineColor(sf::Color::Black);
            priceBox.setOutlineThickness(1.f);
            priceBox.setPosition(50.f, 180.f);  // Posición Y para no tapar el mensaje

            sf::Text pricePrompt("El segmento tiene entradas vendidas;\nsolo puede actualizar el precio.\nDigite el nuevo precio y Enter:",
                font, 20);
            pricePrompt.setFillColor(sf::Color::Black);
            pricePrompt.setPosition(50.f, 110.f);

            sf::Text priceInput("", font, 20);
            priceInput.setFillColor(sf::Color::Black);
            priceInput.setPosition(priceBox.getPosition().x + 5, priceBox.getPosition().y + 2);

            while (!inputDone && win.isOpen())
            {
                sf::Event e;
                while (win.pollEvent(e))
                {
                    if (e.type == sf::Event::Closed) {
                        win.close();
                        return;
                    }
                    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
                    {
                        try {
                            float val = std::stof(priceStr);
                            if (val > 0) {
                                newPrice = val;
                                inputDone = true;
                            }
                        }
                        catch (...) {}
                    }
                    else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Backspace)
                    {
                        if (!priceStr.empty()) {
                            priceStr.pop_back();
                            priceInput.setString(priceStr);
                        }
                    }
                    else if (e.type == sf::Event::TextEntered)
                    {
                        if (e.text.unicode < 128)
                        {
                            char c = static_cast<char>(e.text.unicode);
                            if ((isdigit(c) || c == '.') && priceStr.size() < 10) {
                                priceStr.push_back(c);
                                priceInput.setString(priceStr);
                            }
                        }
                    }
                }
                win.clear(BG_COLOR);
                win.draw(header);
                win.draw(headerTitle);
                win.draw(pricePrompt);
                win.draw(priceBox);
                win.draw(priceInput);
                // Dibujar el botón "Volver" en la parte superior
                win.draw(backButton);
                win.draw(backButtonText);
                win.display();
            }

            if (newPrice > 0)
            {
                segRef.setPrice(newPrice);
                if (seatingMap.find(key) != seatingMap.end())
                    seatingMap[key].setCost(newPrice);

                sf::Text okText("Precio actualizado exitosamente.\nClick 'Volver' para salir.",
                    font, 20);
                okText.setFillColor(sf::Color::Black);
                okText.setPosition(50.f, 220.f);

                bool waitOk = true;
                while (waitOk && win.isOpen())
                {
                    sf::Event e;
                    while (win.pollEvent(e))
                    {
                        if (e.type == sf::Event::Closed) {
                            win.close();
                            return;
                        }
                        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                        {
                            sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                            if (backButton.getGlobalBounds().contains(m)) {
                                waitOk = false;
                            }
                        }
                    }
                    win.clear(BG_COLOR);
                    win.draw(header);
                    win.draw(headerTitle);
                    win.draw(okText);
                    // Dibujar el botón "Volver" en la parte superior
                    win.draw(backButton);
                    win.draw(backButtonText);
                    win.display();
                }
            }
            return;
        }
        else
        {
            // Si no hay entradas vendidas, se permite actualizar filas, asientos y precio.
            bool doneEditing = false;
            std::string rowStr, seatsStr, priceStr;
            sf::Text rowPrompt("Nueva cantidad de filas:", font, 20);
            rowPrompt.setFillColor(sf::Color::Black);
            rowPrompt.setPosition(50.f, 110.f);

            sf::RectangleShape rowBox(sf::Vector2f(100.f, 30.f));
            rowBox.setFillColor(sf::Color::White);
            rowBox.setOutlineColor(sf::Color::Black);
            rowBox.setOutlineThickness(1.f);
            rowBox.setPosition(50.f, 140.f);

            sf::Text rowInput("", font, 20);
            rowInput.setFillColor(sf::Color::Black);
            rowInput.setPosition(55.f, 145.f);

            sf::Text seatsPrompt("Nueva cantidad de asientos:", font, 20);
            seatsPrompt.setFillColor(sf::Color::Black);
            seatsPrompt.setPosition(50.f, 190.f);

            sf::RectangleShape seatsBox(sf::Vector2f(100.f, 30.f));
            seatsBox.setFillColor(sf::Color::White);
            seatsBox.setOutlineColor(sf::Color::Black);
            seatsBox.setOutlineThickness(1.f);
            seatsBox.setPosition(50.f, 220.f);

            sf::Text seatsInput("", font, 20);
            seatsInput.setFillColor(sf::Color::Black);
            seatsInput.setPosition(55.f, 225.f);

            sf::Text pricePrompt("Nuevo precio:", font, 20);
            pricePrompt.setFillColor(sf::Color::Black);
            pricePrompt.setPosition(50.f, 270.f);

            sf::RectangleShape priceBox(sf::Vector2f(100.f, 30.f));
            priceBox.setFillColor(sf::Color::White);
            priceBox.setOutlineColor(sf::Color::Black);
            priceBox.setOutlineThickness(1.f);
            priceBox.setPosition(50.f, 300.f);

            sf::Text priceInput("", font, 20);
            priceInput.setFillColor(sf::Color::Black);
            priceInput.setPosition(55.f, 305.f);

            sf::Text instruct("Presione Enter para guardar.", font, 18);
            instruct.setFillColor(sf::Color::Black);
            instruct.setPosition(50.f, 350.f);

            int activeField = 0; // 0=rows, 1=seats, 2=price

            sf::RectangleShape activeBorder;
            activeBorder.setFillColor(sf::Color::Transparent);
            activeBorder.setOutlineColor(HIGHLIGHT_COLOR);
            activeBorder.setOutlineThickness(2.f);
            activeBorder.setPosition(rowBox.getPosition());
            activeBorder.setSize(rowBox.getSize());

            while (!doneEditing && win.isOpen())
            {
                sf::Event e;
                while (win.pollEvent(e))
                {
                    if (e.type == sf::Event::Closed) {
                        win.close();
                        return;
                    }
                    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                        if (backButton.getGlobalBounds().contains(m)) {
                            return;
                        }
                        if (rowBox.getGlobalBounds().contains(m))   activeField = 0;
                        if (seatsBox.getGlobalBounds().contains(m)) activeField = 1;
                        if (priceBox.getGlobalBounds().contains(m)) activeField = 2;
                    }
                    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab)
                    {
                        activeField = (activeField + 1) % 3;
                    }
                    else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
                    {
                        try {
                            int newRowsVal = std::stoi(rowStr);
                            int newSeatsVal = std::stoi(seatsStr);
                            float newPriceVal = std::stof(priceStr);
                            if (newRowsVal > 0 && newSeatsVal > 0 && newPriceVal > 0)
                            {
                                segRef.setRows(newRowsVal);
                                segRef.setSeats(newSeatsVal);
                                segRef.setPrice(newPriceVal);
                                if (seatingMap.find(key) != seatingMap.end())
                                    seatingMap.erase(key);

                                sf::Text doneText("Segmento actualizado exitosamente.\nClick 'Volver' para salir.",
                                    font, 20);
                                doneText.setFillColor(sf::Color::Black);
                                doneText.setPosition(50.f, 400.f);

                                bool waitOk = true;
                                while (waitOk && win.isOpen())
                                {
                                    sf::Event e2;
                                    while (win.pollEvent(e2))
                                    {
                                        if (e2.type == sf::Event::Closed) {
                                            win.close();
                                            return;
                                        }
                                        if (e2.type == sf::Event::MouseButtonPressed && e2.mouseButton.button == sf::Mouse::Left)
                                        {
                                            sf::Vector2f mm(e2.mouseButton.x, e2.mouseButton.y);
                                            if (backButton.getGlobalBounds().contains(mm)) {
                                                waitOk = false;
                                            }
                                        }
                                    }
                                    win.clear(BG_COLOR);
                                    win.draw(header);
                                    win.draw(headerTitle);
                                    win.draw(rowPrompt);
                                    win.draw(rowBox);
                                    win.draw(rowInput);
                                    win.draw(seatsPrompt);
                                    win.draw(seatsBox);
                                    win.draw(seatsInput);
                                    win.draw(pricePrompt);
                                    win.draw(priceBox);
                                    win.draw(priceInput);
                                    win.draw(instruct);
                                    win.draw(doneText);
                                    
                                    win.draw(backButton);
                                    win.draw(backButtonText);
                                    win.display();
                                }
                                return;
                            }
                        }
                        catch (...) {}
                    }
                    else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Backspace)
                    {
                        if (activeField == 0 && !rowStr.empty()) {
                            rowStr.pop_back();
                            rowInput.setString(rowStr);
                        }
                        else if (activeField == 1 && !seatsStr.empty()) {
                            seatsStr.pop_back();
                            seatsInput.setString(seatsStr);
                        }
                        else if (activeField == 2 && !priceStr.empty()) {
                            priceStr.pop_back();
                            priceInput.setString(priceStr);
                        }
                    }
                    else if (e.type == sf::Event::TextEntered)
                    {
                        if (e.text.unicode < 128)
                        {
                            char c = static_cast<char>(e.text.unicode);
                            if (activeField < 2)
                            {
                                if (isdigit(c))
                                {
                                    if (activeField == 0) {
                                        rowStr.push_back(c);
                                        rowInput.setString(rowStr);
                                    }
                                    else {
                                        seatsStr.push_back(c);
                                        seatsInput.setString(seatsStr);
                                    }
                                }
                            }
                            else
                            {
                                if (isdigit(c) || c == '.')
                                {
                                    priceStr.push_back(c);
                                    priceInput.setString(priceStr);
                                }
                            }
                        }
                    }
                }
                if (activeField == 0) {
                    activeBorder.setPosition(rowBox.getPosition());
                    activeBorder.setSize(rowBox.getSize());
                }
                else if (activeField == 1) {
                    activeBorder.setPosition(seatsBox.getPosition());
                    activeBorder.setSize(seatsBox.getSize());
                }
                else {
                    activeBorder.setPosition(priceBox.getPosition());
                    activeBorder.setSize(priceBox.getSize());
                }

                win.clear(BG_COLOR);
                win.draw(header);
                win.draw(headerTitle);
                win.draw(rowPrompt);
                win.draw(rowBox);
                win.draw(rowInput);
                win.draw(seatsPrompt);
                win.draw(seatsBox);
                win.draw(seatsInput);
                win.draw(pricePrompt);
                win.draw(priceBox);
                win.draw(priceInput);
                win.draw(instruct);
                win.draw(activeBorder);
              
                win.draw(backButton);
                win.draw(backButtonText);
                win.display();
            }
        }
        return;
    }

    if (actionChosen == 2)
    {
        // Si hay entradas vendidas, no se permite eliminar el segmento.
        if (seatsSoldFlag)
        {
            sf::Text msg("No se puede eliminar el segmento,\nya tiene entradas vendidas.\n\nClick 'Volver' para continuar.",
                font, 20);
            msg.setFillColor(sf::Color::Black);
            msg.setPosition(50.f, 150.f);

            bool waitMsg = true;
            while (waitMsg && win.isOpen())
            {
                sf::Event e;
                while (win.pollEvent(e))
                {
                    if (e.type == sf::Event::Closed) {
                        win.close();
                        return;
                    }
                    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                        if (backButton.getGlobalBounds().contains(m)) {
                            waitMsg = false;
                        }
                    }
                }
                win.clear(BG_COLOR);
                win.draw(header);
                win.draw(headerTitle);
                win.draw(msg);
              
                win.draw(backButton);
                win.draw(backButtonText);
                win.display();
            }
            return;
        }
        else
        {
            segmentsByEvent.getAt(chosenEventIndex).deletePosition(chosenSegmentIndex);
            int currentCount = segmentCount.getAt(chosenEventIndex);
            segmentCount.getAt(chosenEventIndex) = currentCount - 1;
            if (seatingMap.find(key) != seatingMap.end())
                seatingMap.erase(key);

            sf::Text doneMsg("Segmento eliminado exitosamente.\n\nClick 'Volver' para continuar...",
                font, 20);
            doneMsg.setFillColor(sf::Color::Black);
            doneMsg.setPosition(50.f, 150.f);

            bool waitDone = true;
            while (waitDone && win.isOpen())
            {
                sf::Event e;
                while (win.pollEvent(e))
                {
                    if (e.type == sf::Event::Closed) {
                        win.close();
                        return;
                    }
                    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                        if (backButton.getGlobalBounds().contains(m)) {
                            waitDone = false;
                        }
                    }
                }
                win.clear(BG_COLOR);
                win.draw(header);
                win.draw(headerTitle);
                win.draw(doneMsg);
                // Dibujar el botón "Volver" en la parte superior
                win.draw(backButton);
                win.draw(backButtonText);
                win.display();
            }
            return;
        }
    }
}

void Menu::updateReport() {

    if (!window)
        return;

    
    window->setTitle("Actualizar Reporte - Sistema de Ventas de Entradas");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente." << std::endl;
        return;
    }

    
    if (event.getEventCount() == 0) {
        sf::Text msg("No hay eventos disponibles para generar el reporte.\nPresione cualquier tecla para continuar...", font, 24);
        msg.setFillColor(TEXT_COLOR); // TEXT_COLOR es negro
        msg.setPosition(50.f, 200.f);

        bool exitLoop = false;
        while (window->isOpen() && !exitLoop) {
            sf::Event ev;
            while (window->pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) {
                    window->close();
                    return;
                }
                if (ev.type == sf::Event::KeyPressed || ev.type == sf::Event::MouseButtonPressed)
                    exitLoop = true;
            }
            window->clear(BG_COLOR);
            window->draw(msg);
            window->display();
        }
        window->setTitle("Sistema de Ventas de Entradas");
        return;
    }

    // Si hay eventos, generar el reporte
    std::string fileName = "reporte_eventos_actualizado.txt";
    EventReport report;
    report.generateGlobalReport(event, segment, seatingMap, fileName);

    
    sf::Text successMsg("Reporte actualizado con exito.\nPresione cualquier tecla para continuar...", font, 24);
    successMsg.setFillColor(TEXT_COLOR); 
    successMsg.setPosition(50.f, 200.f);

    bool exitLoop = false;
    while (window->isOpen() && !exitLoop) {
        sf::Event ev;
        while (window->pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window->close();
                return;
            }
            if (ev.type == sf::Event::KeyPressed || ev.type == sf::Event::MouseButtonPressed)
                exitLoop = true;
        }
        window->clear(BG_COLOR);
        window->draw(successMsg);
        window->display();
    }

    // Restaurar el título de la ventana
    window->setTitle("Sistema de Ventas de Entradas");
}

void Menu::generateCodesSFML(Discount& discount)
{
    
    sf::RenderWindow& win = *window;
    
    win.setTitle("Generar Códigos de Descuento");

    
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente.\n";
        return;
    }

    const float headerHeight = 100.f;
    sf::RectangleShape header(sf::Vector2f(1200.f, headerHeight));
    header.setFillColor(HEADER_COLOR);
    header.setPosition(0.f, 0.f);

    sf::Text titleText("Generar Códigos de Descuento", font, 30);
    titleText.setFillColor(TEXT_COLOR);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((1200.f - titleBounds.width) / 2.f, headerHeight + 50.f);

    sf::Text labelPercentage("Porcentaje de descuento:", font, 20);
    labelPercentage.setFillColor(TEXT_COLOR);
    labelPercentage.setPosition(300.f, 250.f);

    sf::Text labelCount("Cantidad de codigos:", font, 20);
    labelCount.setFillColor(TEXT_COLOR);
    labelCount.setPosition(300.f, 350.f);

    std::string percentageString;
    std::string countString;

    sf::Text percentageInputText("", font, 20);
    percentageInputText.setFillColor(sf::Color::Black);
    percentageInputText.setPosition(300.f, 280.f);

    sf::Text countInputText("", font, 20);
    countInputText.setFillColor(sf::Color::Black);
    countInputText.setPosition(300.f, 380.f);

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

    bool percentageActive = false;
    bool countActive = false;

    sf::Text errorMessage("", font, 20);
    errorMessage.setFillColor(sf::Color::Red);
    errorMessage.setPosition(300.f, 450.f);

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

            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
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
                    percentageActive = false;
                    countActive = false;
                }

               
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
                                    errorMessage.setString("La cantidad debe ser mayor a 0.");
                            }
                            catch (...)
                            {
                                errorMessage.setString("Cantidad inválida.");
                            }
                        }
                        else
                            errorMessage.setString("Ingrese la cantidad de códigos.");
                    }

                    if (errorMessage.getString().isEmpty())
                    {
                        
                        std::string currentCodes = discount.getCodesString();
                        int currentCount = 0;
                        {
                            std::stringstream ss(currentCodes);
                            std::string line;
                            while (std::getline(ss, line))
                            {
                                if (!line.empty())
                                    currentCount++;
                            }
                        }

                        
                        if (currentCount + countValue > 15)
                        {
                            errorMessage.setString("El límite de descuentos es 15.");
                        }
                      
                        else if (percentageValue > 100)
                        {
                            errorMessage.setString("No se pueden generar descuentos mayores al 100%.");
                        }
                        else
                        {
                            
                            discount.configure(percentageValue, countValue);
                            codesText.setString(discount.getCodesString());
                            
                            percentageString.clear();
                            countString.clear();
                        }
                    }
                }

               
                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    inGenerateScreen = false;
                }
            }

            // Manejo de texto en las cajas
            if (ev.type == sf::Event::TextEntered)
            {
                if (ev.text.unicode == 8)
                {
                    
                    if (percentageActive && !percentageString.empty())
                        percentageString.pop_back();
                    else if (countActive && !countString.empty())
                        countString.pop_back();
                }
                else if (ev.text.unicode < 128)
                {
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

        // Texto con la lista de códigos generados
        win.draw(codesText);

        win.display();
    }
    // Al salir, restablecemos título
    win.setTitle("Sistema de Ventas de Entradas");
}

void Menu::showDiscountCodesSFML(Discount& discount)
{
    
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
        win.clear(BG_COLOR);
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
    
    window->setTitle("Vender Entrada - Sistema de Ventas");

    
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente." << std::endl;
        return;
    }

    
    std::string cedula = "";
    int selectedEvent = -1;
    int selectedSegment = -1;
    std::string ticketCountStr = "";
    int ticketCount = 0;

    
    int currentStep = 0;

    
    sf::Text promptText("", font, 24);
    promptText.setFillColor(sf::Color::Black);
    promptText.setPosition(50.f, 20.f);

    
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
        } 
        window->clear(sf::Color(50, 50, 50));
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
    } 

    
    window->clear(BG_COLOR);
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

    
    int currentFaqIndex = -1;

    // Creamos un objeto Faq para obtener el contenido desde faq.cpp
    Faq faq;
    std::string faqContents[4] = {
        faq.getRulesContent(),      
        faq.getHowToBuyContent(),      
        faq.getDiscountsContent(),     
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

        
        win.clear(BG_COLOR);
        // Dibujar el header
        win.draw(header);
        
        for (int i = 0; i < NUM_OPTIONS; i++)
            win.draw(options[i]);
        
        win.draw(titleText);
        // Dibujar el contenido de la FAQ
        win.draw(faqContent);

        win.display();
    }

   
    win.setTitle("Sistema de Ventas de Entradas");
}