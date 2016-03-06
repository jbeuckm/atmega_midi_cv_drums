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
}


void TLV5628::send(int dacNumber, char value) {
    
}

