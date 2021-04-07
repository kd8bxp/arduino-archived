/*
 * Tilt and Draw
 * Board: Arduino Esplora
 *
 * David Siver
 * CS 4985
 * Fall 2015
 */

#include <Esplora.h>
#include <SPI.h>
#include <TFT.h>

#define DEBUG 
#define serialBaudRate 9600
#define xMin 0
#define xMax EsploraTFT.width() - 1
#define yMin 0
#define yMax EsploraTFT.height() - 1
#define low -100
#define high 100
#define sensitivity 20
#define pixels 5
#define oneButtonPressed 3
#define sideLength 3

int xPos = EsploraTFT.width() / 2;
int yPos = EsploraTFT.height() / 2;
int oldXPos = xPos;
int oldYPos = yPos;
boolean canDraw = false;

void setup() {
  Serial.begin(serialBaudRate);
  EsploraTFT.begin();
  EsploraTFT.background(0, 0, 0);
}

void loop() {
  canDraw = false;
  int xAxis = Esplora.readAccelerometer(X_AXIS);
  int yAxis = Esplora.readAccelerometer(Y_AXIS);
  setPosition(xAxis, yAxis);
  checkPosition();
  draw();
  if(abs(Esplora.readAccelerometer(X_AXIS))>200 || abs(Esplora.readAccelerometer(Y_AXIS))>200){
    EsploraTFT.background(0, 0, 0);
  }
  delay(100);
}

void draw() {
  EsploraTFT.fill(255, 255, 255); // white
  EsploraTFT.rect(xPos, yPos, sideLength, sideLength);
  int upButtonState = Esplora.readButton(SWITCH_UP);
  int downButtonState = Esplora.readButton(SWITCH_DOWN);
  int leftButtonState = Esplora.readButton(SWITCH_LEFT);
  int rightButtonState = Esplora.readButton(SWITCH_RIGHT);
  int buttonStateSum = upButtonState + downButtonState + leftButtonState + rightButtonState;
  if (buttonStateSum == oneButtonPressed) {
    canDraw = true;
  }
  if (oldXPos != xPos || oldYPos != yPos) {
    if (canDraw) {
      if (upButtonState == LOW) {
        EsploraTFT.fill(0, 255, 255); // yellow
      }
      else if (downButtonState == LOW) {
        EsploraTFT.fill(0, 255, 0); // green
      }
      else if (leftButtonState == LOW) {
        EsploraTFT.fill(255, 0, 0); // blue
      }
      else if (rightButtonState == LOW) {
        EsploraTFT.fill(0, 0, 255); // red
      }
    }
    else {
      EsploraTFT.fill(0, 0, 0);
    }
    EsploraTFT.rect(oldXPos, oldYPos, sideLength, sideLength);
  }
  oldXPos = xPos;
  oldYPos = yPos;
}

void setPosition(int xAxis, int yAxis) {
  if (xAxis < sensitivity && xAxis > -sensitivity) {
    xPos = xPos;
  }
  else {
    xPos += map(xAxis, low, high, pixels, -pixels);
  }
  if (yAxis < sensitivity && yAxis > -sensitivity) {
    yPos = yPos;
  }
  else {
    yPos += map(yAxis, low, high, -pixels, pixels);
  }
}

void checkPosition() {
  if (xPos < xMin) {
    (xPos = xMin);
  }
  if (xPos > xMax) {
    (xPos = xMax);
  }
  if (yPos < yMin) {
    (yPos = yMin);
  }
  if (yPos > yMax) {
    (yPos = yMax);
  }
}