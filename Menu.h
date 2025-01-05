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
    void sell();
    void showAbout();
    void configureDiscounts();
    void showEventMenu(Event &event);
    void listEventAndSegments(Event &event, Segment &segment);
    void sellTicket();
    void createUser();
    void checkSales();
    User* searchUserById(const std::string &id);

private:
    int validateChoice(int maxOption);

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
