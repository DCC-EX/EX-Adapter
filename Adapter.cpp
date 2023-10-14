/*
 *  © 2023 Chris Harlow
 *  © 2023 Alex Shepherd
 *  © 2023 Balazs Racz
 *  All rights reserved.
 *
 *  This file is part of EX-Adapter for LCC
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CommandStation.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <Arduino.h> 
#include "Adapter.h"
#include "Bus.h"

ParserState Adapter::state=ParserState::READY; 
char Adapter::opcode='?';
byte Adapter::params=0;
uint64_t Adapter::p[4];

const uint64_t sender_mask=0xFFFFFFFFFFFF0000;

void Adapter::setup() {
    Serial.begin(115200);
    Serial.print(F("<L>\r\n")); // introduce self to CS.
    Bus::setCallback(eventHandler);
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

const char CMD_LCC='L'; // <opcode 
const char CMD_LISTENS_TO='L';  //<LL id xsender:event>
const char CMD_SENDS='S';       //<LS xevent>
const char CMD_READY='R';       // <LR>
const char CMD_SEND=' ';  // <L event>
const char CMD_DUMP='D';

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
             else if (input>='A' && input<='F') p[params]=p[params]*base + (input-'A' + 10);
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

    switch(opcode) {   
        case CMD_SEND: // <L  event>  send event to LCC         
            Bus::sendEvent(p[0]);
            break;
    
        case CMD_READY: // <LR>  all registration done 
            Bus::ready();
            break;
    
        case CMD_LISTENS_TO:  // <LL id event> 
            // Add sender:event to listened list with id
            Bus::registerInboundEvent(p[1],p[0]);
            break;

        case CMD_SENDS:  // <LS event>
            Bus::registerOutboundEvent(p[0]);
            break; 
        
        case CMD_DUMP: // <LD>  Dump
            Serial.print(F("<* LD no longer exists *>\n"));
            break;
        
        default:
            // All other incoming commands are ignored to meet 
            // DCC-EX throttle rules.
            break;
    }
}

void Adapter::eventHandler(uint32_t eventid) {
    Serial.print(F("<L "));
    Serial.print(eventid);
    Serial.print(F(">\n"));  
}
