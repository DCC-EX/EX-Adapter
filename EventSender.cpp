#include <Arduino.h>
#include "EventSender.h"
#include "Adapter.h"

// This represents an event that the CS may send.
// Its implemented as a LIFO list for simplicity/size as the expected
// data volumes probabaly dont justify a hash table. 

EventSender * EventSender::first=NULL;

EventSender::EventSender(uint64_t _event) {
  event=_event;
  next=EventSender::first;
  EventSender::first=this;
}

int EventSender::count() {
  int c=0;
  for (EventSender * e= first; e ; e=e->next) c++;
  return c;
}

void EventSender::list() {
  Serial.println(F("<* May send:"));
  for (EventSender * e= first; e ; e=e->next) { 
    Serial.print(' ');
    Adapter::printHex(e->event); 
    Serial.println();
  }
Serial.print(F("*>\r\n"));
}

