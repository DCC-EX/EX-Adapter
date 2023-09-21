#ifndef Bus_h
#define Bus_h
#include <Arduino.h>
typedef void (*EVENT_CALLBACK)(uint64_t event);

class Bus {
      public:

      // Tell bus list of all outbound events we may send 
      static void outboundEvents(uint16_t outbound[], uint16_t count);
      
      // Tell bus list of all sender:events that we are interested in
      static void inboundEvents(uint64_t inbound[], uint16_t count);

      // Tell bus the callback function to invoke when a listened to event arrives.
      static void setCallback(EVENT_CALLBACK callback);

      // Tell bus we are config complete and ready to play. 
      static void ready();

      // Send an event on the bus
      static void sendEvent(uint16_t eventid);
      
      // Arduino activity loop called repeatedly
      static void loop();
};

#endif