
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
  void venta();
  void showAbout();
  void configureDiscounts();

  void configureEvent(Event &event, Segment &segment);
  void listEventAndSegments(Event &event, Segment &segment);

  int validateChoice(int &choice, int &size);
  int updateSegmentEventCount(Event &event);

  void showAbout();
  void saveEvent(Event &event, Segment &segment);
};
