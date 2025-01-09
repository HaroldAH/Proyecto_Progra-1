#ifndef EVENT_H
#define EVENT_H

#include <string>
using namespace std;

class Event {
private:
    string name;
    string date;
    string description;      
    int eventCount;              
    Event* events;    
    
public:
    Event(); 
    ~Event();
      
    string getName();
    void setName(string& aName);

    string getDate();
    void setDate(string& aDate);

    string getDescription();
    void setDescription(string& aDescription);

    void saveEvent(Event& event); 
    
    Event* getEvents(); 
    int getEventCount(); 

    void expandAndAssignEvents(Event& event, int& numEvents);
    bool* isEventAvailableForSegments(int index); 

    int getValidIntInput();
    bool isValidDate(string &date);
};

#endif 
