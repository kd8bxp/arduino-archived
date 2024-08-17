#include "M5UnitSynth.h"

M5UnitSynth synth;

void setup() {
synth.begin(&Serial2, UNIT_SYNTH_BAUD, 16, 17);
synth.setInstrument(0, 0, 1); // synth piano 1
}

void loop() {
synth.setNoteOn(0, 60, 127);
delay(1000);
synth.setNoteOn(0, 62, 127);
delay(1000);
synth.setNoteOn(0, 64, 127);
delay(1000);
synth.setNoteOn(0, 65, 127);
delay(1000);
synth.setNoteOn(0, 67, 127);
delay(1000);
synth.setNoteOn(0, 69, 127);
delay(1000);
synth.setNoteOn(0, 71, 127);
delay(1000);
synth.setNoteOn(0, 72, 127);
delay(1000);
}
