#include <HardwareSerial.h>

#define DEV_BL_PIN  7

void setup() {
  analogWrite(DEV_BL_PIN,0);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    char buffer[256];                 // Buffer to store input data
    size_t bufferSize = 0;            // Current size of data in buffer

    while (Serial.available() > 0) {
      char input = Serial.read();
      buffer[bufferSize++] = input;   // Store input in buffer
      // Check if the buffer is full, or a newline character is received
      if (bufferSize >= sizeof(buffer) || input == '\n') {
      // Send the entire buffer via Serial2
        Serial.println(buffer);
        delay(10);
      // Reset buffer and size for the next input
        bufferSize = 0;
        memset(buffer, 0, sizeof(buffer));
      }
    }
  }
}
