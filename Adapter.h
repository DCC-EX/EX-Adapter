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

#ifndef Adapter_h
#define Adapter_h
#include <Arduino.h>
enum ParserState  {
      READY,  // looking for <
      GETOP1,  // expecting opcode L  
      GETOP2,  // expecting opcode2
      SKIP,   // skipping blanks
      PARSE,  // reading value
    };

class Adapter {
    public:
      static void setup();
      static void loop();
      static void eventHandler(uint32_t eventid);
    
    private:
    static char opcode;
    static byte params;
    static ParserState state;
    static uint64_t p[4];
    static bool parse(char hot);
    static void processCommand();
    
};

#endif