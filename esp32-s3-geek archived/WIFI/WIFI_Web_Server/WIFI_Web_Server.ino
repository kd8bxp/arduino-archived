#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include "WIFI_Driver.h"

// Set these to your desired credentials.
const char *ssid = "ESP32-S3-GEEK";
const char *password = "Waveshare";

WiFiServer server(80);
WiFiClient client;

void setup()
{
  Config_Init();
  LCD_Init();
  Serial.begin(115200);
  LCD_SetBacklight(100);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_SetRotate(90);
  LCD_Clear(0x000f);

  WIFI_AP_Init();
}

void loop()
{
  WiFiClient client = server.available();   // listen for incoming clients
  WIFI_Web_Server(client);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
