#ifndef MartyKeyboard_h
#define MartyKeyboard_h

#include <Adafruit_MCP23X17.h>
#include <MIDIUSB.h>



class MartyKeyboard
{
public:
  enum class Mode {
    CHORD = 1,
    DISCO = 2

  };
  MartyKeyboard(uint8_t i2c_addr, uint8_t octave);
  void poll();
  uint8_t getLatestNote();
  void setMode(Mode mode, bool enabled);
  void setChord(uint8_t chord[]);

private:

  Adafruit_MCP23X17 _mcp;
  uint16_t _currentState;
  uint16_t _previousState;
  uint8_t _octave;
  uint8_t _latestNote;
  //mode valuyes
  uint8_t _discoCount;
  uint8_t _chord[8];

  //modes
  bool _discoEnabled;

  void _translateToMidi(uint16_t mcpData, uint16_t prevMcpData);
  void _noteOn(byte channel, byte pitch, byte velocity);
  void _noteOff(byte channel, byte pitch, byte velocity);
  void _allNotesOff();
  void _printBinLED(uint8_t data);

};

#endif