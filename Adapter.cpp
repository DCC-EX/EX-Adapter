#include <Arduino.h> 
#include "Adapter.h"
#include "EventListener.h"
#include "EventSender.h"
#include "Bus.h"

ParserState Adapter::state=ParserState::READY; 
char Adapter::opcode='?';
byte Adapter::params=0;
uint64_t Adapter::p[4];

const uint64_t sender_mask=0xFFFFFFFFFFFF0000;

void Adapter::setup() {
    Serial.begin(115200);
    Serial.print(F("<L>\r\n")); // introduce self to CS.
    // Bus::setCallback(eventArrived); 
}

void Adapter::loop() {
    if (ready) Bus::loop();

    // Read any inbound
    while(Serial.available()) {
        if (parse(Serial.read())) {
            // and do only one command per loop
            processCommand();
            break;
        }
    }
}

const char CMD_LCC='L'; // <opcode 
const char CMD_LISTENS_TO='L';  //<LL id xsender:event>
const char CMD_SENDS='S';       //<LS xevent>
const char CMD_READY='R';       // <LR>
const char CMD_SEND=' ';  // <L event>
const char CMD_DUMP='D';
 

bool Adapter::ready=false;

bool Adapter::parse(char input) {
  static byte base=10;
    switch (state) {
        case READY:
             if (input=='<') state=GETOP1;
             break;
        case GETOP1:
             // This check is not nice but allows us to ditch 
            // anything we are not interested in quickly.
            if (input!=CMD_LCC) {
                state=READY;
                break;
             }
             state=GETOP2;
             break;
        case GETOP2:
            // This check is not nice but allows us to ditch 
            // anything we are not interested in quickly.
             if (input == CMD_SEND  
                 || input == CMD_LISTENS_TO 
                 || input == CMD_SENDS
                 || input == CMD_READY
                 || input == CMD_DUMP) {
                    opcode=input; 
                    state=SKIP;
                    params=0;
                    p[0]=0;
             }
             else state=READY; // ignore until next <
             break;
        case SKIP:
             if (input==' ') break;
             if (input=='>') {
                state=READY;
                return true;
             }
             if (input=='x') {
                base=16;
                state=PARSE;
                break;
             }
             base=10;
             state=PARSE;
             [[fallthrough]]; 

        case PARSE:
             if (input>='0' && input<='9') p[params]=p[params]*base + (input-'0');
             else if (input>='A' && input<='F') p[params]=p[params]*base + (input-'A'+10);
             else if (input==' ') {
                params++;
                state=SKIP;
             } 
             else if (input=='>') {
                params++;
                state=READY;
                return true; 
             } 
             break; // side effect, ignores dots, colons  in x
        }
    return false;  
    } 

   void Adapter::processCommand() {

    // Commands from DCC-EX are as follows
       
    if (opcode==CMD_SEND) { // <L  event>  send self:event to LCC         
        Bus::sendEvent(p[0]);
        return; 
    }
    
    if (opcode==CMD_READY) { // <LR>  all data got from EXRAIL
        Bus::setCallback(eventHandler);
        // create heap resident arrays that the Bus can keep a reference to.
        int16_t senderCount=EventSender::count();
        uint64_t * senderArray = new uint64_t[senderCount];
        int sa=0;
        for (EventSender * e=EventSender::first; e; e=e->next) {
            senderArray[sa++]=e->event;
        }
        Bus::outboundEvents(senderArray,senderCount);
        
        int16_t listenerCount=EventListener::count();
        uint64_t * listenerArray = new uint64_t[listenerCount];
        int la=0;
        for (EventListener * e=EventListener::first; e; e=e->next) {
            listenerArray[la++]=e->event;
        }
        Bus::inboundEvents(listenerArray,listenerCount);
        ready=true;  
        Bus::ready();
        return; 
    }
    
    if (opcode==CMD_LISTENS_TO) { // <LL  id sender:event> 
        // Add sender:event to listened list with id
        new EventListener(p[0],p[1]);
        return; 
    }
    
    if (opcode==CMD_SENDS) { // <LS event>
        // Add self:event to possible sending list
        // De-duplicate as exrail may send from multiple places
        for (EventSender * es=EventSender::first;es;es=es->next) 
            if (es->event==p[0]) return; 
        new EventSender(p[0]);
        return; 
    }
    
    if (opcode==CMD_DUMP) { // <LD>  Dump
        EventListener::list();
        EventSender::list();
        return; 
    }
   
    // All other incoming commands are ignored for now
}

void Adapter::eventHandler(uint16_t eventid) {
    Serial.print(F("<L "));
    Serial.print(eventid);
    Serial.print(F(">\n"));  
}

const char Hexpattern[]="0123456789ABCDEF";

void Adapter::printHex(uint64_t value) {
     if (value == 0) {
        Serial.print(F("x00"));
        return;
     }
     byte * px=((byte *) & value) + sizeof(value);
     Serial.print('x');
     bool print=false; // until first nonzero byte
     for (int i=sizeof(value);i;i--) {
       px--;
       if (*px) print=true;
       if (print) {
        Serial.print(Hexpattern[(*px) >> 4]);
        Serial.print(Hexpattern[(*px) & 0x0F]);
        }
     }
}
