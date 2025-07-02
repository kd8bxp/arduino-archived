#include "src/NTPClient/NTPClient.h"
// change next line to use with another board/shield
#include <WiFi.h>
#include "src/M5Atom/M5Atom.h"

const char *ssid     = "";
const char *password = "";
// UTC Beijing
const long utcOffsetInSeconds = 8 * 3600;

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "ntp.jst.mfeed.ad.jp", utcOffsetInSeconds);

CRGB colors[] = {
  0x000000,
  0xffffff,
  0xff0000,
  0xff8000,
  0xffff00,
  0x80ff00,
  0x00ff00,
  0x00ff80,
  0x00ffff,
  0x0080ff,
  0x0000ff,
  0x8000ff,
  0xff00ff,
  0xff0080
};

bool showmode = false; 

void setup(){
  Serial.begin(115200);
  M5.begin(true, false, true);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
}

void loop() {
  timeClient.update();
  String timeStamp = timeClient.getFormattedTime();  
  Serial.println(timeStamp);

  String hourStamp = timeStamp.substring(0,2);
  String minuteStamp = timeStamp.substring(3,5);
  String secondStamp = timeStamp.substring(6,8);

  int hour = hourStamp.toInt();
  int minute = minuteStamp.toInt();
  int second = secondStamp.toInt();
  
  if(hour > 12)
  {
    hour = hour - 12;
  }  

  //切换显示描述
  if (showmode){  
    Serial.println("showmode second");
    int j=0;
    for (int i = 0; i < 25; i++) {
      int ii = i % 5;    
      j=i/5;
      if (i<second/3){
        M5.dis.drawpix(ii,j, colors[4]);
      }else{
        M5.dis.drawpix(ii,j,colors[0]); 
      }
      //0-3秒时中间显示个标识,防止用户以为不显示了
      if (second>=0 and second<=3){ 
        M5.dis.drawpix(2,2, colors[4]);        
      }
    }
  }
  //按键后切换显示时间
  else{
    Serial.println("showmode time");
    M5.dis.drawpix(2, 2, colors[4]);

    //写小时
    int j=0;
    for (int i = 0; i < 12; i++) {
        int ii = i % 5;  
        j=i/5;   
           
        if (i<hour){
          M5.dis.drawpix(ii, j, colors[1]);
        }else{
          M5.dis.drawpix(ii, j, colors[0]); 
        } 
    }
  
    //写分钟
    j=0;
    for (int i = 0; i < 12; i++) {
        int ii = 4- i % 5;    
        j=4-i/5;     
         
        if (i<minute/5){
          M5.dis.drawpix(ii,j, colors[8]);
        }else{
          M5.dis.drawpix(ii,j,colors[0]); 
        } 
    }
  }
  
  if (M5.Btn.wasReleasefor(15))
  {
    showmode=!showmode;
  } 
  
  delay(1000);
  //加不加这句,屏幕都闪烁
  M5.update();
}
