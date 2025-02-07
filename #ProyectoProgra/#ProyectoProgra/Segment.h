#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include "List.h"
#include <SFML/Graphics.hpp>

class Segment {
private:
    std::string segmentName;
    int rows;
    int seats;
    float price;
    int segmentCapacity;
    List<int> segmentCount;
    List<List<Segment>> segmentsByEvent;
public:
    Segment();
    ~Segment();

    std::string getName();
    int getRows();
    int getSeats();
    float getPrice();

    void setName(const std::string& aSegmentName);
    void setRows(int aRows);
    void setSeats(int aSeats);
    void setPrice(float aPrice);

    List<int>& getSegmentCount();
    List<List<Segment>>& getSegmentsByEvent();

    void manageSegments(Segment& segment, int& numEvents);
    void addSegmentData(Segment& segment, int index, sf::RenderWindow& win);
    void saveSegments(sf::RenderWindow& win, Segment& segment, int events, int eventIndex = 0);

    int getValidIntInput();
    float getValidFloatInput();
    std::string getValidStringInput(const std::string& prompt);
};

#endif // SEGMENT_H