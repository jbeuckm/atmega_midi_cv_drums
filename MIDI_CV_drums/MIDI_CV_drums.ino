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

TLV5628 velocityDAC(CLK_PIN, DATA_PIN, LOAD_PIN, LDAC_PIN);

// this corrects since the layout doesn't maintain voice order for the DAC output lines
byte dacMap[] = {0, 1, 2, 3, 7, 6, 5, 4};

byte voice_gates[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
byte voice_accents[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

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

  byte voice = pitch - 100;

  voice_gates[voice] = 2;
  voice_accents[voice] = velocity << 1;

}


int i;

ISR(TIMER1_COMPA_vect) {
  for (i=0; i<8; i++) {
    
    if (voice_gates[i] > 1) {
      velocityDAC.setValue(dacMap[i], voice_accents[i]);
      setLatchPin(i, HIGH);
      voice_gates[i] -= 1;
    }
    else if (voice_gates[i] == 1) {
      setLatchPin(i, LOW);
      velocityDAC.setValue(dacMap[i], 0);
      voice_gates[i] = 0;
      digitalWrite(GATE_LED, LOW);
    }
    
  }
}


// -----------------------------------------------------------------------------

void setup()
{
  initLatch(GATE_LATCH_A0, GATE_LATCH_A1, GATE_LATCH_A2, 
  GATE_LATCH_ENABLE, GATE_LATCH_CLEAR, GATE_LATCH_DATA);

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

