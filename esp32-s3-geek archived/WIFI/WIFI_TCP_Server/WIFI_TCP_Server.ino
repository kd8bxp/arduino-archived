#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include <WiFi.h>

const char *ssid = "ESP32-S3-GEEK";
const char *password = "Waveshare";
WiFiServer server; /// Declares the server object

void intToIpAddress(uint32_t ip, char *result) {
    sprintf(result, "%d.%d.%d.%d", ip & 255,(ip >> 8) & 255,(ip >> 16) & 255,(ip >> 24) & 255);
}

void setup()
{
/************Each module is initialized***************/  
    Config_Init();
    LCD_Init();
    Serial.begin(115200);
    LCD_SetBacklight(100);
    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, 0x000f);
    Paint_SetRotate(90);
    LCD_Clear(0x000f);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); //Turn off wifi hibernation in STA mode to improve response speed
    WiFi.begin(ssid, password);
/**************Wait for WIFI connection***************/  
    char point;
    char line = 60;
    while (WiFi.status() != WL_CONNECTED)
    {
        Paint_DrawString_EN(0, 5, "ESP32-S3-GEEK", &Font24, 0x000f, 0xfff0);
        Paint_DrawString_EN(0, 40, "Trying to connect", &Font20, 0x000f, 0xfff0);
        Paint_DrawString_EN(point, line, ".", &Font20, 0x000f, 0xfff0);
        point += 12;
        if(point >= 228)
        {
          line += 15;
          point = 0; 
        }
        printf(".\r\n");
        delay(1000);
    }
    LCD_Clear(0x000f);    
}

void loop()
{
/******************WIFI is connected******************/  
    IPAddress myIP = WiFi.localIP();
    uint32_t ipAddress = WiFi.localIP();
    char ipAddressStr[16]; 
    intToIpAddress(ipAddress, ipAddressStr);
    printf("Connected\r\n");
    printf("IP Address:");
    printf("%s\r\n", ipAddressStr);
    server.begin(8080); 
/***********Wait for the client to connect***********/  
    WiFiClient client = server.available(); //Try to create a customer object
    Paint_DrawString_EN(0, 7, "Connected WIFI", &Font24, 0x000f, 0xfff0);
    Paint_DrawString_EN(0, 50, "IP:", &Font24, 0x000f, 0xfff0);
    Paint_DrawString_EN(55, 54, ipAddressStr, &Font16, 0x000f, 0xfff0);
    Paint_DrawString_EN(0, 90, "Trying to access", &Font20, 0x000f, 0xfff0);
    printf("Trying to access the server\r\n");
/************client connection successful************/       
     if (client) //If current customer available
    {
      printf("[Client connected]\r\n");
      while (client.connected() || client.available()) //If it is connected or has received unread data
      {
        if (client.available()) //If there is data to read
        {
            String line = client.readStringUntil('\n'); // Read data from the client up to the newline character
            const char* charArray = line.c_str();       // Gets a string ending in '\n'
            // Removes the trailing return newline character
            int length = line.length();
            while (length > 0 && (line[length - 1] == '\r' || line[length - 1] == '\n')) {
                length--;
            }
            // Creates a new array of characters, excluding the return newline character
            char* modifiedCharArray = new char[length + 1];
            memcpy(modifiedCharArray, charArray, length);
            modifiedCharArray[length] = '\0'; // Add a null character to ensure that the character array ends in null

            LCD_Clear(0x000f);
            Paint_DrawString_EN(0, 10, "Received:", &Font24, 0x000f, 0xfff0);
            Paint_DrawString_EN(0, 33, modifiedCharArray, &Font20, 0x000f, 0xff00);
        }
      }
      client.stop();                                //End the current connection
      printf("[Client disconnected]\r\n");
    }
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
