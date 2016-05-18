#include <MIDI.h>
#include "AD8804/AD8804.h"

#define CLK_PIN A0
#define SDI_PIN A1
#define CS_PIN A2
#define SHDN_PIN A3

#define GATE_LED 13

AD8804 velocityDAC(CLK_PIN, SDI_PIN, CS_PIN, SHDN_PIN);

// this corrects since the layout doesn't maintain voice order for the DAC output lines
byte dacMap[] = {0, 1, 2, 3, 7, 6, 5, 4, 8, 9, 10, 11};

byte voice_gates[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
byte voice_accents[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

MIDI_CREATE_DEFAULT_INSTANCE();


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (pitch < 100) return;
  if (pitch > 111) return;

  digitalWrite(GATE_LED, HIGH);

  byte voice = pitch - 100;

  voice_gates[voice] = 2;
  voice_accents[voice] = velocity << 1;

}


int i;

ISR(TIMER1_COMPA_vect) {
  for (i=0; i<12; i++) {
    
    if (voice_gates[i] > 1) {
      velocityDAC.setValue(dacMap[i], voice_accents[i]);
      voice_gates[i] -= 1;
    }
    else if (voice_gates[i] == 1) {
      velocityDAC.setValue(dacMap[i], 0);
      voice_gates[i] = 0;
      digitalWrite(GATE_LED, LOW);
    }
    
  }
}


// -----------------------------------------------------------------------------

void setup()
{
  pinMode(GATE_LED, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);

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

