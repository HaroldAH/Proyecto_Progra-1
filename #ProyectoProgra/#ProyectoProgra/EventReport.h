#ifndef EVENT_REPORT_H
#define EVENT_REPORT_H

#include <string>
#include <map>
#include <tuple>
#include "Event.h"
#include "Segment.h"
#include "Seating.h"

class EventReport {
public:
    EventReport();
    ~EventReport();


    void generateGlobalReport(Event& evento,
        Segment& segmento,
        std::map<std::tuple<int, int>, Seating>& seatingMap,
        std::string& fileName);
};

#endif 
