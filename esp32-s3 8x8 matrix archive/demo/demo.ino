#define PIN_NEOPIXEL 14
void setup() {
  // No need to initialize the RGB LED
}
// the loop function runs over and over again forever
void loop() {
  neopixelWrite(PIN_NEOPIXEL, RGB_BRIGHTNESS, 0, 0);  // Red
  delay(1000);
  neopixelWrite(PIN_NEOPIXEL, 0, RGB_BRIGHTNESS, 0);  // Green
  delay(1000);
  neopixelWrite(PIN_NEOPIXEL, 0, 0, RGB_BRIGHTNESS);  // Blue
  delay(1000);
  neopixelWrite(PIN_NEOPIXEL, 0, 0, 0);  // Off / black
  delay(1000);
}
