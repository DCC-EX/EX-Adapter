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
      static void printHex(uint64_t value );
      static void eventHandler(uint16_t eventid);
    
    private:
    static char opcode;
    static byte params;
    static ParserState state;
    static uint64_t p[4];
    static bool ready; 
    static bool parse(char hot);
    static void processCommand();
    
};

#endif