#include <MIDI.h>
#include "TLV5628.h"

#define CLK_PIN 17
#define LOAD_PIN 16
#define DATA_PIN 17
#define LDAC_PIN 17

TLV5628 velocityDAC();

MIDI_CREATE_DEFAULT_INSTANCE();




void handleNoteOn(byte channel, byte pitch, byte velocity)
{

  velocityDACs[nextNoteOutput].setValue(velocity * 32);

  digitalWrite(gatePins[nextNoteOutput], HIGH);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{  

}

 

// -----------------------------------------------------------------------------

void setup()
{

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  MIDI.begin(10);
}

void loop()
{
  MIDI.read();
}

