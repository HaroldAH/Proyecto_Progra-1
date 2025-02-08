#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Sale.h"
#include <map>
#include <memory>
#include <tuple>
#include "Event.h"
#include "Segment.h"
#include "User.h"
#include "Discount.h"
#include "Seating.h"
#include "Faq.h"
#include "EventReport.h"
#include <limits>
#include <sstream>
#include <vector>
#include <cctype>
#include "Music.h"

class Menu
{
private:
    Faq faq;
    Sale sale;
    Event event;
    Segment segment;
    User user;
    Discount discount;
    Seating seating;
    map<std::tuple<int, int>, Seating> seatingMap;
    Seating& operator=(const Seating& other);
    sf::RenderWindow* window;

public:
    // Modificar para que reciba la ventana de SFML
    void showMenu();
    int validateChoice(int& choice, int& size);
    void configureDiscounts(Discount& discount);
    void configureEvent(Event& event, Segment& segment);
    void listEventAndSegments(Event& event, Segment& segment);

    // Modificar para pasar la ventana a la función sellTicket
    void sellTicket(User& user, sf::RenderWindow& window);

    void saveEvent(Event& event, Segment& segment);
    void showAbout();
    void manageCodes(Discount& discount);
    void showFaq();
    void modifyOrDeleteSegment(Menu& menu, Event& event, Segment& segment,
        std::map<std::tuple<int, int>, Seating>& seatingMap);
    int readIntInRange(int min, int max, const string& prompt = "Opción inválida. Reingrese: ");
    void cancelPurchase();
    void updateReport();
    void executeOption(int option, sf::RenderWindow& window); // Pasar la ventana
    void generateCodesSFML(Discount& discount);
    void showDiscountCodes(Discount& discount);
    void showDiscountMenuSFML(Discount& discount);
    void showDiscountCodesSFML(Discount& discount);
    void deleteDiscountSFML(Discount& discount);
    void sellTicketSFML();
    void showFaqMenuSFML();
};

#endif
