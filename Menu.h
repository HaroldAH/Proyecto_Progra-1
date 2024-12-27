#pragma once
#include "Discount.h"
#include "Event.h"
#include "Segment.h"
class Menu
{
private:
  Segment segment;
  Event event;
public:
    void showMenu();
    void configureDiscounts();
    void showEventMenu(Event& event);
    void listEventAndSegments( Event& event, Segment& segment);
    int validateChoice(int& choice, int& size);
   
};
void showAbout();
