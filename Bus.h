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
#ifndef Bus_h
#define Bus_h
#include <Arduino.h>
typedef void (*EVENT_CALLBACK)(uint32_t eventid);

class Bus {
      public:

      // Tell bus about an outbound event we may send 
      static void registerOutboundEvent(uint64_t eventid);
      
      // Tell bus about an event we want to listen to
      static void registerInboundEvent(uint64_t inbound, int32_t callbackId);

      // Tell bus the callback function to invoke when a listened to event arrives.
      static void setCallback(EVENT_CALLBACK _callback);

      // Tell bus we are config complete and ready to play. 
      static void ready();

      // Send an event on the bus
      static void sendEvent(uint64_t eventid);

      // loop() support
      static void loop();
      
      static EVENT_CALLBACK adapterCallback;
};

#endif