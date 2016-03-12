#include <MIDI.h>
#include "TLV5628/TLV5628.h"

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

#define GATE_LED 13

//TLV5628 velocityDAC();

MIDI_CREATE_DEFAULT_INSTANCE();

void initLatch(int A0, int A1, int A2, int ENABLE, int CLEAR, int DATA) 
{
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(CLEAR, OUTPUT);
  pinMode(DATA, OUTPUT);

  digitalWrite(ENABLE, HIGH);
  digitalWrite(CLEAR, HIGH);
  delayMicroseconds(1);
  digitalWrite(CLEAR, LOW);
}

void setLatchPin(int pin, bool data)
{  
  digitalWrite(GATE_LATCH_A0, (pin & B001));
  digitalWrite(GATE_LATCH_A1, ((pin >> 1) & B001));
  digitalWrite(GATE_LATCH_A2, ((pin >> 2) & B001));

  digitalWrite(GATE_LATCH_DATA, data);
  digitalWrite(GATE_LATCH_ENABLE, LOW);
  delayMicroseconds(1);
  digitalWrite(GATE_LATCH_ENABLE, HIGH);
}


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (pitch < 100) return;
  if (pitch > 107) return;

  digitalWrite(GATE_LED, HIGH);

  int voice = pitch - 100;

//  velocityDAC.setValue(voice, velocity << 1);

  setLatchPin(voice, HIGH);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{  
  if (pitch < 100) return;
  if (pitch > 107) return;

  digitalWrite(GATE_LED, LOW);

  int voice = pitch - 100;

//  velocityDAC.setValue(voice, velocity << 1);

  setLatchPin(voice, LOW);
}

 

// -----------------------------------------------------------------------------

void setup()
{
  initLatch(GATE_LATCH_A0, GATE_LATCH_A1, GATE_LATCH_A2, 
  GATE_LATCH_ENABLE, GATE_LATCH_CLEAR, GATE_LATCH_DATA);

  pinMode(GATE_LED, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  MIDI.begin(10);
}

void loop()
{
  MIDI.read();
}

