/*
 *  © 2023 Alex Shepherd
 *  © 2023 Balazs Racz
 *  © 2023 Chris Harlow
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


#include "Bus.h"

/// START OF SETUP ROUTINES 
EVENT_CALLBACK Bus::adapterCallback;

// Tell bus the callback function to invoke when a listened to event arrives.
// Must be called first.
void Bus::setCallback(EVENT_CALLBACK _callback) {
    adapterCallback=_callback;
    // ?? CBUS
}

// Tell bus about an outbound event we may send 
void Bus::registerOutboundEvent(uint64_t event) {
// ?? CBUS 
}
      
// Tell bus about an event we are interested in
void Bus::registerInboundEvent(uint64_t event, int32_t callbackId) {
// ?? CBUS
}
     
// Tell bus we are config complete and ready to play. 
void Bus::ready() {
    //?? CBUS
}
// Send an event on the bus
void Bus::sendEvent(uint64_t eventid) {
    // ?? CBUS 
}



