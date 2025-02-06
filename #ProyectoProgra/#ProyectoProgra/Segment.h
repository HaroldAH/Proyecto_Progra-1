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
    void saveSegments(Segment& segment, int events, int eventIndex);

    void addSegmentData(Segment& segment, int index);

    int getValidIntInput();
    float getValidFloatInput();
    string getValidStringInput(const string& prompt);

};

#endif

