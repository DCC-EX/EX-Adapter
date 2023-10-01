#include <Arduino.h>
#include "Adapter.h"

/* Sample DCC-EX Commands To Execute for Testing

<LL 0 x0201570002B00302>
<LL 1 x0201570002B00303>
<LS x1122334455667788>
<LD>
<LR>

*/

void setup() {
    Adapter::setup();
}

void loop() {
    Adapter::loop(); 
}