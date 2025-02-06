#include "Faq.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>


// Alias para evitar conflictos con SFML
namespace sfml = sf;

// Definición de constantes de color (usando el mismo formato que en Menu)
static const sfml::Color BG_COLOR(200, 200, 200);       // Fondo gris
static const sfml::Color BOX_COLOR(230, 230, 230);        // Color de los botones (no usado en este ejemplo)
static const sfml::Color HIGHLIGHT_COLOR = sfml::Color(255, 0, 0); // Rojo para resaltar
static const sfml::Color TEXT_COLOR = sfml::Color::Black;          // Texto negro

// ----------------------------------------------------------------------
// Muestra el menú de FAQ (Preguntas Frecuentes)
void Faq::showFaq()
{
    // Crear una ventana similar al menú principal
    sfml::RenderWindow window(sfml::VideoMode(800, 600), "Preguntas Frecuentes", sfml::Style::Close);

    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente en Faq::showFaq().\n";
        return;
    }

    const int NUM_OPTIONS = 5;
    int selectedOption = -1;
    std::string faqOptions[NUM_OPTIONS] = {
        "1. Reglas del evento",
        "2. Como comprar entradas",
        "3. Aplicacion de descuentos",
        "4. Como ver las ventas realizadas",
        "5. Volver al menu principal"
    };

    sfml::Text menuItems[NUM_OPTIONS];
    sfml::RectangleShape optionBoxes[NUM_OPTIONS];

    float startY = 140.f;
    float boxWidth = 550.f;
    float boxHeight = 55.f;
    float spacing = 70.f;

    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        menuItems[i].setFont(font);
        menuItems[i].setString(faqOptions[i]);
        menuItems[i].setCharacterSize(30);
        menuItems[i].setFillColor(TEXT_COLOR);
        menuItems[i].setPosition(130.f, startY + i * spacing);

        optionBoxes[i].setSize(sfml::Vector2f(boxWidth, boxHeight));
        optionBoxes[i].setFillColor(BOX_COLOR);
        optionBoxes[i].setPosition(sfml::Vector2f(120.f, (startY - 5.f) + i * spacing));
    }

    while (window.isOpen())
    {
        sfml::Event event;
        sfml::Vector2i mousePos = sfml::Mouse::getPosition(window);

        // Determinar qué opción se está seleccionando
        selectedOption = -1;
        for (int i = 0; i < NUM_OPTIONS; i++)
        {
            sfml::FloatRect bounds = optionBoxes[i].getGlobalBounds();
            if (bounds.contains(static_cast<sfml::Vector2f>(mousePos)))
            {
                selectedOption = i;
            }
        }

        while (window.pollEvent(event))
        {
            if (event.type == sfml::Event::Closed)
            {
                window.close();
                return;
            }
            else if (event.type == sfml::Event::KeyPressed)
            {
                if (event.key.code == sfml::Keyboard::Enter && selectedOption != -1)
                {
                    window.close();
                    switch (selectedOption + 1)
                    {
                    case 1: showRules();     break;
                    case 2: showHowToBuy();  break;
                    case 3: showDiscounts(); break;
                    case 4: showViewSales(); break;
                    case 5: return;
                    }
                }
            }
            else if (event.type == sfml::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sfml::Mouse::Left && selectedOption != -1)
                {
                    window.close();
                    switch (selectedOption + 1)
                    {
                    case 1: showRules();     break;
                    case 2: showHowToBuy();  break;
                    case 3: showDiscounts(); break;
                    case 4: showViewSales(); break;
                    case 5: return;
                    }
                }
            }
        }

        window.clear(BG_COLOR);
        for (int i = 0; i < NUM_OPTIONS; i++)
        {
            if (i == selectedOption)
                optionBoxes[i].setFillColor(HIGHLIGHT_COLOR);
            else
                optionBoxes[i].setFillColor(BOX_COLOR);

            window.draw(optionBoxes[i]);
            window.draw(menuItems[i]);
        }
        window.display();
    }
}

// ----------------------------------------------------------------------
// Muestra las reglas del evento
void Faq::showRules()
{
    sfml::RenderWindow window(sfml::VideoMode(800, 600), "Reglas del evento", sfml::Style::Close);

    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente en Faq::showRules().\n";
        return;
    }

    sfml::Text title("Reglas del evento", font, 28);
    title.setFillColor(sfml::Color::Yellow);
    title.setStyle(sfml::Text::Bold);
    title.setPosition(250.f, 40.f);

    sfml::Text content(
        "- Llegar con minimo 30 minutos de anticipacion.\n"
        "- Mantener el orden.\n"
        "- No ingresar con objetos prohibidos.\n"
        "- Respetar normas de seguridad.\n"
        "- No nos hacemos responsables por objetos perdidos.\n",
        font, 20
    );
    content.setFillColor(TEXT_COLOR);
    content.setPosition(80.f, 120.f);

    sfml::Text instruction("Presione Enter o Escape para volver...", font, 18);
    instruction.setFillColor(sfml::Color::Green);
    instruction.setPosition(180.f, 500.f);

    while (window.isOpen())
    {
        sfml::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sfml::Event::Closed)
                window.close();
            else if (e.type == sfml::Event::KeyPressed)
            {
                if (e.key.code == sfml::Keyboard::Enter || e.key.code == sfml::Keyboard::Escape)
                    window.close();
            }
        }

        window.clear(BG_COLOR);
        window.draw(title);
        window.draw(content);
        window.draw(instruction);
        window.display();
    }
}

