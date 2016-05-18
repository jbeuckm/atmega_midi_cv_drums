#include "Arduino.h"
#include "AD8804.h"


AD8804::AD8804(int clkPin, int sdiPin, int csPin, int shdnPin) {
    _clkPin = clkPin;
    pinMode(_clkPin, OUTPUT);
    _sdiPin = sdiPin;
    pinMode(_sdiPin, OUTPUT);
    _csPin = csPin;
    pinMode(_csPin, OUTPUT);
    _shdnPin = shdnPin;
    pinMode(_shdnPin, OUTPUT);

	digitalWrite(_csPin, HIGH);
	digitalWrite(_clkPin, LOW);
	digitalWrite(_shdnPin, HIGH);
}


void AD8804::setValue(byte dacNumber, byte value) {

	digitalWrite(_clkPin, LOW);
	digitalWrite(_csPin, LOW);

	byte mask;

  for (mask = B1000; mask>0; mask >>= 1) {


    if (dacNumber & mask){
      digitalWrite(_sdiPin, HIGH);
    }
    else{
      digitalWrite(_sdiPin, LOW);
    }

	digitalWrite(_clkPin, HIGH);
    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);
  }

    
  for (mask = B10000000; mask>0; mask >>= 1) {


    if (value & mask){
      digitalWrite(_sdiPin, HIGH);
    }
    else{
      digitalWrite(_sdiPin, LOW);
    }

	digitalWrite(_clkPin, HIGH);
    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);

  }

    // register load happens at this time
	digitalWrite(_csPin, HIGH);
    delayMicroseconds(1);
    
}
