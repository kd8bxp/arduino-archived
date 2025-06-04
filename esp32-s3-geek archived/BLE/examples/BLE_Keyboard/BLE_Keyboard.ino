/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include "BleKeyboard.h"

BleKeyboard bleKeyboard("ESP32-S3-GEEK", "Waveshare", 100);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending 'Waveshare'...");
    bleKeyboard.print("Waveshare");

    delay(500);

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);

    delay(500);

    Serial.println("Sending Ctrl+Alt+Delete...");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_ALT);
    bleKeyboard.press(KEY_DELETE);
    delay(100);
    bleKeyboard.releaseAll();
  }

  Serial.println("Waiting 5 seconds...");
  delay(5000);
}
