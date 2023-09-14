#ifndef EventSender_h
#define EventSender_h

#include <Arduino.h>
class EventSender {
    public:
     EventSender(uint16_t _event);
     static void list();
    private:
     static EventSender * first;
     uint16_t event;
     EventSender* next; 
};
#endif
