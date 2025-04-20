//Rotate animation when to tilt M5StickC.
//Prepare 7pcs 80x80 image files and convert to 2byte(RRRRRGGGGGBBBBB) x 7 x 6400(=80x80) array.

//M5SticC を傾けて画像を回転させるプログラム
//7個の80x80の画像データを用意して2byte(RRRRRGGGGGBBBBB) x 7 x 6400(=80x80)の配列に変換してください.

#include "src/M5StickC/M5StickC.h"
#include "img.c"//イメージデータの場所//image data

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;
int xpos = 40;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.IMU.Init();//加速度を有効//Acceleromater enable
  
  M5.Axp.ScreenBreath(9);//画面輝度を設定(7~15)//Brightness
  M5.Lcd.setRotation(3);//画面を横向きに//DisplayRotation
  M5.Lcd.fillScreen(0x0000);//画面を黒で塗りつぶし//Fill screen Black
  M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[3]);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  accY = accY-0.0;//補正(必要なら)//Calibration(if necessary)
  if(accY<-0.2||accY>0.2){
    int moveSpeed = -int(floor(accY*10/2));
    //左に傾いているとき//When to tilt left side
    if(accY>0.1){
      xpos = max(0,xpos+moveSpeed);
      M5.Lcd.fillRect(xpos+80,0,-moveSpeed,80,0x0000);
    }
    //右に傾いているとき//When to tilt right side
    else if(accY<-0.1){
      xpos = min(79,xpos+moveSpeed);
      M5.Lcd.fillRect(xpos-moveSpeed,0,moveSpeed,80,0x0000);
    }
    if(xpos<7){
      M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[0]);
    }
    else if(xpos<20){
      M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[1]);
    }
    else if(xpos<33){
      M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[2]);
    }
    else if(xpos<47){
      M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[3]);
    }
    else if(xpos<60){
      M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[4]);
    }
    else if(xpos<73){
      M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[5]);
    }
    else{
      M5.Lcd.drawBitmap(xpos, 0, 80, 80, img[6]);
    }
  }
}
