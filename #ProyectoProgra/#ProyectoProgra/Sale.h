#ifndef SALE_H
#define SALE_H
#include <sstream>

#include <iostream>
#include <iomanip>
#include <map>
#include <tuple>
#include <string>
#include <limits>
#include "User.h"
#include "Event.h"
#include "Segment.h"
#include "Seating.h"
#include "Discount.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cctype>    // Para isdigit


using namespace std;

// Definiciones de colores (ajusta según tus necesidades)
static const sf::Color BG_COLOR_EV(200, 200, 200);
static const sf::Color HEADER_COLOR_EV(160, 160, 160);
static const sf::Color TEXT_COLOR_EV = sf::Color::Black;
static const sf::Color HIGHLIGHT_COLOR_EV = sf::Color::Red;


class Sale {
private:
    // Métodos privados para la lógica interna
    bool checkEventsAvailability(Event& event);

    UserData* getOrRegisterUser(User& user, sf::RenderWindow& win); // Modificado para SFML

    int chooseEvent(Event& event, sf::RenderWindow& win); // Modificado para SFML

    int chooseSegment(Segment& segment, int selectedEvent, sf::RenderWindow& window);
    Seating& ensureSeating(int selectedEvent, int selectedSegment,
        List<List<Segment>>& segments,
        std::map<std::tuple<int, int>, Seating>& seatingMap,
        sf::RenderWindow& win);


    int buyTickets(UserData* currentUser, Event& event, int selectedEvent, Seating& seating, sf::RenderWindow& window);

    float applyDiscountIfWanted(Discount& discount, sf::RenderWindow& window);

    string askCardNumber();

    int readIntInRange(int minValue, int maxValue, const std::string& errorPrompt);

    void printInvoice(UserData* currentUser, Event& event, int selectedEvent, List<List<Segment>>& segments, int selectedSegment,
        int numTickets, float ticketPrice, float discountPercentage, float totalCost,
        int* purchasedRows, char* purchasedCols, int numPurchasedSeats, std::string cardNumber, sf::RenderWindow& window);

public:
    // Función pública que maneja la compra de boletos
    void cancelPurchase(User& user, Event& event, Segment& segment,
        std::map<std::tuple<int, int>, Seating>& seatingMap, sf::RenderWindow& window);


    void sell(User& user, Event& event, Segment& segment,
        std::map<std::tuple<int, int>, Seating>& seatingMap, Discount& discount,
        sf::RenderWindow& window);

};

#endif
