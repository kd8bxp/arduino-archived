//#include "src/M5Cardputer/M5Cardputer.h"
#include <M5Cardputer.h>

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.setTextFont(&fonts::Orbitron_Light_32);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.drawString("Buzzer Test",
                                   M5Cardputer.Display.width() / 2,
                                   M5Cardputer.Display.height() / 2);
}

void loop() {
    M5Cardputer.Speaker.tone(10000,100);
    delay(100);
    M5Cardputer.Speaker.tone(4000,20);
    delay(10);
}
