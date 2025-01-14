#ifndef MENU_H
#define MENU_H

#include "Sale.h"
#include <map>
#include <tuple>
#include "Event.h"
#include "Segment.h"
#include "User.h"
#include "Discount.h"
#include "Seating.h"
#include "Faq.h"

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

public:
    void showMenu();
    int validateChoice(int &choice, int &size);
    void configureDiscounts(Discount &discount);
    void configureEvent(Event &event, Segment &segment);
    void listEventAndSegments(Event &event, Segment &segment);
    void sellTicket(User &user);
    int updateSegmentEventCount(Event &event);
    void saveEvent(Event &event, Segment &segment);
    void showAbout();
    void manageCodes(Discount &discount);
    void showFaq();
};

#endif
