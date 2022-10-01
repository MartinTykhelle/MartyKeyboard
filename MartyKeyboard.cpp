#include "MartyKeyboard.h"


MartyKeyboard::MartyKeyboard(uint8_t i2c_addr)
{
  _mcp.begin_I2C(i2c_addr);

  //Define pins 0 to 11 (key pins) as pullup
  for (int i = 0; i < 12; i++) {
    _mcp.pinMode(i, INPUT_PULLUP);
  }
  //define pins 12-15 (led pins) as output 
  for (int i = 12; i < 16; i++) {
    _mcp.pinMode(i, OUTPUT);
  }


}

void MartyKeyboard::poll()
{
  
  uint16_t  _currentState = (_mcp.readGPIOB() << 8) | _mcp.readGPIOA();
  if (_currentState != _previousState){
    _mcp.digitalWrite(12,HIGH);

    delay(50);
        _mcp.digitalWrite(12, LOW);
    delay(50);
  }

  Serial.println(_currentState,BIN);
  Serial.println(_previousState,BIN);
  delay(1000);
  _previousState = _currentState;
}