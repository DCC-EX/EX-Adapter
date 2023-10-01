/* Code below is based on help from Balazs Racz
   Full and appropriate copyright attribution will follow. 
  
NOTES: 
    Bus:: Functions below are the simple shim from the Adapter.
    All openmrn/openlcb references are confined to this file.
    
    The code contains markers //??? where I (chris) have no 
    idea if this is correct or not. 

*/


#include "Bus.h"
#include <OpenMRNLite.h>
#include <openlcb/CallbackEventHandler.hxx>
#include <executor/Notifiable.hxx>

using namespace openlcb;

const gpio_num_t CAN_RX_PIN = GPIO_NUM_4;
const gpio_num_t CAN_TX_PIN = GPIO_NUM_5;
Esp32HardwareTwai twai(CAN_RX_PIN, CAN_TX_PIN);
    
const uint64_t NODE_ID = UINT64_C(0x05010101DCCE);  // TODO make configurable
OpenMRN openmrn(NODE_ID);
CallbackEventHandler * eventHandler;

extern const char *const openlcb::SNIP_DYNAMIC_FILENAME = "Why do I need this"; //???

/// Event handler called automatically for incomin event of interest.
/// The user_bits are the Adapter-chosen id number to save a re-lookup.
void event_callback(const EventRegistryEntry &registry_entry,
                    EventReport *report, BarrierNotifiable *done) {
  AutoNotify an(done);   // <-- never forget this line! The stack will freeze if you forget it.
  uint32_t user_bits = registry_entry.user_arg & CallbackEventHandler::USER_BIT_MASK;
  Bus::adapterCallback(user_bits); // callback to the Adapter
}



// 16bit events (ie with no sender) sent by EXRAIL will have our ID attached.
#define OWN_EVENT(e) ((e & UINT64_C(0xFFFFFFFFFFFF0000))? e: e | (NODE_ID<<16))

/// START OF SETUP ROUTINES 
EVENT_CALLBACK Bus::adapterCallback;
// Tell bus the callback function to invoke when a listened to event arrives.
// Must be called first.
void Bus::setCallback(EVENT_CALLBACK _callback) {
    adapterCallback=_callback;
    eventHandler=new CallbackEventHandler(openmrn.stack()->node(),
           &event_callback, nullptr);
}

// Tell bus list of all outbound events we may send 
void Bus::outboundEvents(uint64_t outbound[], int16_t count) {
    for (int i=0;i<count;i++) 
        eventHandler->add_entry(OWN_EVENT(outbound[i]),
            CallbackEventHandler::RegistryEntryBits::IS_PRODUCER);
}
      
// Tell bus list of all sender:events that we are interested in
void Bus::inboundEvents(uint64_t inbound[], int16_t count) {
    // notice the array position (Adapter id) is set as the user_bits
    for (int i=0;i<count;i++) 
        eventHandler->add_entry(inbound[i], 
          i | CallbackEventHandler::RegistryEntryBits::IS_CONSUMER);
}
     
// Tell bus we are config complete and ready to play. 
void Bus::ready() {
    // Add the hardware CAN device as a bridge
    openmrn.add_can_port_async("/dev/twai/twai0");
    openmrn.begin();
    openmrn.start_executor_thread();
}

/// END OF SETUP ROUTINES 

// START OF RUNTIME ROUTINES

// Send an event on the bus
void Bus::sendEvent(uint64_t eventid) {
    send_event(openmrn.stack()->node(),OWN_EVENT(eventid)); 
}

// Arduino activity loop called repeatedly
void Bus::loop() {
    // no longer required as we have done openmrn.start_executor_thread()
    //  openmrn.loop();
}



