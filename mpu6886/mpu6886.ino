#include "src/M5Atom/M5Atom.h"

int oldX, oldY;

void setup() {
  M5.begin(true, false, true);
  M5.IMU.Init();
  oldX = -1;
  oldY = -1;
}

void loop() {
  float accX, accY, accZ;
  int x, y;

  M5.IMU.getAccelData(&accX, &accY, &accZ);
  x = constrain(accX * 5 + 2, 0, 4);
  y = constrain(accY * 5 + 2, 0, 4);
  if (oldX != x || oldY != y) {
    M5.dis.drawpix(oldX, oldY, 0x000000);
    M5.dis.drawpix(x, y, 0xffffff);
    oldX = x;
    oldY = y;
  }
}
