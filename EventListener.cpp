#include <Arduino.h>
#include "EventListener.h"
#include "Adapter.h"

// This represents an event that the CS is interested in.
// Its implemented as a LIFO list for simplicity/size as the expected
// data volumes probabaly dont justify a hash table. 

EventListener * EventListener::first=NULL;

EventListener::EventListener(uint16_t _id, uint64_t _event) {
  event=_event;
  id=_id;
  next=EventListener::first;
  EventListener::first=this;
}

uint16_t EventListener::getId(uint64_t _event) {
  for (EventListener * e= first; e ; e=e->next) 
    if (_event==e->event) return e->id;
  return NOT_FOUND;
}

int EventListener::count() {
  int c=0;
  for (EventListener * e= first; e ; e=e->next) c++;
  return c;
}

void EventListener::list() {
  Serial.println(F("<* Listening for:"));
  for (EventListener * e= first; e ; e=e->next) { 
    Serial.print(e->id);
    Serial.print(' ');
    Adapter::printHex(e->event); 
    Serial.println();
  }
Serial.print(F("*>\r\n"));
}
