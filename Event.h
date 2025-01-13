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
    int* purchasesByUser;      
    string* userIds;           

public:
    Event();                   
    ~Event();                  

    string getName();
    void setName(string& aName);

    string getDate();
    void setDate(string& aDate);

    string getDescription();
    void setDescription(string& aDescription);

    Event* getEvents();
    int getEventCount();

    
    void initializeTracking(int maxUsers);               
    bool purchaseTickets(const string& userId, int numTickets); 
    int getTicketsPurchasedByUser(const string& userId);  

    
    void expandAndAssignEvents(Event& event, int& numEvents); 
    int getValidIntInput();                                    
    void saveEvent(Event& event);                              
    bool isValidDate(string& date);                            
};

#endif
