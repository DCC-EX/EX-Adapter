#include <Arduino.h>
#include "Adapter.h"

/* Sample DCC-EX Commands To Execute for Testing

<LL 0 x0201570002B00302>
<LL 1 x0201570002B00303>
<LL 2 x1122334455667788>
<LS x1122334455667788>
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