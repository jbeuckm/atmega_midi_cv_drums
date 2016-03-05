atmega_midi_cv_drums
========================

Uses TLV5628 octal DAC to generate eight velocity CV signals. A CMOS addressable latch (CD4099) generates eight gate signals.

Euro power connectors distribute the gate and CV signals for eight drum voices.

MIDI notes start at 100 so we can use channel 10 without overlapping the General MIDI percussion map.

![screenshot](schemo.png)

