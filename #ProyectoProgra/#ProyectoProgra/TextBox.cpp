#include "TextBox.h"

TextBox::TextBox(float x, float y, float width, float height) {
    box.setPosition(x, y);
    box.setSize({ width, height });
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color::Black);

    if (!font.loadFromFile("resourses/arial.ttf")) {
        cout << "Error: No se pudo cargar la fuente arial.ttf." << std::endl;
    }


    text.setFont(font);
    text.setPosition(x + 5, y + 5);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    isActive = false;
}

void TextBox::handleEvent(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            isActive = true;
        }
        else {
            isActive = false;
        }
    }

    if (isActive && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !input.empty()) {
            input.pop_back();  // Borrar último carácter
        }
        else if (event.text.unicode < 128 && event.text.unicode != '\b') {
            input += static_cast<char>(event.text.unicode);
        }
        text.setString(input);
    }
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

std::string TextBox::getInput() const {
    return input;
}

void TextBox::clear() {
    input.clear();
    text.setString("");
}
