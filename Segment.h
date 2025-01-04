#pragma once
#include "Event.h"
#include <string>
 #include "Seating.h"
using namespace std;

class Segment {
private:
    string segmentName;
    int rows;
    int seats;
    float price;     
    int segmentCapacity;   
    int* segmentCount; 
    Segment** segmentsByEvent; 
    int numEvents;
   Seating seating;

public:
    Segment();  
    ~Segment(); 

    string getName();
  void setName(const std::string& aSegmentName);

    int getRows();
    void setRows(int& aRows);

    int getSeats();
    void setSeats(int& aSeats);

    float getPrice();
    void setPrice(float& aPrice);

    void initializeSegments(int& capacity);
    void saveSegments(Event& event);  

    Segment** getSegmentsByEvent();
    void getSegmentCount(Event& event, int* destino);
    int getValidIntInput();

};
