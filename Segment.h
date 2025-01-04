#ifndef SEGMENT_H
#define SEGMENT_H

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
    int numEvents; 
    int index;
    int* segmentCount; 
    Segment** segmentsByEvent; 

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

    int* getSegmentCount();
    void expandSegments(Segment& segment, int& numEvents);

    void manageSegments(Segment& segment, int& numEvents);
    void saveSegments(Segment& segment, int events);  

    void addSegmentData(Segment& segment);
    bool askToAddSegments(int& eventIndex);

    int getValidIntInput();
    float getValidFloatInput();
    
    Segment** getSegmentsByEvent();

};

#endif 