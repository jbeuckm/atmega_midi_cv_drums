#include "Arduino.h"
#include "TLV5628.h"


TLV5628::TLV5628(int clkPin, int dataPin, int loadPin, int ldacPin) {
    _clkPin = clkPin;
    pinMode(_clkPin, OUTPUT);
    _dataPin = dataPin;
    pinMode(_dataPin, OUTPUT);
    _loadPin = loadPin;
    pinMode(_loadPin, OUTPUT);
    _ldacPin = ldacPin;
    pinMode(_ldacPin, OUTPUT);

	digitalWrite(_ldacPin, LOW);
	digitalWrite(_clkPin, LOW);
	digitalWrite(_loadPin, HIGH);
}


void TLV5628::setValue(byte dacNumber, byte value) {

	digitalWrite(_ldacPin, LOW);
	digitalWrite(_clkPin, LOW);
	digitalWrite(_loadPin, HIGH);

	byte mask;

  for (mask = B100; mask>0; mask >>= 1) {

	digitalWrite(_clkPin, HIGH);

    if (dacNumber & mask){
      digitalWrite(_dataPin, HIGH);
    }
    else{
      digitalWrite(_dataPin, LOW);
    }

    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);
  }


	// clock in the RNG bit (x2 output voltage)
	digitalWrite(_clkPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(_dataPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
	delayMicroseconds(1);

    
  for (mask = B10000000; mask>0; mask >>= 1) {

	digitalWrite(_clkPin, HIGH);

    if (value & mask){
      digitalWrite(_dataPin, HIGH);
    }
    else{
      digitalWrite(_dataPin, LOW);
    }

    delayMicroseconds(1);
	digitalWrite(_clkPin, LOW);
    delayMicroseconds(1);

  }

      digitalWrite(_dataPin, LOW);
    delayMicroseconds(1);


	digitalWrite(_loadPin, LOW);
    delayMicroseconds(1);
	digitalWrite(_loadPin, HIGH);
    delayMicroseconds(1);
    
}

