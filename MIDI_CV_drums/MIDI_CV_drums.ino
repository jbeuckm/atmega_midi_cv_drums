#include <MIDI.h>
#include "TLV5628.h"

#define CLK_PIN 7
#define LOAD_PIN 10
#define DATA_PIN 8
#define LDAC_PIN 9

#define GATE_LATCH_A0 A2
#define GATE_LATCH_A1 A1
#define GATE_LATCH_A2 A0
#define GATE_LATCH_ENABLE A4
#define GATE_LATCH_CLEAR A3
#define GATE_LATCH_DATA A5

TLV5628 velocityDAC();

MIDI_CREATE_DEFAULT_INSTANCE();

void setLatchPin(byte pin, bool data)
{
  
}


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (pitch < 100) return;
  if (pitch > 107) return;

  int voice = pitch - 100;

  velocityDAC.setValue(voice, velocity << 1);

  setLatchPin(voice, HIGH);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{  
  if (pitch < 100) return;
  if (pitch > 107) return;

  int voice = pitch - 100;

  velocityDAC.setValue(voice, velocity << 1);

  setLatchPin(voice, LOW);
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

