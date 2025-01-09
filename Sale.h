#ifndef SALE_H
#define SALE_H

#include <iostream>
#include <iomanip>
#include <map>
#include <tuple>
#include <vector>
#include <string>
#include <limits> 

#include "User.h"
#include "Event.h"
#include "Segment.h"
#include "Seating.h"
#include "Discount.h"

class Sale {
public:
    void sell(User &user,
              Event &event,
              Segment &segment,
              std::map<std::tuple<int,int>,Seating> &seatingMap,
              Discount &discount);

private:
    bool checkEventsAvailability(Event &event);
    UserData* getOrRegisterUser(User &user);
    int chooseEvent(Event &event);
    int chooseSegment(Segment &segment, int selectedEvent);
    Seating& ensureSeating(int selectedEvent,
                           int selectedSegment,
                           Segment** segments,
                           std::map<std::tuple<int,int>,Seating> &seatingMap);
    int buyTickets(UserData *currentUser, Seating &seating);
    float applyDiscountIfWanted(Discount &discount);
    std::string askCardNumber();
    void printInvoice(UserData* currentUser,
                      Event &event,
                      int selectedEvent,
                      Segment** segments,
                      int selectedSegment,
                      int numTickets,
                      float ticketPrice,
                      float discountPercentage,
                      float totalCost,
                      std::vector<std::pair<int, char>> &purchasedSeats,
                      std::string &cardNumber);
    
    
    int readIntInRange(int minValue, int maxValue, const std::string &errorPrompt);
};

#endif 
