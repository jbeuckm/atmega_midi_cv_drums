#include <TLV5628.h>
#include <AD8804.h>
#include <MIDI.h>
#include <EEPROM.h>

#define CLK_PIN A0
#define SDI_PIN A1
#define CS_PIN A2
#define SHDN_PIN A3

#define GATE_LED 13

#define UNASSIGNED 127

AD8804 velocityDAC(CLK_PIN, SDI_PIN, CS_PIN, SHDN_PIN);

byte noteMap[128];

void loadNoteMap() {
  // clear initial map
  for (int i=0; i<128; i++) {
    noteMap[i] = UNASSIGNED;
  }
  // read map values
  for (int i=0; i<12; i++) {
    byte noteNumber = EEPROM.read(i);
    noteMap[noteNumber] = i;
  }
}

void saveMapping(byte dacChannel, byte noteNumber) {
  byte oldNoteNumber = EEPROM.read(dacChannel);
  noteMap[oldNoteNumber] = UNASSIGNED;
  noteMap[noteNumber] = dacChannel;
  EEPROM.write(dacChannel, noteNumber);
}

byte voice_gates[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
byte voice_accents[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

MIDI_CREATE_DEFAULT_INSTANCE();


void handleSystemExclusive(byte message[], unsigned size) {

  if (message[1] != 0x77) return;      // manufacturer ID
  if (message[2] != 0x08) return;      // model ID
  if (message[3] != 0x01) return;  // device ID

  byte dacIndex = message[4];
  byte midiNote = message[5];

  saveMapping(dacIndex, midiNote);
}


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  byte dacChannel = noteMap[pitch];
  if (dacChannel == UNASSIGNED) {
    return;
  }

  digitalWrite(GATE_LED, HIGH);

  voice_gates[dacChannel] = 2;
  voice_accents[dacChannel] = velocity << 1;
}


int i;

ISR(TIMER1_COMPA_vect) {
  for (i=0; i<12; i++) {
    
    if (voice_gates[i] > 1) {
      velocityDAC.setValue(i, voice_accents[i]);
      voice_gates[i] -= 1;
    }
    else if (voice_gates[i] == 1) {
      velocityDAC.setValue(i, 0);
      voice_gates[i] = 0;
      digitalWrite(GATE_LED, LOW);
    }
    
  }
}


// -----------------------------------------------------------------------------

void setup()
{
  loadNoteMap();
  
  pinMode(GATE_LED, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleSystemExclusive(handleSystemExclusive);

    cli();//stop interrupts
    
    //set timer1 interrupt at 1kHz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0;
    // set timer count for 1khz increments
    OCR1A = 1999;// = (16*10^6) / (1000*8) - 1
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS11 bit for 8 prescaler
    TCCR1B |= (1 << CS11);   
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    sei();//allow interrupts
    
    
    MIDI.begin(10);
}

void loop()
{
  MIDI.read();
}
