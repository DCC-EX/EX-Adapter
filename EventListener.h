#ifndef EventListener_h
#define EventListener_h

#include <Arduino.h>
class EventListener {
    public:
     EventListener(uint16_t _id, uint64_t _event);
     static uint16_t getId(uint64_t _event);
     static const uint16_t NOT_FOUND=0xFFFF;
     static void list();
     static int count(); 
     static EventListener * first;
     uint64_t event;
     uint16_t id;
     EventListener* next; 
};
#endif
