/*
  Modified M5Rover code to work with the Generic Hub Wheel Robot
  LeRoy Miller, KD8BXP April 2021
*/

#include <WiFi.h>
#include <WiFiUdp.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_task_wdt.h"
#include <ArduinoOTA.h>

const char *ssid = "M5AP";
const char *password = "77777777";
const char *hostname = "robot1";  //used for OTA, bonus can use http://hostname.local if connected to AP or router

// Motor Left
#define motorLSpeed 32
#define motorLDir 19
#define motorLBrake 16

// Motor Right
#define motorRSpeed 33
#define motorRDir 18
#define motorRBrake 17

#define DIR_L_REVERSE 1
#define DIR_R_REVERSE 0
#define DIR_L_FORWARD 0
#define DIR_R_FORWARD 1
#define LEFTSPD pwmChannel_1
#define RIGHTSPD pwmChannel_2

#define LOWFREQ 110
#define HIFREQ 460
#define CHANNEL 1

const int freq = 3000;
const int LEFTSPD = 0;
const int RIGHTSPD = 1;
const int resolution = 8;

//TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);
WiFiServer server(80);

WiFiUDP Udp1;

//AtomMotion Atom;
bool direction = true;
uint8_t SendBuff[9] = {0xAA, 0x55,
                       0x00,
                       0x00,
                       0x00,
                       0x00,
                       0x00,
                       0x00,
                       0xee};



int16_t speed_buff[4] = {0};
int8_t speed_sendbuff[4] = {0};
uint32_t count = 0;
uint8_t IIC_ReState = I2C_ERROR_NO_BEGIN;


xSemaphoreHandle CtlSemaphore;


//DC motor speed range -127~127

void TaskMotion(void *pvParameters){
  while (1)
  {
  if (speed_buff[2] > 0) {digitalWrite(motorLDir, DIR_L_FORWARD); } else if (speed_buff[2] < 0) { digitalWrite(motorLDir, DIR_L_REVERSE); }
  if (speed_buff[3] > 0) {digitalWrite(motorLDir, DIR_R_FORWARD); } else if (speed_buff[3] < 0) { digitalWrite(motorLDir, DIR_R_REVERSE); }

    ledcWrite(LEFTSPD, fabs(speed_buff[2]));
    ledcWrite(RIGHTSPD, fabs(speed_buff[3]));
    vTaskDelay( 100 / portTICK_RATE_MS );
    //M5.dis.drawpix(0, 0xff0000);
  
  }
}

void setup()
{
Serial.begin(115200);
 WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  pinMode(motorLSpeed, OUTPUT);
  pinMode(motorLDir, OUTPUT);
  pinMode(motorRSpeed, OUTPUT);
  pinMode(motorRDir, OUTPUT);
  pinMode(motorLBrake, OUTPUT);
  pinMode(motorRBrake, OUTPUT);

  ledcSetup(LEFTSPD, freq, resolution);
  ledcSetup(RIGHTSPD, freq, resolution);

  ledcAttachPin(motorLSpeed, LEFTSPD);
  ledcAttachPin(motorRSpeed, RIGHTSPD);

  digitalWrite(motorLDir, DIR_L_FORWARD);
  digitalWrite(motorRDir, DIR_R_FORWARD);
  digitalWrite(motorRBrake, HIGH);
  digitalWrite(motorLBrake, HIGH);
  //M5.begin(true, false, true);
  //Atom.Init();     //sda  25     scl  21 
  vSemaphoreCreateBinary( CtlSemaphore );
  xTaskCreatePinnedToCore(
    TaskMotion
    ,  "TaskMotion"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);

    uint64_t chipid = ESP.getEfuseMac();
    String str = ssid + String((uint32_t)(chipid >> 32), HEX);

    //Serial.begin(115200);
    //Set device in STA mode to begin with
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1),
                      IPAddress(192, 168, 4, 1),
                      IPAddress(255, 255, 255, 0));

    WiFi.softAP(str.c_str(), password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.begin();

    Udp1.begin(1003);
OTA();
    
}


uint8_t Setspeed(int16_t Vtx, int16_t Vty, int16_t Wt)
{
    Wt = (Wt > 100) ? 100 : Wt;
    Wt = (Wt < -100) ? -100 : Wt;

    Vtx = (Vtx > 100) ? 100 : Vtx;
    Vtx = (Vtx < -100) ? -100 : Vtx;
    Vty = (Vty > 100) ? 100 : Vty;
    Vty = (Vty < -100) ? -100 : Vty;

    Vtx = (Wt != 0) ? Vtx * (100 - abs(Wt)) / 100 : Vtx;
    Vty = (Wt != 0) ? Vty * (100 - abs(Wt)) / 100 : Vty;

    speed_buff[0] = Vty - Vtx - Wt;
    speed_buff[1] = Vty + Vtx + Wt;
    speed_buff[3] = Vty - Vtx + Wt;
    speed_buff[2] = Vty + Vtx - Wt;

}  

void loop()
{
    int udplength = Udp1.parsePacket();
    if (udplength)
    {
        char udodata[udplength];
        Udp1.read(udodata, udplength);
        IPAddress udp_client = Udp1.remoteIP();
        if ((udodata[0] == 0xAA) && (udodata[1] == 0x55) && (udodata[7] == 0xee))
        {
            for (int i = 0; i < 8; i++)
            {
                Serial.printf("%02X ", udodata[i]);
            }
            Serial.println();
            if (udodata[6] == 0x01)
            {
                IIC_ReState = Setspeed(udodata[3] - 100, udodata[4] - 100, udodata[5] - 100);
            }
            else
            {
                IIC_ReState = Setspeed(0, 0, 0);
            }
        }
        else
        {
            IIC_ReState = Setspeed(0, 0, 0);
        }
    }
    count++;
    if (count > 100)
    {
        count = 0;

    }
   ArduinoOTA.handle();
}

void OTA() {

  ArduinoOTA.setHostname(hostname);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
     // Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      //Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      //Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) delay(1);//Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) delay(1);//Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) delay(1);//Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) delay(1);//Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) delay(1);//Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}
