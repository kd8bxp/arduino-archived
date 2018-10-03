#include <Streaming.h>

/* 
 *  https://yoursunny.com/t/2016/WittyCloud-first/
 *  
 *  GPIO#2 controls the tiny blue LED next to the WiFi antenna. HIGH turns off the LED, and LOW turns on the LED.
    GPIO#15 controls red of the RGB LED. HIGH turns on the color, and LOW turns off the color.
    GPIO#12 controls green of the RGB LED. HIGH turns on the color, and LOW turns off the color.
    GPIO#13 controls blue of the RGB LED. HIGH turns on the color, and LOW turns off the color.
*/


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial << "setup\n";
}

void blink(int pin) {
  Serial << "analogRead(A0)=" << analogRead(A0) << "\n";

  pinMode(pin, OUTPUT);
  Serial << "pinMode(" << pin << ",OUTPUT)\n";
  delay(1000);

  digitalWrite(pin, 0);
  Serial << "digitalWrite(" << pin << ",0)\n";
  delay(1000);

  digitalWrite(pin, 1);
  Serial << "digitalWrite(" << pin << ",1)\n";
  delay(1000);

  for (int pct = 10; pct <= 90; pct += 10) {
    analogWrite(pin, static_cast<int>(PWMRANGE * 0.01 * pct));
    Serial << "analogWrite(" << pin << "," << pct << "%)\n";
    delay(1000);
  }

  analogWrite(pin, 0);
}

void loop() {

  blink(2);
  blink(12); //green
  blink(13); //blue
  blink(15); //red

}
