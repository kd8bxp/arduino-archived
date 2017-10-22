void checkMode() {
  /*
  if (mode == 0) { mode0(); }
  if (mode == 1) { mode1(); }
  if (mode == 2) { mode2(); }
  yield();
  */
  yield();
   switch (mode) {
    case 0: 
      mode0();
      break;
     case 1:
      mode1();
      break;
     case 2:
      mode2();
      break;
     default:
      //mode error, so maybe I should just stop everything.
      stop();
      break;
  }
}

