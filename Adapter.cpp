#include <Arduino.h> 
#include "Adapter.h"
#include "EventListener.h"
#include "EventSender.h"

ParserState Adapter::state=ParserState::READY; 
char Adapter::opcode='?';
byte Adapter::params=0;
uint64_t Adapter::p[4];


void Adapter::setup() {
    Serial.begin(115200);
    Serial.print(F("<L>\r\n")); // introduce self to CS. 
}

void Adapter::loop() {
    // Read any inbound
    while(Serial.available()) {
        if (parse(Serial.read())) {
            // and do only one command per loop
            processCommand();
            break;
        }
    }
}


bool Adapter::parse(char input) {
  static byte base=10;
    switch (state) {
        case READY:
             if (input=='<') state=GETOP1;
             break;
        case GETOP1:
             // This check is not nice but allows us to ditch 
            // anything we are not interested in quickly.
            if (input!='L') {
                state=READY;
                break;
             }
             state=GETOP2;
             break;
        case GETOP2:
            // This check is not nice but allows us to ditch 
            // anything we are not interested in quickly.
             if (input!='L' && input !='S' && input!='X' && input!='D') {
                state=READY;
                break;
             }
             opcode=input; 
             state=SKIP;
             params=0;
             p[0]=0;
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
             else if (input>='A' && input<='F') p[params]=p[params]*base + (input-'A');
             else if (input==' ') {
                params++;
                state=SKIP;
             } 
             else if (input=='>') {
                params++;
                state=READY;
                return true; 
             } 
             break; // side effect, ignores dots in x
        }
    return false;  
    } 

   void Adapter::processCommand() {
    /*
    Serial.print("opcode=");
    Serial.print(opcode);
    Serial.print(" params=");
    Serial.println(params); 
    */

    // Commands from DCC-EX are as follows
       
    if (opcode=='X') { // <LX  event>  send self:event to LCC 
        //   LCC::send(p[0]);
        return; 
    }
    
    if (opcode=='L') { // <LL  id sender:event> 
        // Add sender:event to listened list with id
        new EventListener(p[0],p[1]); 
        return; 
    }
    
    if (opcode=='S') { // <LS event>
        // Add self:event to possible sending list
        new EventSender(p[0]);
        return; 
    }
    
    if (opcode=='D') { // <LD>  Dump
        EventListener::list();
        EventSender::list();
        return; 
    }
   
    // All other incoming commands are ignored for now
}

const char Hexpattern[]="0123456789ABCDEF";
void Adapter::printHex(uint16_t value) {
     printHexLittleEndian((byte *) &value, sizeof(value));
} 
void Adapter::printHex(uint64_t value) {
     printHexLittleEndian((byte *) &value, sizeof(value));
} 

void Adapter::printHexLittleEndian(byte * px, byte length) {
     Serial.print('x');
     px+=length; 
     for (int i=length;i;i--) {
       px--;
       Serial.print(Hexpattern[(*px) >> 4]);
       Serial.print(Hexpattern[(*px) & 0x0F]);
     }
}