// ----------------------------------------------------------------------
// Muestra las instrucciones para comprar entradas
void Faq::showHowToBuy()
{
    sfml::RenderWindow window(sfml::VideoMode(800, 600), "Como comprar entradas", sfml::Style::Close);

    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente en Faq::showHowToBuy().\n";
        return;
    }

    sfml::Text title("Como comprar entradas", font, 28);
    title.setFillColor(sfml::Color::Yellow);
    title.setStyle(sfml::Text::Bold);
    title.setPosition(200.f, 40.f);

    sfml::Text content(
        "- Seleccione 'Vender Entrada' en el menu principal.\n"
        "- Registre un usuario si no existe.\n"
        "- Elija el evento, segmento y asientos.\n"
        "- Complete el proceso de pago.\n"
        "- Reciba confirmacion.\n",
        font, 20
    );
    content.setFillColor(TEXT_COLOR);
    content.setPosition(80.f, 120.f);

    sfml::Text instruction("Presione Enter o Escape para volver...", font, 18);
    instruction.setFillColor(sfml::Color::Green);
    instruction.setPosition(180.f, 500.f);

    while (window.isOpen())
    {
        sfml::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sfml::Event::Closed)
                window.close();
            else if (e.type == sfml::Event::KeyPressed)
            {
                if (e.key.code == sfml::Keyboard::Enter || e.key.code == sfml::Keyboard::Escape)
                    window.close();
            }
        }

        window.clear(BG_COLOR);
        window.draw(title);
        window.draw(content);
        window.draw(instruction);
        window.display();
    }
}

// ----------------------------------------------------------------------
// Muestra las instrucciones para la aplicación de descuentos
void Faq::showDiscounts()
{
    sfml::RenderWindow window(sfml::VideoMode(800, 600), "Descuentos", sfml::Style::Close);

    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente en Faq::showDiscounts().\n";
        return;
    }

    sfml::Text title("Aplicacion de descuentos", font, 28);
    title.setFillColor(sfml::Color::Yellow);
    title.setStyle(sfml::Text::Bold);
    title.setPosition(180.f, 40.f);

    sfml::Text content(
        "- Se generan codigos en 'Configurar Descuentos'.\n"
        "- Al vender entrada, ingrese el codigo.\n"
        "- Si es valido, se aplica al subtotal.\n"
        "- Cada codigo puede tener uso limitado.\n",
        font, 20
    );
    content.setFillColor(TEXT_COLOR);
    content.setPosition(80.f, 120.f);

    sfml::Text instruction("Presione Enter o Escape para volver...", font, 18);
    instruction.setFillColor(sfml::Color::Green);
    instruction.setPosition(180.f, 500.f);

    while (window.isOpen())
    {
        sfml::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sfml::Event::Closed)
                window.close();
            else if (e.type == sfml::Event::KeyPressed)
            {
                if (e.key.code == sfml::Keyboard::Enter || e.key.code == sfml::Keyboard::Escape)
                    window.close();
            }
        }

        window.clear(BG_COLOR);
        window.draw(title);
        window.draw(content);
        window.draw(instruction);
        window.display();
    }
}

// ----------------------------------------------------------------------
// Muestra las instrucciones para consultar las ventas realizadas
void Faq::showViewSales()
{
    sfml::RenderWindow window(sfml::VideoMode(800, 600), "Como ver las ventas", sfml::Style::Close);

    sfml::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente en Faq::showViewSales().\n";
        return;
    }

    sfml::Text title("Como ver las ventas realizadas", font, 28);
    title.setFillColor(sfml::Color::Yellow);
    title.setStyle(sfml::Text::Bold);
    title.setPosition(140.f, 40.f);

    sfml::Text content(
        "- Seleccione 'Consultar Ventas'.\n"
        "- Se muestra la ocupacion de asientos por evento.\n"
        "- Ve cuantas entradas se han vendido.\n"
        "- Tambien puede consultar el estado de cada asiento.\n",
        font, 20
    );
    content.setFillColor(TEXT_COLOR);
    content.setPosition(60.f, 120.f);

    sfml::Text instruction("Presione Enter o Escape para volver...", font, 18);
    instruction.setFillColor(sfml::Color::Green);
    instruction.setPosition(180.f, 500.f);

    while (window.isOpen())
    {
        sfml::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sfml::Event::Closed)
                window.close();
            else if (e.type == sfml::Event::KeyPressed)
            {
                if (e.key.code == sfml::Keyboard::Enter || e.key.code == sfml::Keyboard::Escape)
                    window.close();
            }
        }

        window.clear(BG_COLOR);
        window.draw(title);
        window.draw(content);
        window.draw(instruction);
        window.display();
    }
}