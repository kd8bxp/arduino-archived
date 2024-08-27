// PacketMonitor32 of M5STACK : 2018.01.11 macsbug
// spacehuhn/PacketMonitor32 : 
//  https://github.com/spacehuhn/PacketMonitor32/
// DISPLAY: Channel,RSSI,Packet per Second,deauth packets,SD Card enabled
// Button : click to change channel hold to dis/enable SD
// SD : GPIO4=CS(CD/D3), 23=MOSI(CMD), 18=CLK, 19=MISO(D0)
//--------------------------------------------------------------------
#include <M5Stack.h>
#include "Free_Fonts.h"
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <string>
#include <cstddef>
#include <Preferences.h>
#include <M5StackUpdater.h>
#define MAX_CH 14     // 1-14ch(1-11 US,1-13 EU and 1-14 Japan)
#define SNAP_LEN 2324 // max len of each recieved packet
#define BUTTON_PIN 39 // button to change the channel
#define MAX_X 315     // 128
#define MAX_Y 230     //  51
#if CONFIG_FREERTOS_UNICORE
#define RUNNING_CORE 0
#else
#define RUNNING_CORE 1
#endif
#include "Buffer.h"
#include "FS.h"
#include "SD.h"
enum { sd_sck = 18, sd_miso = 19, sd_mosi = 23, sd_ss = 4 };
esp_err_t event_handler(void* ctx, system_event_t* event) { return ESP_OK; }
/* ===== run-time variables ===== */
Buffer sdBuffer;
Preferences preferences;
bool useSD = false;
bool buttonPressed = false;
bool buttonEnabled = true;
uint32_t lastDrawTime;
uint32_t lastButtonTime;
uint32_t tmpPacketCounter;
uint32_t pkts[MAX_X]; // here the packets per second will be saved
uint32_t deauths = 0; // deauth frames per second
unsigned int ch = 1;  // current 802.11 channel
int rssiSum;
double multiplicator = 1;
// ===== main program ================================================
void setup() {
	// Serial ----------------------------------------------------------
	Serial.begin(115200);
	// System & WiFi ---------------------------------------------------
	nvs_flash_init();
	tcpip_adapter_init();
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	//ESP_ERROR_CHECK(esp_wifi_set_country(WIFI_COUNTRY_EU));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
	ESP_ERROR_CHECK(esp_wifi_start());
	esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
	// display -------------------------------------------------------
	M5.begin();
	if (digitalRead(BUTTON_A_PIN) == 0) {
		Serial.println("Will Load menu binary");
		updateFromFS(SD, "/menu.bin");
		ESP.restart();
	}
	dacWrite(25, 0); // Speaker OFF
	M5.Lcd.fillScreen(TFT_BLACK);
	M5.Lcd.setTextColor(WHITE, BLACK);
	M5.Lcd.setTextSize(1);
	M5.Lcd.setRotation(0);
	/* show start screen */
	M5.Lcd.fillScreen(BLACK);
	drawScale();
	M5.Lcd.setFreeFont(FM9);
	M5.Lcd.setTextDatum(TL_DATUM);
	M5.Lcd.fillRect(0, 0, 320, 20, BLUE);
	// SD card ---------------------------------------------------------
	SPI.end();
	SPI.begin(sd_sck, sd_miso, sd_mosi, sd_ss);
	SD.begin(sd_ss, SPI, 24000000);
	if (!SD.begin(sd_ss, SPI)) {
		Serial.println("Card Mount Failed"); 
		//return;
	}
	sdBuffer = Buffer();
	if (setupSD()) {
		sdBuffer.open(&SD); Serial.println(" SD CHECK OPEN");
	}
	// I/O -----------------------------------------------------------
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	useSD = false;
	// second core ----------------------------------------------------
	xTaskCreatePinnedToCore(
		coreTask,               /* Function to implement the task */
		"coreTask",             /* Name of the task */
		2500,                   /* Stack size in words */
		NULL,                   /* Task input parameter */
		0,                      /* Priority of the task */
		NULL,                   /* Task handle. */
		RUNNING_CORE);          /* Core where the task should run */
								// start Wifi sniffer ---------------------------------------------
	esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous);
	esp_wifi_set_promiscuous(true);
}
// ===== main program ================================================
void loop() {
	vTaskDelay(portMAX_DELAY);
}
// ===== functions ===================================================
double getMultiplicator() {
	uint32_t maxVal = 1;
	for (int i = 0; i < MAX_X; i++) {
		if (pkts[i] > maxVal) maxVal = pkts[i];
	}
	if (maxVal > MAX_Y) return (double)MAX_Y / (double)maxVal;
	else return 1;
}
// ===== functions ===================================================
void drawScale() {
	int s = 10, a = 0;
	M5.Lcd.setTextColor(WHITE, BLACK);  // Pkts Scale
	for (int ypos = MAX_Y; ypos > 120; ypos = ypos - s) {
		M5.Lcd.setTextDatum(MR_DATUM);
		M5.Lcd.drawString(String((int)((MAX_Y - ypos)/multiplicator)), 30, ypos - 1 - a);
		a = a + 10;
	}
	M5.Lcd.setFreeFont(FM9);
	M5.Lcd.setTextDatum(TL_DATUM);
}

