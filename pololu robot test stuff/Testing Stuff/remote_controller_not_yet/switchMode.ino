void switchMode() {
  mode ++;
  if (mode > modeCounter) { mode = 0; }
  delay(100); //small delay debounce (I hope)
}

