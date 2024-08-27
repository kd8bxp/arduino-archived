#include <M5Stack.h>
#include "M5StackUpdater.h" 

#ifdef ARDUINO_ODROID_ESP32
#define DAC_PIN 26
#else
#define DAC_PIN 25
#endif

//static const char msgCal[] PROGMEM = {"Calibration procedure..."};

// encryption types
#define NOENC		0
#define WPA			2
#define WPA2		3
#define WPA_WPA2	4
#define WEP			5
#define NONE		7
#define AUTO		8

// Graph constant
#define WIDTH 320
#define HEIGHT 240
#define BANNER_HEIGHT 16
#define GRAPH_BASELINE (HEIGHT - 18)
#define GRAPH_HEIGHT (HEIGHT - 52)
#define CHANNEL_WIDTH (WIDTH / 16)

// RSSI RANGE
#define RSSI_CEILING -40
#define RSSI_FLOOR -100
#define NEAR_CHANNEL_RSSI_ALLOW -70

// define color
#define TFT_WHITE   ILI9341_WHITE   /* 255, 255, 255 */
#define TFT_BLACK   ILI9341_BLACK   /*   0,   0,   0 */
#define TFT_RED     ILI9341_RED     /* 255,   0,   0 */
#define TFT_ORANGE  ILI9341_ORANGE  /* 255, 165,   0 */
#define TFT_YELLOW  ILI9341_YELLOW  /* 255, 255,   0 */
#define TFT_GREEN   ILI9341_GREEN   /*   0, 255,   0 */
#define TFT_CYAN    ILI9341_CYAN    /*   0, 255, 255 */
#define TFT_BLUE    ILI9341_BLUE    /*   0,   0, 255 */
#define TFT_MAGENTA ILI9341_MAGENTA /* 255,   0, 255 */

// Channel color mapping from channel 1 to 14
uint16_t channel_color[] = {
  TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN, TFT_CYAN, TFT_MAGENTA,
  TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN, TFT_CYAN, TFT_MAGENTA,
  TFT_RED, TFT_ORANGE
};

#define RSSI_TRESHOLD	100  // threshold of RSSI alarm level
#define RSSI_OFFSET		95   // offset for displayed data
#define MAX_CHAN_QTY	205  // max number of channel for spacing 405.456543MHz
#define MAX_DISP_LINE	205  // limit horizontal display resolution if need for small display
#define MAX_SAMPLING	20	//20    // qty of samples in each iteration (1...100) to found a max RSSI value
#define TEXT_SIZE 2
#ifdef ARDUINO_ODROID_ESP32
#define ROTATION 0
#else
#define ROTATION 1
#endif
#define FCHAN_X 210	//95         // X position of frequency data
#define FCHAN_Y 4          // Y position of frequency data
#define AP_MAX_COUNT	100

byte cal[MAX_CHAN_QTY], data;
byte RSSI_data;

int RSSI_dbm, RSSI_max, RSSI_maxO[MAX_CHAN_QTY], RSSI_dbmS[MAX_CHAN_QTY];
int i, res_1, res_2, tmp_1, tmp_2, res_dif;
float f_chan;

uint8_t scan_count = 0, intflag = 0;

int		o_n;
int32_t o_channels[AP_MAX_COUNT];
int32_t o_rssis[AP_MAX_COUNT];
int32_t o_encrypt[AP_MAX_COUNT];
int		o_height[AP_MAX_COUNT];
String	o_SSID[AP_MAX_COUNT];
bool showfree = false;
bool printnets = false;
int btnApressed = 0;

void restart() {intflag = 1;}

void beep(int duration) {
	int i;
	for (i = 0; i<duration; i++) {
		dacWrite(DAC_PIN, 255);
		delay(1);
		dacWrite(DAC_PIN, 0);
		delay(1);
	}
}

// the setup routine runs once when M5Stack starts up
void setup() {
//    attachInterrupt(digitalPinToInterrupt(BUTTON_A_PIN), ISR, FALLING);	
	
    // initialize the M5Stack object
    M5.begin();
	Wire.begin();
	if (digitalRead(BUTTON_A_PIN) == 0) {
		Serial.println("Setup Will Load menu binary");
		updateFromFS(SD, "/menu.bin");
		ESP.restart();
	}
	beep(20);

  M5.Lcd.setRotation(ROTATION);
  // init banner
  M5.Lcd.setTextSize(2);
//  M5.Lcd.fillScreen(TFT_BLUE);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_RED);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print(" ESP ");
  M5.Lcd.setTextColor(TFT_WHITE, TFT_ORANGE);
  M5.Lcd.print(" 32 ");
  M5.Lcd.setTextColor(TFT_WHITE, TFT_GREEN);
  M5.Lcd.print(" WiFi ");
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
  M5.Lcd.print("Analyzer");

  M5.Lcd.fillRect(0, BANNER_HEIGHT, 320, 224, TFT_BLACK);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // rest for WiFi routine?
  
  intflag = 0;
}

