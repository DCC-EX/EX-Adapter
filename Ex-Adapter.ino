#include <Arduino.h>
#include "Adapter.h"

void setup() {
    Adapter::setup();
    // LCC setup
}

void loop() {
    Adapter::loop(); // Read any inbound
   // LCC::loop();
}