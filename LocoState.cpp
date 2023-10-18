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

#include "LocoState.h"

LocoState * LocoState::first=nullptr;
    
LocoState::LocoState(uint16_t _id) {
    id=_id;
    next=first;
    first=this;
    DCCSpeedByte=0x80; // speed 0 fwd.
    functonMap=0;   // no functions on
    }

LocoState * LocoState::get(uint16_t _id) {
        for (auto p=first;p;p=p->next) {
            if (p->id==_id) return p;
        }
        return new LocoState(_id);
}

bool LocoState::isFunctionOn(byte _func) {
        return functonMap & (1 <<_func);
}