// the loop routine runs over and over again forever
void loop(){
	if(M5.BtnA.wasPressed()) {
		beep(20);
		if(showfree==true){showfree=false; Serial.println("DoNotShowFree");}
		else {showfree=true; Serial.println("ShowFree");}
	}	
	if(M5.BtnB.wasPressed()) {
		beep(20);
		if(printnets==true){printnets=false; Serial.println("DoNotShowFree");}
		else {printnets=true; Serial.println("ShowFree");}
		M5.Lcd.fillRect(0, BANNER_HEIGHT, TFT_WIDTH, TFT_HEIGHT-BANNER_HEIGHT, BLACK);
	}
	Scannetworks();
	ReadBatt(280,2);
    M5.update();
	if (intflag) {
		Serial.println("Loop Will Load menu binary");
		updateFromFS(SD, "/menu.bin");
		ESP.restart();
	}
}

void Scannetworks(void) {
  uint8_t ap_count[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int32_t max_rssi[] = {-100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100};

  // WiFi.scanNetworks will return the number of networks found
//  int n = WiFi.scanNetworks();
  int n = WiFi.scanNetworks(false, true);  
	Serial.println("");  
	Serial.print(n);
	Serial.println(" Nets found\n\r");

  // clear old graph
 clear_old();
  //M5.Lcd.fillRect(0, BANNER_HEIGHT, 320, 224, TFT_BLACK);
  M5.Lcd.setTextSize(1);

  if (n == 0) {
    M5.Lcd.setTextColor(TFT_BLACK);
    M5.Lcd.setCursor(0, BANNER_HEIGHT);
    M5.Lcd.println("no networks found");
  } else {
    // plot found WiFi info

    for (int i = 0; i < n; i++) {
      int32_t channel = WiFi.channel(i);
	  o_channels[i]=channel;
      int32_t rssi = WiFi.RSSI(i);
	  o_rssis[i]=rssi;
	  int32_t encryption = WiFi.encryptionType(i);
	  o_encrypt[i] = encryption; 

	  Serial.print(WiFi.SSID(i));
	  Serial.print("\t");
	  Serial.println(encryption);
	  if(printnets)	{
		  M5.Lcd.setTextColor(WHITE);
		  M5.Lcd.setCursor(10, 20+BANNER_HEIGHT+10*i);
		  M5.Lcd.print(WiFi.SSID(i));
	  }	
	  else {
      uint16_t color = channel_color[channel - 1];
      int height = constrain(map(rssi, RSSI_FLOOR, RSSI_CEILING, 1, GRAPH_HEIGHT), 1, GRAPH_HEIGHT);
	  o_height[i] = height;
      // channel stat
      ap_count[channel - 1]++;
      if (rssi > max_rssi[channel - 1]) {
        max_rssi[channel - 1] = rssi;
      }

	  if(!showfree)	goto free;	
      if (WiFi.encryptionType(i) == NOENC || WiFi.encryptionType(i) == NONE){	
free:
      M5.Lcd.drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, color);
      M5.Lcd.drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel + 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, color);

      // Print SSID, signal strengh and if not encrypted
      M5.Lcd.setTextColor(color);
      M5.Lcd.setCursor((channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE - 10 - height);
      M5.Lcd.print(WiFi.SSID(i));
	  o_SSID[i]=WiFi.SSID(i);
      M5.Lcd.print('(');
      M5.Lcd.print(rssi);
      M5.Lcd.print(')');

      if (WiFi.encryptionType(i) == NOENC || WiFi.encryptionType(i) == NONE) {		  
        M5.Lcd.print('*'); }
		} //if(encrypt)
	}
    }	//  for (int i = 0; i < n; i++)
  }		//   } else {

  // save old values
  o_n=n;

  // print WiFi stat
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor(0, BANNER_HEIGHT);
  M5.Lcd.print(n);
 // digitalWrite(test, HIGH);
  M5.Lcd.print(" networks, suggested channels: ");
//  digitalWrite(test, LOW);
  bool listed_first_channel = false;
//  for (int i = 1; i <= 11; i++) { // channels 12-14 may not available
  for (int i = 1; i <= 14; i++) { // channels 12-14 may not available
    if ((i == 1) || (max_rssi[i - 2] < NEAR_CHANNEL_RSSI_ALLOW)) { // check previous channel signal strengh
      if ((i == sizeof(channel_color)) || (max_rssi[i] < NEAR_CHANNEL_RSSI_ALLOW)) { // check next channel signal strengh
        if (ap_count[i - 1] == 0) { // check no AP exists in same channel
          if (!listed_first_channel) {
            listed_first_channel = true;
          } else {
            M5.Lcd.print(", ");
          }
          M5.Lcd.print(i);
        }
      }
    }
  }
//  	  digitalWrite(test, HIGH);
  // draw graph base axle
  M5.Lcd.drawFastHLine(0, GRAPH_BASELINE, 320, TFT_WHITE);
  for (int i = 1; i <= 14; i++) {
    M5.Lcd.setTextColor(channel_color[i - 1]);
    M5.Lcd.setCursor((i * CHANNEL_WIDTH) - ((i < 10)?3:6), GRAPH_BASELINE + 2);
    M5.Lcd.print(i);
    if (ap_count[i - 1] > 0) {
      M5.Lcd.setCursor((i * CHANNEL_WIDTH) - ((ap_count[i - 1] < 10)?9:12), GRAPH_BASELINE + 11);
      M5.Lcd.print('(');
      M5.Lcd.print(ap_count[i - 1]);
      M5.Lcd.print(')');
    }	//     if (ap_count[i - 1] > 0)
  }		//   for (int i = 1; i <= 14; i++)
}

