#ifndef MartyKeyboard_h
#define MartyKeyboard_h

#include <Adafruit_MCP23X17.h>
#include <MIDIUSB.h>



class MartyKeyboard
{
  public:
    MartyKeyboard(uint8_t i2c_addr);
    void poll();
  private:
    Adafruit_MCP23X17 _mcp;
    uint8_t _currentState;
    uint8_t _previousState;
};

#endif