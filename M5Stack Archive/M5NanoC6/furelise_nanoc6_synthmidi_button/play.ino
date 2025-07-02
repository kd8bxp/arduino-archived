void play() {

// iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    //M5.Speaker.tone(melody[thisNote], noteDuration * 0.9);
synth.setInstrument(0, 0, instrument);
synth.setNoteOn(0, melody[thisNote], 127); //noteDuration * 0.9);
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    //noTone(buzzer);
    synth.setNoteOff(0, melody[thisNote], 127);
    NanoC6.update();
    if (NanoC6.BtnA.wasPressed()) {
      digitalWrite(M5NANO_C6_BLUE_LED_PIN, LOW);
        instrument++;
        if (instrument > 127) {instrument = 1; }
    }
    digitalWrite(M5NANO_C6_BLUE_LED_PIN, HIGH);
    //M5.Speaker.mute();
  }
}
