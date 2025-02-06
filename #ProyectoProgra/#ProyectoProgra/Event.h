#ifndef EVENT_H
#define EVENT_H

#include <string>
#include "List.h"
#include "Segment.h"

using namespace std;

class Event {
private:
    string name;
    string date;
    string description;
    int eventCount;
    List<Event> events;
    List<int> purchasesByUser;
    List<string> userIds;

public:
    Event();
    ~Event();

    string getName();
    void setName(string& aName);

    string getDate();
    void setDate(string& aDate);

    string getDescription();
    void setDescription(string& aDescription);

    List<Event>& getEvents();
    int getEventCount();

    void initializeTracking(int maxUsers);
    bool purchaseTickets(const string& userId, int numTickets);
    int getTicketsPurchasedByUser(const string& userId);

    void assignEvents(Event& event, int numEvents);
    int getValidIntInput();
    void saveEvent(Event& event, Segment& segment);
    bool isValidDate(string& date);

    bool cancelTickets(const string& userId, int numTickets);
    string getValidStringInput(const string& prompt);
};

#endif
