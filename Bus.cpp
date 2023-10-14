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
#include <SPIFFS.h>
#include <esp_spi_flash.h>

#define USE_TWAI

#include "config.h"

/// ConfigDef comes from config.h and is specific to this particular device and
/// target. It defines the layout of the configuration memory space and is also
/// used to generate the cdi.xml file. Here we instantiate the configuration
/// layout. The argument of offset zero is ignored and will be removed later.
static constexpr openlcb::ConfigDef cfg(0);

namespace openlcb
{
    // Name of CDI.xml to generate dynamically.
    const char CDI_FILENAME[] = "/spiffs/cdi.xml";

    // This will stop openlcb from exporting the CDI memory space upon start.
    extern const char CDI_DATA[] = "";

    // Path to where OpenMRN should persist general configuration data.
    extern const char *const CONFIG_FILENAME = "/spiffs/openlcb_config";

    // The size of the memory space to export over the above device.
    extern const size_t CONFIG_FILE_SIZE = cfg.seg().size() + cfg.seg().offset();

    // Default to store the dynamic SNIP data is stored in the same persistant
    // data file as general configuration data.
    extern const char *const SNIP_DYNAMIC_FILENAME = CONFIG_FILENAME;
}

using namespace openlcb;

const gpio_num_t CAN_RX_PIN = GPIO_NUM_4;
const gpio_num_t CAN_TX_PIN = GPIO_NUM_5;
Esp32HardwareTwai twai(CAN_RX_PIN, CAN_TX_PIN);
    
const uint64_t NODE_ID = UINT64_C(0x05010101DCCE);  // TODO make configurable
OpenMRN openmrn(NODE_ID);
CallbackEventHandler * eventHandler;

/// Event handler called automatically for incomin event of interest.
/// The user_bits are the Adapter-chosen id number to save a re-lookup.
void event_callback(const EventRegistryEntry &registry_entry,
                    EventReport *report, BarrierNotifiable *done) {
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
    printf("Bus::ready()\n");
      // Initialize the SPIFFS filesystem as our persistence layer
    if (!SPIFFS.begin())
    {
        printf("SPIFFS failed to mount, attempting to format and remount\n");
        if (!SPIFFS.begin(true))
        {
            printf("SPIFFS mount failed even with format, giving up!\n");
            while (1)
            {
                // Unable to start SPIFFS successfully, give up and wait
                // for WDT to kick in
            }
        }
    }

#if defined(USE_TWAI)
    twai.hw_init();
#endif // USE_TWAI

    // Create the CDI.xml dynamically
    openmrn.create_config_descriptor_xml(cfg, openlcb::CDI_FILENAME);

    // Create the default internal configuration file
    openmrn.stack()->create_config_file_if_needed(cfg.seg().internal_config(),
        openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
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



