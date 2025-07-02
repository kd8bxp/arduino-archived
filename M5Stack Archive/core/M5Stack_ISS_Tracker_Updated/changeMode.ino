
void changeMode() {
  if (mode == 2) {mode = -1;}
  mode += 1;
  switch (mode) {
    case 0:
    //nothing needs done, display will update within 10 seconds.
    M5.Lcd.clearDisplay();
    M5.Lcd.drawString("UPDATING MAP ...", 80, 100,GFXFF);
    M5.Lcd.drawString("May take up to 10 seconds.", 47,118,GFXFF);
    M5.Lcd.display();
    break;
    case 1:
    displayPass();
    break;
    case 2:
    displayPPL();
    break;
    case 3:
    M5.Lcd.clearDisplay();
    M5.Lcd.fillScreen(0x0000);
    M5.Lcd.display();
    break;
    default:
    break;
  }
}


/*
 * Modes:
 * 0 - Map Display
 * 1 - Display Pass Predictions
 * 2 - Display People in Space
 */