void setChannel(int newChannel) {
	ch = newChannel;
	if (ch > MAX_CH || ch < 1) ch = 1;
	preferences.begin("packetmonitor32", false);
	preferences.putUInt("channel", ch);
	preferences.end();
	esp_wifi_set_promiscuous(false);
	esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
	esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous);
	esp_wifi_set_promiscuous(true);
}
// ===== functions ===================================================
bool setupSD() {
	if (!SD.begin(sd_ss, SPI)) {
		Serial.println("Card Mount Failed"); return false;
	}
	uint8_t cardType = SD.cardType();
	if (cardType == CARD_NONE) {
		Serial.println("No SD_MMC card attached"); return false;
	}
	Serial.print("SD_MMC Card Type: ");
	if (cardType == CARD_MMC) {
		Serial.println("MMC");
	}
	else if (cardType == CARD_SD) {
		Serial.println("SDSC");
	}
	else if (cardType == CARD_SDHC) {
		Serial.println("SDHC");
	}
	else {
		Serial.println("UNKNOWN");
	}
	uint64_t cardSize = SD.cardSize() / (1024 * 1024);
	Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
	return true;
}
// ===== functions ===================================================
void wifi_promiscuous(void* buf, wifi_promiscuous_pkt_type_t type) {
	wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
	wifi_pkt_rx_ctrl_t ctrl = (wifi_pkt_rx_ctrl_t)pkt->rx_ctrl;
	if (type == WIFI_PKT_MGMT &&
		(pkt->payload[0] == 0xA0 || pkt->payload[0] == 0xC0)) deauths++;
	if (type == WIFI_PKT_MISC) return;   // wrong packet type
	if (ctrl.sig_len > SNAP_LEN) return; // packet too long
	uint32_t packetLength = ctrl.sig_len;
	if (type == WIFI_PKT_MGMT) packetLength -= 4;
	// fix for known bug in the IDF
	// https://github.com/espressif/esp-idf/issues/886
	//Serial.print(".");
	tmpPacketCounter++;
	rssiSum += ctrl.rssi;
	if (useSD) sdBuffer.addPacket(pkt->payload, packetLength);
}
// ===== functions ===================================================
void draw() {
	multiplicator = getMultiplicator();
	drawScale();
	int len, rssi;
	if (pkts[MAX_X - 1] > 0) rssi = rssiSum / (int)pkts[MAX_X - 1];
	else rssi = rssiSum;
	String p = (String)ch + "|" + (String)rssi + "|Pkts " +
		(String)tmpPacketCounter + " [" + deauths + "]" +
		(useSD ? "|SD" : "") + "|"+ (String)multiplicator;
	M5.Lcd.setTextColor(WHITE, BLUE);                    // packet
	M5.Lcd.drawString(p + "  ", 10, 2);                 // string DRAW
	for (int i = 40; i < MAX_X; i++) {                  // LINE DRAW
		len = pkts[i] * multiplicator;
		len = len * 2;
		if ((MAX_Y - len) < (MAX_Y - 200)) { len = 200; }  // over flow
		M5.Lcd.drawFastVLine(i, 31, MAX_Y+10-31, TFT_BLACK);      // LINE EARSE
		M5.Lcd.drawFastVLine(i, MAX_Y+10-len, len, GREEN);// LINE DRAW
		if (i < MAX_X - 1) pkts[i] = pkts[i + 1];
	}
}
// ===== functions ===================================================
void coreTask(void * p) {
	uint32_t currentTime;
	while (true) {
		currentTime = millis();
		/* bit of spaghetti code, have to clean this up later :D_ */
		// check button
		if (digitalRead(BUTTON_PIN) == LOW) {
			M5.Lcd.fillRect(0, 0, 320, 20, BLUE);
			if (buttonEnabled) {
				if (!buttonPressed) {
					buttonPressed = true;
					lastButtonTime = currentTime;
				}
				else if (currentTime - lastButtonTime >= 2000) {
					if (useSD) {
						useSD = false;
						sdBuffer.close(&SD);
						draw();
					}
					else {
						if (setupSD())
							sdBuffer.open(&SD);
						draw();
					}
					buttonPressed = false;
					buttonEnabled = false;
				}
			}
		}
		else {
			if (buttonPressed) {
				setChannel(ch + 1);
				draw();
			}
			buttonPressed = false;
			buttonEnabled = true;
		}
		// save buffer to SD
		if (useSD) sdBuffer.save(&SD);
		// draw Display
		if (currentTime - lastDrawTime > 1000) {
			lastDrawTime = currentTime;
			// Serial.printf("\nFree RAM %u %u\n", 
			// heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT),
			// heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT));
			// for debug purposes
			pkts[MAX_X - 1] = tmpPacketCounter;
			draw();
			//Serial.println((String)pkts[MAX_X - 1]);
			tmpPacketCounter = 0;
			deauths = 0;
			rssiSum = 0;
		}
		// Serial input
		if (Serial.available()) {
			ch = Serial.readString().toInt();
			if (ch < 1 || ch > 14) ch = 1;
			setChannel(ch);
		}
	}
}