
#include "src/M5Stack/M5Stack.h"
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "M5Bala2.h"
#include "picture_down.h"
#include "picture_left.h"
#include "picture_right.h"
#include "picture_stop.h"
#include "picture_up.h"

const char ssid[] = "ESP32";
const char password[] = "esp32pass";
const int localPort = 10000;
Preferences preferences;
M5Bala2 m5bala(Wire);
WiFiUDP udp;

boolean but_A = false;
boolean but_B = false;
boolean but_UP = false;
boolean but_DOWN = false;
boolean but_LEFT = false;
boolean but_RIGHT = false;

void ClearKeys() {
  but_A = false;
  but_B = false;
  but_UP = false;
  but_DOWN = false;
  but_LEFT = false;
  but_RIGHT = false;
}

bool KeyPadLoop() {
  if (udp.parsePacket()) {
    char r = udp.read();
    if (r == 'z') {
      ClearKeys();
      but_A = true;
    }  // else but_A=false;
    if (r == 'x') {
      ClearKeys();
      but_B = true;
    }  // else but_B=false;
    if (r == '8') {
      ClearKeys();
      but_UP = true;
    }  // else but_UP=false;
    if (r == '2') {
      ClearKeys();
      but_DOWN = true;
    }  // else but_DOWN=false;
    if (r == '4') {
      ClearKeys();
      but_LEFT = true;
    }  // else but_LEFT=false;
    if (r == '6') {
      ClearKeys();
      but_RIGHT = true;
    }  // else but_RIGHT=false;
    return true;
  }
  return false;
}

void auto_tune_gyro_offset() {
  M5.Speaker.tone(500, 200);
  delay(300);
  M5.update();
  delay(2000);
  m5bala.imu->calcGyroOffsets(true);
  float gyroXoffset = m5bala.imu->getGyroXoffset();
  float gyroYoffset = m5bala.imu->getGyroYoffset();
  float gyroZoffset = m5bala.imu->getGyroZoffset();
  preferences.putFloat("gyroXoffset", gyroXoffset);
  preferences.putFloat("gyroYoffset", gyroYoffset);
  preferences.putFloat("gyroZoffset", gyroZoffset);
  preferences.end();
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  delay(500);
  M5.begin();
  Wire.begin();
  Wire.setClock(400000UL);  // Set I2C frequency to 400kHz
  delay(500);
  m5bala.begin();
  if (M5.BtnC.isPressed()) {
    preferences.begin("m5bala-cfg", false);
    auto_tune_gyro_offset();
  } else {
    preferences.begin("m5bala-cfg", true);
    m5bala.imu->setGyroOffsets(preferences.getFloat("gyroXoffset"),
                               preferences.getFloat("gyroYoffset"),
                               preferences.getFloat("gyroZoffset"));
  }
  WiFi.softAP(ssid, password);
  //initWiFi();
  udp.begin(localPort);
  M5.Lcd.drawBitmap(40, 0, 240, 240, (uint8_t*)picture_stop);
}

bool dummy() {
  static int loop_interval = 0;
  static int phase = -1;
  if (micros() >= loop_interval) {
    loop_interval = micros() + 5000000;
    phase = (phase + 1) % 8;
    switch (phase) {
      case 0:
        but_A = true;
        break;
      case 1:
        but_UP = true;
        break;
      case 2:
        but_A = true;
        break;
      case 3:
        but_DOWN = true;
        break;
      case 4:
        but_A = true;
        break;
      case 5:
        but_RIGHT = true;
        break;
      case 6:
        but_A = true;
        break;
      case 7:
        but_LEFT = true;
        break;
    }
    return true;
  }
  return false;
}

void loop() {
  m5bala.run();
  M5.update();

  bool received = KeyPadLoop();
  // received = dummy();
  if (received) {
    if (but_LEFT) {
      but_LEFT = false;
      M5.Lcd.drawBitmap(40, 0, 240, 240, (uint8_t*)picture_left);
      m5bala.stop();
      m5bala.rotate(-50);
      m5bala.move(50);  // for adjust
    } else if (but_RIGHT) {
      but_RIGHT = false;
      M5.Lcd.drawBitmap(40, 0, 240, 240, (uint8_t*)picture_right);
      m5bala.stop();
      m5bala.rotate(50);
      m5bala.move(0);  // for adjust
    } else if (but_UP) {
      but_UP = false;
      M5.Lcd.drawBitmap(40, 0, 240, 240, (uint8_t*)picture_up);
      m5bala.stop();
      m5bala.move(60);
    } else if (but_DOWN) {
      but_DOWN = false;
      M5.Lcd.drawBitmap(40, 0, 240, 240, (uint8_t*)picture_down);
      m5bala.stop();
      m5bala.move(-80);
    } else if (but_A) {
      but_A = false;
      M5.Lcd.drawBitmap(40, 0, 240, 240, (uint8_t*)picture_stop);
      m5bala.stop();
    }
  }
}
