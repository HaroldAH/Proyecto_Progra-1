#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include "List.h"

using namespace std;

class Segment {
private:
    string segmentName;
    int rows;
    int seats;
    float price;     
    int segmentCapacity;  
    int numEvents; 
    int index;
    List<int> segmentCount; 
    List<List<Segment>> segmentsByEvent; 

public:
    Segment();  
    ~Segment(); 

    string getName();
    void setName(string& aSegmentName);

    int getRows();
    void setRows(int& aRows);

    int getSeats();
    void setSeats(int& aSeats);

    float getPrice();
    void setPrice(float& aPrice);

    List<int>& getSegmentCount();
    List<List<Segment>>& getSegmentsByEvent();

    void manageSegments(Segment& segment, int& numEvents);
    void saveSegments(Segment& segment, int events);  

    void addSegmentData(Segment& segment, int index);
    bool askToAddSegments(int& eventIndex);

    int getValidIntInput();
    float getValidFloatInput();
};

#endif