void clear_old(void){
    M5.Lcd.setTextColor(TFT_BLACK);
  if (o_n == 0) {
    M5.Lcd.setCursor(0, BANNER_HEIGHT);
    M5.Lcd.println("no networks found");
  } else {
    // plot found WiFi info
    for (int i = 0; i < o_n; i++) {
      int32_t channel = o_channels[i];
      int height = o_height[i];

	  if(printnets){
	  M5.Lcd.fillRect(0, 20+BANNER_HEIGHT+10*i, (o_SSID[i].length()+4)*8,8, TFT_BLACK);    
	  }
	  else{
      M5.Lcd.drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, TFT_BLACK);
      M5.Lcd.drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel + 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, TFT_BLACK);

      // Print SSID, signal strengh and if not encrypted
	  M5.Lcd.fillRect((channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE - 10 - height, (o_SSID[i].length()+4)*7,8, TFT_BLACK);    
	  }
	  }
  }
//  digitalWrite(test, HIGH);
  M5.Lcd.fillRect(0, BANNER_HEIGHT, 319, 8, TFT_BLACK);
  M5.Lcd.fillRect(0, BANNER_HEIGHT+8, 120, 8, TFT_BLACK);
  M5.Lcd.fillRect(0, 232, 300, 8, TFT_BLACK);
//  digitalWrite(test, LOW);
}


void ReadBatt(uint16_t x, uint16_t y) {
	float Bv;	
	uint8_t W=30, H = 10, Bl;
	uint16_t color;			// , x = 260, y = 16;
	analogSetCycles(64);
#ifdef ARDUINO_ODROID_ESP32
	Bv = (float)(analogRead(36) / 565.1);
#else
	Bv = (float)(analogRead(34) / 239.6);
#endif

	M5.Lcd.fillRect(x+2, y+2, 2*W, H, BLACK);
	M5.Lcd.drawRect(x, y, W+3, H+4, WHITE);
	M5.Lcd.fillRect(x+2+W, y+2+H/4, 4, 6, WHITE);

	Bl = (uint8_t)((Bv-3.2)*100);
	if(Bl>100)Bl=100;
	if(Bl>60) color = GREEN;
	if(Bl<50) color = YELLOW;
	if(Bl<30) color = RED;

	M5.Lcd.fillRect(x+2, y+2, Bl*W/100, H, color);
	M5.Lcd.setCursor(x+7, y+4);

	M5.Lcd.setTextColor(BLUE);
	Serial.println(Bv);
	M5.Lcd.printf("%d%%%", Bl);
	if (Bl <= 0) M5.powerOFF();
}

	  
