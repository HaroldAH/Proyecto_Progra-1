#ifndef SALE_H
#define SALE_H

#include <map>
#include <tuple>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include "User.h"
#include "Event.h"
#include "Segment.h"
#include "Seating.h"
#include "Discount.h"

using namespace std;

class Sale {
private:
    bool checkEventsAvailability(Event &event);
    UserData* getOrRegisterUser(User &user);
    int chooseEvent(Event &event);
    int chooseSegment(Segment &segment, int selectedEvent);
    Seating& ensureSeating(int selectedEvent,
                           int selectedSegment,
                           Segment** segments,
                           map<tuple<int,int>,Seating> &seatingMap);
    int buyTickets(UserData *currentUser, Seating &seating);
    float applyDiscountIfWanted(Discount &discount);
    string askCardNumber();
    void printInvoice(UserData* currentUser,
                      Event &event,
                      int selectedEvent,
                      Segment** segments,
                      int selectedSegment,
                      int numTickets,
                      float ticketPrice,
                      float discountPercentage,
                      float totalCost,
                      vector<pair<int, char>> &purchasedSeats,
                      string &cardNumber);
public:
    void sell(User &user,
              Event &event,
              Segment &segment,
              map<tuple<int,int>, Seating> &seatingMap,
              Discount &discount);


};

#endif
