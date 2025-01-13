#ifndef SALE_H
#define SALE_H

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

class Sale {
private:
    bool checkEventsAvailability(Event &event);

    UserData* getOrRegisterUser(User &user);

    int chooseEvent(Event &event);

    int chooseSegment(Segment &segment, int selectedEvent);

    Seating& ensureSeating(int selectedEvent,
                           int selectedSegment,
                           Segment** segments,
                           std::map<std::tuple<int,int>,Seating> &seatingMap);

    int buyTickets(UserData *currentUser, Event &event, int selectedEvent);

    float applyDiscountIfWanted(Discount &discount);

    string askCardNumber();

    int readIntInRange(int minValue, int maxValue, const std::string &errorPrompt);

    void printInvoice(UserData* currentUser,
                  Event &event,
                  int selectedEvent,
                  Segment** segments,
                  int selectedSegment,
                  int numTickets,
                  float ticketPrice,
                  float discountPercentage,
                  float totalCost,
                  int* purchasedRows,
                  char* purchasedCols,
                  int numPurchasedSeats,
                  std::string cardNumber);


public:

    void sell(User &user,
              Event &event,
              Segment &segment,
              map<std::tuple<int,int>,Seating> &seatingMap,
              Discount &discount);
};

#endif
