#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include <WiFi.h>

const char *ssid = "ESP32-S3-GEEK";
const char *password = "Waveshare";

const IPAddress serverIP(192,168,6,123); //Address to be accessed
uint16_t serverPort = 8080;              //Server port number
WiFiClient client;                       // Declares a client object that is used to connect to the server

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
    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
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
        point += 8;
        if(point >= 230)
        {
          line += 15;
          point = 0; 
        }
        printf(".\r\n");
        delay(500);
    }
/******************WIFI is connected******************/  
    LCD_Clear(0x000f);
    IPAddress myIP = WiFi.localIP();
    uint32_t ipAddress = WiFi.localIP();
    char ipAddressStr[16]; 
    intToIpAddress(ipAddress, ipAddressStr);
    Paint_DrawString_EN(0, 20, "Connected", &Font24, 0x000f, 0xfff0);
    Paint_DrawString_EN(0, 50, "MyIP:", &Font24, 0x000f, 0xfff0);
    Paint_DrawString_EN(80, 54, ipAddressStr, &Font16, 0x000f, 0xfff0);
    printf("Connected\r\n");
    printf("IP Address:");
    printf("%s\r\n", ipAddressStr);
}

void loop()
{
/***************Wait to connect server***************/  
    Paint_DrawString_EN(0, 83, "Trying to access", &Font20, 0x000f, 0xfff0);
    printf("Trying to access the server\r\n");
/************Server connection successful************/      
    if (client.connect(serverIP, serverPort)) //Attempting to access the target address
    {
        LCD_Clear(0x000f);
        Paint_DrawString_EN(0, 25, "ESP32-S3-GEEK", &Font24, 0x000f, 0xfff0);
        Paint_DrawString_EN(0, 65, "Access successful", &Font20, 0x000f, 0xfff0);
        printf("Access successful\r\n");
        client.print("Hello world!");                    //Send data to the server
/**************Receiving server message**************/  
        while (client.connected() || client.available()) //If it is connected or has received unread data
        {
            if (client.available()) //If there is data to read
            {
                String line = client.readStringUntil('\r'); /// Reads data to a newline
                printf("Read the data:\r\n");
                printf("%s\r\n",line);
                client.write(line.c_str()); //Send back the received data
                LCD_Clear(0x000f);
                Paint_DrawString_EN(0, 10, "Received:", &Font24, 0x000f, 0xfff0);
                Paint_DrawString_EN(0, 33, line.c_str(), &Font20, 0x000f, 0xff00);
            }
        }

        printf("Close current connection\r\n");
        client.stop(); //Close the client
    }
    else
    {
        printf("Access failure\r\n");
        client.stop(); //Close the client
    }
    delay(5000);
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
