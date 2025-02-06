#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox {
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    bool isActive;
    std::string input;

public:
    TextBox(float x, float y, float width, float height);

    void handleEvent(sf::Event event);

    void draw(sf::RenderWindow& window);

    std::string getInput() const;

    void clear();
};

#endif // TEXTBOX_H

