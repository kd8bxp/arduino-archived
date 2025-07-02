#include "src/M5Atom/M5Atom.h"

/* this Example only for M5Atom Matrix */
//externconst unsigned char  AtomImageData[375+2];
extern const unsigned char AtomImageData[2+(4*6+1)*5*3]; //(4文字＊6ドット+1ドット空き)*5ラスタ*3RGB

bool IMU6886Flag = false;
float accX = 0, accY = 0, accZ = 0;

void setup()
{
  M5.begin(true, false, true);
  if (M5.IMU.Init() != 0)
    IMU6886Flag = false;
  else
    IMU6886Flag = true;
  //    M5.dis.animation((uint8_t *)AtomImageData, 5000, LED_Display::kMoveLeft, 18);
  M5.dis.displaybuff((uint8_t *)AtomImageData, 0, 0);
  delay(1000);
}

int wait = 1;

void loop()
{
  int i = 100;
  while (i-- > 0) {
    M5.dis.displaybuff((uint8_t *)AtomImageData, i, 0);
//    if (M5.Btn.wasPressed()) {
//      wait *= 2;
//    }
    if (M5.Btn.pressedFor(100)) { //ボタン長押しでスクロール一時停止し、スクロール速度も遅くする
      wait *= 2;
      while (1) {
        delay(50);
        M5.update();
        if (M5.Btn.wasReleased()) break;
      }
    }
    if (IMU6886Flag == true){
      M5.IMU.getAccelData(&accX, &accY, &accZ);
        Serial.printf("%.2f,%.2f,%.2f mg\r\n", accX * 1000, accY * 1000, accZ * 1000);
//        M5.IMU.getAccelData(&accX, &accY, &accZ);
//        Serial.printf("%.2f,%.2f,%.2f o/s \r\n", gyroX, gyroY, gyroZ);
//        M5.IMU.getTempData(&temp);
//        Serial.printf("Temperature : %.2f C \r\n", temp);
      while (accZ > 0.0) { //LED表示面を下向きにすると、スクロール一時停止
        delay(100);
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        Serial.printf("%.2f,%.2f,%.2f mg\r\n", accX * 1000, accY * 1000, accZ * 1000);
      }
      delay(wait);
      M5.update();
    }
  }
}
