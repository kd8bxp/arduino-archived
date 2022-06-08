#include "WiFi.h"

char* factoryssid = "MySpectrumWiFi68-2G";
char* factorypassword =  "loudowl369";

void wifiBegin() {
  m5.begin();
  ez.begin();
  ez.wifi.readFlash();
  if (ez.wifi.networks.size() > 0) {
    String str = ez.wifi.networks[0].SSID; 
    int str_len = str.length() + 1; 
    char char_array[str_len];
    str.toCharArray(char_array, str_len);
    m5.Lcd.println(char_array);

    str = ez.wifi.networks[0].key; 
    str_len = str.length() + 1; 
    char char_array2[str_len];
    str.toCharArray(char_array2, str_len);
    m5.Lcd.println(char_array2);
    m5.Lcd.println("Detect OLD WiFi..");

    WiFi.begin( char_array,char_array2 );
  }
  else{
    WiFi.begin( factoryssid,factorypassword );
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    m5.Lcd.println("Connecting to WiFi..");
  }
  m5.Lcd.println("Connected to the WiFi network");
}
