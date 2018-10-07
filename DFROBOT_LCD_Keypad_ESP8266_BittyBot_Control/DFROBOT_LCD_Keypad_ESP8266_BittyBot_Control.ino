#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal.h>
 
#define SSID "BittyBotxxxx" // insert your SSID
#define PASS "" // insert your password
#define DST_IP "192.168.4.1" //api.openweathermap.org
#include <SoftwareSerial.h>
SoftwareSerial esp(3,2);  // use pins 7, 8 for software Serial 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int goCmd;

// read the buttons
int read_LCD_buttons()
{
	adc_key_in = analogRead(0);      // read the value from the sensor
// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
// we add approx 50 to those values and check to see if we are close
	if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
// For V1.1 us this threshold
	
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 250)  return btnUP;
	if (adc_key_in < 450)  return btnDOWN;
	if (adc_key_in < 650)  return btnLEFT;
	if (adc_key_in < 850)  return btnSELECT;
	

// For V1.0 comment the other threshold and use the one below:
/*
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 150)  return btnUP;
	if (adc_key_in < 265)  return btnDOWN;
	if (adc_key_in < 460)  return btnLEFT;
	if (adc_key_in < 690)  return btnSELECT;
*/
	return btnNONE;  // when all others fail, return this...
}

void setup()
{

Serial.begin(9600);
esp.begin(9600);  

lcd.begin(16, 2);              // start the library
	
	pinMode(10, OUTPUT);
digitalWrite(10, HIGH);
lcd.setCursor(0, 0);
	lcd.print("BittyBot Wifi Control!"); // print a simple message
  Serial.println("Init");
  
  esp.println("AT+RST"); // restet and test if module is redy
  delay(1000);
  if(esp.find("ready")) {
    Serial.println("WiFi - Module is ready");
  }else{
    Serial.println("Module dosn't respond.");
    while(1);
  }
  delay(1000);
  // try to connect to wifi
  boolean connected=false;
  for(int i=0;i<5;i++){
    if(connectWiFi()){
      connected = true;
      Serial.println("Connected to WiFi...");
      break;
    }
  }
  if (!connected){
    Serial.println("Coudn't connect to WiFi.");
    while(1);
  }
  delay(5000);
  esp.println("AT+CIPMUX=0"); // set to single connection mode



}

void loop()
{
	
	lcd.setCursor(0, 1);           // move to the begining of the second line
	
	lcd_key = read_LCD_buttons();  // read the buttons

	switch (lcd_key)               // depending on which button was pushed, we perform an action
	{
		case btnRIGHT:
		{
			lcd.print("RIGHT ");
		goCmd = 11;
		sendtoweb();
			break;
		}
		case btnLEFT:
		{
			lcd.print("LEFT ");
		goCmd = 10;
		sendtoweb();
			break;
		}
		case btnUP:
		{
			lcd.print("Forward ");
		goCmd = 16;
		sendtoweb();
			break;
		}
		case btnDOWN:
		{
			lcd.print("Backward ");
		goCmd = 17;
		sendtoweb();
			break;
		}
		case btnSELECT:
		{
			lcd.print("STOP");
			goCmd = 18;
		sendtoweb();
			break;
		}
		case btnNONE:
		{
			lcd.print("                 ");
			digitalWrite(10, HIGH);
			break;
		}
	}
	// lcd.print(lcd_key);

}



void sendtoweb()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  esp.println(cmd);
  Serial.println(cmd);
  if(esp.find("Error")) return;
  cmd = "GET /?drive=";
  cmd += goCmd;
  cmd += " HTTP/1.0\r\n\r\n\r\n";
  esp.print("AT+CIPSEND=");
  esp.println(cmd.length());
  if(esp.find(">")){
    Serial.print(">");
  }else{
    esp.println("AT+CIPCLOSE");
    Serial.println("connection timeout");
    delay(1000);
    return;
  }
  esp.print(cmd);
  
  delay(2000);
}
 
boolean connectWiFi()
{
  esp.println("AT+CWMODE=1");
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp.println(cmd);
  delay(2000);
  if(esp.find("OK")){
    Serial.println("OK, Connected to WiFi.");
    return true;
  }else{
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}