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
#ifndef LocoState_h
#define LocoState_h
#include <Arduino.h>

class LocoState {
    public: 
    static LocoState * get(uint16_t _id);
    byte DCCSpeedByte;
    int32_t functonMap;
    bool isFunctionOn(byte _func);
    LocoState(uint16_t _id);
    static LocoState * first;
    LocoState * next;
    int16_t id;
};
#endif