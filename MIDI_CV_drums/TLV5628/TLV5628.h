/*
  Library for controlling the Texas Instruments octal DAC
*/
#ifndef TLV5628_h
#define TLV5628_h

#include "Arduino.h"

class TLV5628
{

  public:
    TLV5628(int clkPin, int dataPin, int loadPin, int ldacPin);
    void setValue(byte dacNumber, byte value);

  private:
    int _clkPin, _dataPin, _loadPin, _ldacPin;
    
};

#endif