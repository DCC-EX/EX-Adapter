#ifndef EventSender_h
#define EventSender_h

#include <Arduino.h>
class EventSender {
    public:
     EventSender(uint64_t _event);
     static void list();
     static EventSender * first;
     static int count();
     uint64_t event;
     EventSender* next; 
};
#endif
