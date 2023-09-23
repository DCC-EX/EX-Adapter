/* Code below is based on this message from Balazs Racz
  
  In the software stack I am most familiar with (openmrn),
  there are APIs like
    send_event(uint64_t event_id) 
    and a class called CallbackEventHandler 
    with an event handler callback function on the constructor 
    and add_entry to register your interest in events. 

    You must register both outgoing events (those that you send, 
    i.e., you are the producer of them, 
    as well as those that you want to receive, 
    i.e., you are the consumer of them).
*/
/// Functions below are the simple shim from the Adapter.
/// All openmrn/openlcb references are confined to this file.

/// Sections of this experimantal code have been "borrowed" from:
///  https://github.com/openmrn/OpenMRNLite/blob/master/examples/ESP32SerialBridge/ESP32SerialBridge.ino
/// full and appropriate copyright attribution will follow. 


#include "Bus.h"
#include <openMRNLite.h>
#include <openlcb/CallbackEventHandler.hxx>


const gpio_num_t CAN_RX_PIN = GPIO_NUM_4;
const gpio_num_t CAN_TX_PIN = GPIO_NUM_5;
const uint64_t NODE_ID = UINT64_C(0x05010101DCCE);
OpenMRN openmrn(NODE_ID);

openlcb::CallbackEventHandler * eventHandler;

// 16bit events sent by EXRAIL with no sender have our ID attached.
#define OWN_EVENT(e) ((e & UINT64_C(0xFFFFFFFFFFFF0000))? e: e | (NODE_ID<<16))

/// START OF SETUP ROUTINES 

// Tell bus the callback function to invoke when a listened to event arrives.
// Must be called first.
void Bus::setCallback(EVENT_CALLBACK callback) {

    //// ????? HOW DO I DO THIS... 
    eventHandler=new openlcb::CallbackEventHandler(openmrn,callback,NULL);
}

// Tell bus list of all outbound events we may send 
void Bus::outboundEvents(uint64_t outbound[], int16_t count) {
    for (int i=0;i<count;i++) 
        eventHandler->add_entry(OWN_EVENT(outbound[i]),openlcb::CallbackEventHandler::RegistryEntryBits::IS_PRODUCER);
}
      
// Tell bus list of all sender:events that we are interested in
void Bus::inboundEvents(uint64_t inbound[], int16_t count) {
    for (int i=0;i<count;i++) 
        eventHandler->add_entry(inbound[i],openlcb::CallbackEventHandler::RegistryEntryBits::IS_CONSUMER);
}
     
// Tell bus we are config complete and ready to play. 
void Bus::ready() {
    // Add the hardware CAN device as a bridge
    openmrn.add_can_port(new Esp32HardwareCan("esp32can", CAN_RX_PIN, CAN_TX_PIN));
    openmrn.begin();
}

/// END OF SETUP ROUTINES 

// START OF RUNTIME ROUTINES

// Send an event on the bus
void Bus::sendEvent(uint64_t eventid) {

    // ??? what is the Node* parameter
    openlcb::send_event(NULL,OWN_EVENT(eventid)); 
}

// Arduino activity loop called repeatedly
void Bus::loop() {
      openmrn.loop();
}
