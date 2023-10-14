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
#include <Arduino.h>
#include "Adapter.h"

/* Sample DCC-EX Commands To Execute for Testing

List events to be listened for:

<LL 0 x0201570002B00302>
<LL 1 x0201570002B00303>
<LL 2 x1122334455667788>

List events that may be sent:
<LS x1122334455667788>

Tell Bus we are ready to run 
<LR>

Send an event 
<L x1122334455667788>

....   others... 
<LL 3 x0201570002B002F7>

<LL 4 x1122334455667787> 
<LL 5 x1122334455667788>
<LL 6 x1122334455667789>

<LS x1122334455667787>
<LS x1122334455667788>
<LS x1122334455667789>

<LD>
<LR>

<L x1122334455667788>

*/

void setup() {
    Adapter::setup();
}

void loop() {
    Adapter::loop(); 
}