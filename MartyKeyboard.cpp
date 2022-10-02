#include "MartyKeyboard.h"


MartyKeyboard::MartyKeyboard(uint8_t i2c_addr
                             , uint8_t octave)
{
  _mcp.begin_I2C(i2c_addr);
  _octave = octave;
  //Define pins 0 to 11 (key pins) as pullup
  for (int i = 0; i < 12; i++) {
    _mcp.pinMode(i, INPUT_PULLUP);
  }
  //define pins 12-15 (led pins) as output
  for (int i = 12; i < 16; i++) {
    _mcp.pinMode(i, OUTPUT);
  }
  //mode vars
  _discoCount = 0;

  //modes
  _discoEnabled = false;


}

void MartyKeyboard::poll()
{

  _currentState = (_mcp.readGPIOB() << 8) | _mcp.readGPIOA();

  uint16_t added =   ~_currentState &  _previousState;
  uint16_t removed =  _currentState & ~_previousState;

  _latestNote = -1;
  for (int i = 0; i < 12; i++) {
    uint8_t note = i + _octave * 12;



    if (added >> i & 1 == 1) {
      _noteOn(0, note, 64);
      _latestNote = note;

      for (uint8_t j = 0; j < sizeof(_chord); j++) {
        if (_chord[j] != 0) {
          _noteOn(0, note + _chord[j], 64);
        }
      }
    }

    if (removed >> i & 1 == 1) {
      _noteOff(0, note, 64);

      for (uint8_t j = 0; j < sizeof(_chord); j++) {
        if (_chord[j] != 0) {
          _noteOff(0, note + _chord[j], 64);
        }
      }

    }

  }



  if (_discoEnabled) {
    _printBinLED(_discoCount);
  }
  else {
    _printBinLED(0);
  }

  _previousState = _currentState;
}

void MartyKeyboard::setMode(Mode mode
                            , bool enabled) {
  if (mode == Mode::DISCO && _discoEnabled != enabled) {
    _discoEnabled = enabled;
  }

}

void MartyKeyboard::setChord(uint8_t chord[]) {

  for (uint8_t i = 0; i < sizeof(_chord); i++) {
    _chord[i] = 0;
  }

  for (uint8_t i = 0; i < sizeof(chord); i++) {
    _chord[i] = chord[i];
  }


}

void MartyKeyboard::_noteOn(byte channel, byte pitch, byte velocity) {
  //1st parameter = code? 1001 or 0x09 like a mask for the second parameter
  //2nd parameter = channel voice message, 1001nnnnn or 0x90 | nnnn means note on on nnnn channel
  //3rd parameter = pitch or note number
  //4th parameter = velocity
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};

  _discoCount++;
  MidiUSB.sendMIDI(noteOn);
}

void MartyKeyboard::_noteOff(byte channel, byte pitch, byte velocity) {

  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};

  MidiUSB.sendMIDI(noteOff);
}
void MartyKeyboard::_allNotesOff() {
  midiEventPacket_t noteOff = {0x0B, 0xB0, 0x7B, 0x00};
  MidiUSB.sendMIDI(noteOff);
}

void MartyKeyboard::_printBinLED(uint8_t data) {
  _mcp.digitalWrite(12, (bool)(data & 1));
  _mcp.digitalWrite(13, (bool)(data & 2));
  _mcp.digitalWrite(14, (bool)(data & 4));
  _mcp.digitalWrite(15, (bool)(data & 8));
}

uint8_t MartyKeyboard::getLatestNote() {
  return _latestNote;
}