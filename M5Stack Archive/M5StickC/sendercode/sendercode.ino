//from: https://lang-ship.com/blog/work/m5stickc-ir/#toc4

#include "src/M5StickC/M5StickC.h"
#include "src/IRremoteESP8266/IRremoteESP8266.h"
#include "src/IRremoteESP8266/IRsend.h"
#include "src/IRremoteESP8266/IRutils.h"
 
const uint16_t kIrLed = 9;              // M5StickCはGPIO9にIRが内蔵
 
IRsend irsend(kIrLed);                  // IR送信を宣言

 //Vizio Codes found: http://www.remotecentral.com/cgi-bin/forums/viewpost.cgi?925066
 //Vizio Device code 0x04FB
const uint32_t CUSTOMER_CODE = 0x04FB;  // カスタマーコードをセット
 
// リモコンコード保存用構造体
struct REMOTE {
  char name[9];
  uint8_t command;
};
 
// リモコンコード一覧
//my vizio appears to use the GV42-L code set

REMOTE remote[] = {
  { "OFF" , 0x25 },
  { "ON" , 0x2A },
  { "VOL+"  , 0x02 },
  { "VOL-"  , 0x03 },
  { "CH+", 0x00},
  { "CH-", 0x01},
  { "MUTE", 0x09},
  //{ "0"     , 0x10 },
  //{ "1"     , 0x11 },
  //{ "2"     , 0x12 },
  //{ "3"     , 0x13 },
  //{ "4"     , 0x14 },
  //{ "5"     , 0x15 },
  //{ "6"     , 0x16 },
  //{ "7"     , 0x17 },
  //{ "8"     , 0x18 },
  //{ "9"     , 0x19 },
  { "INFO", 0x1B},
  { "GUIDE", 0x1C},
  
};
 
int cursor = 0; // カーソル位置
 
void setup() {
  M5.begin();     // M5StickC初期化
  irsend.begin(); // IR初期化
 
  // リモコン項目表示
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 8);
  for ( int i = 0 ; i < ( sizeof(remote) / sizeof(REMOTE) ) ; i++ ) {
    M5.Lcd.print((cursor == i) ? ">" : " ");
    M5.Lcd.println(remote[i].name);
  }
}
 
void loop() {
  M5.update();  // ボタン状態更新
 
  // M5ボタンで送信
  if ( M5.BtnA.wasPressed() ) {
    // 送信4Byte(カスタマーコード2Byte+リモコンコード+反転リモコンコード)
    uint64_t send = 0;
    send = (uint64_t)reverseBits(CUSTOMER_CODE >> 8, 8) << 24;    // カスタマーコード(上位8bit)
    send += (uint64_t)reverseBits(CUSTOMER_CODE & 0xff, 8) << 16; // カスタマーコード(下位8bit)
    send += reverseBits(remote[cursor].command, 8) << 8;          // リモコンコードを順番入れ替えて送信
    send += reverseBits(remote[cursor].command, 8) ^ 0xff;        // リモコンコードのビット反転（パリティ）
    irsend.sendNEC(send);                                         // 送信
 
    // デバッグ表示
    Serial.printf("Send IR : 0x%08LX", send);
    Serial.printf("(customer=0x%04X, ", CUSTOMER_CODE);
    Serial.printf("command=0x%02X)\n", remote[cursor].command);
  }
 
  // 右ボタンでカーソル移動
  if ( M5.BtnB.wasPressed() ) {
    cursor++;
    cursor = cursor % ( sizeof(remote) / sizeof(REMOTE) );
 
    // カーソル描画
    M5.Lcd.setCursor(0, 8);
    for ( int i = 0 ; i < ( sizeof(remote) / sizeof(REMOTE) ) ; i++ ) {
      M5.Lcd.println((cursor == i) ? ">" : " ");
    }
  }
 
  delay(100);
}
