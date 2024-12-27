
#pragma once
class Segment;
#include <string>
using namespace std;
class Event {
private:
    string name;
    string date;
    string description;      
    int eventCount;       
    int eventCapacity;        
    Event* events;    

public:
    Event(); 
    ~Event();  
    string getName();
    void setName(string& aName);

    string getDate() ;
    void setDate(string& aDate);

    string getDescription() ;
    void setDescription(string& aDescription);

    void saveEvent(Event& event); 
    void initializeEvents(int& eventcapacity);
    
    Event* getEvents(); 
    int getEventCount(); 
    
    int getValidIntInput();
};

