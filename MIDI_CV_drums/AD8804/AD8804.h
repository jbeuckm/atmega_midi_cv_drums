/*
  Library for controlling the Analog Devices 12-Channel DAC
  
  http://www.analog.com/media/en/technical-documentation/data-sheets/AD8802_8804.pdf
  
*/
#ifndef AD8804_h
#define AD8804_h

#include "Arduino.h"

class AD8804
{

  public:
    AD8804(int clkPin, int sdiPin, int csPin, int shdnPin);
    void setValue(byte dacNumber, byte value);

  private:
    int _clkPin, _sdiPin, _csPin, _shdnPin;
    
};

#endif