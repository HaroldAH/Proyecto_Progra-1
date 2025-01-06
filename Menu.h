#ifndef MENU_H
#define MENU_H

#include "User.h"
#include "Event.h"
#include "Segment.h"
#include "discount.h"
#include "Seating.h"
#include <map>
#include <tuple>
#include <string>

class Seating;

class Menu {
public:

    void showMenu();
    void sellTicket();
    void createUser();
    void checkSales(Event& event);
    void configureDiscounts();

   
    void configureEvent(Event& event, Segment& segment);
    void listEventAndSegments(Event& event, Segment& segment);
    int updateSegmentEventCount(Event& event);
    void saveEvent(Event& event, Segment& segment);

  
    void sell();
    void showAbout();
    User* searchUserById(const std::string &id);

private:

    int validateChoice(int& choice, int& size);

    std::map<std::tuple<int, int>, Seating> seatingMap;

    Event event;
    Segment segment;
    Discount discountManager;
    User* users = nullptr;

    int userCount = 0;
    int userCapacity = 0;
    int numUsers;
    int maxUsers;
};

#endif
