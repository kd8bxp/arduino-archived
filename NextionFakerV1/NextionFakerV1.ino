/**
 * Program	: NextionFaker
 * Purpose	: Give the Nextion Editor the impression it is talking to a NX type screen while
 * 			  in reality it is talking to a TJC screen
 * Author	: Nico Verduin
 * Date		: 21-1-2016
 * Version	: 1.2   Fixed bug and allow new uploads without resetting the Arduino
 *
 * To enable this program you need:
 * a) arduino uno
 * b) Serial TTL adapter for the Nextion
 *
 * Connect Nextion RX direct to Adapter TX
 * Connect Nextion TX to D0 on Arduino UNO
 * Connect D1 Arduino UNO to Adapter RX.
 * You can connect the GND to the Arduino and the Adapter (although usually not required)
 * GND to the Arduino.
 * Connect the Nextion screen to the 5V of the Arduino
 *
 * Parameters:
 * #define LED 					13		// lpin used for flashing LED
 * #define FLASH_TIME 			500		// flash once a second if uploading
 * #define TIME_OUT 			4000	// if no data is coming in after 4 seconds we're done uploading
 * #define BOOT_UPLOAD_SPEED 	9600	// We start in upload at 9600 baud
 * #define UPLOAD_SPEED			115200	// actual upload at 115200
 *
 * BOOT_UPLOAD_SPEED is the only parameter you might need to change. If you have changed the speed of
 * your screen to 115200 (or any other speed), you will need to change this as well to the same speed.
 *
 * If you need to upload a new sketch to the Arduino, do not forget to disconnect the wire to D0. Otherwise
 * the upload will fail.
 *
 * The program waits at 9600 baud for data coming in. Once data is received it is stored in a
 * character Array. Once we have received 6 x 0xFF (3 at the beginning and 3 at the end), the array
 * is searched for 3 characters "TJC". If found, these are replaced with "NX" and since this is one
 * byte less, the rest of the string is move left.
 * Then the message is sent to the Editor who will recognize it. and start sending firmware at 115200
 * baud. The Arduino stops at 9600 baud and switches to 115200 baud. During reception LED pin 13
 * will start flashing once a second.
 *
 * Credits go to
 * - Gerhard Kropf for support, input and test programs
 *
 * This program is free for use at your own risk.
 */

#include "Arduino.h"
//
// globals
//
char 		receiveBuffer[100];		// buffer for receiving messages. Should be large enough
uint8_t 	hexFFCtr = 0;			// counts the number oof FF's
uint8_t 	bufferIndex = 0;		// pointer in transmission buffer
char 		* TJCptr;				// location of TJC string if it exists
boolean 	mode = false;			// start of first step
byte 		c;						// just a temp field for each received byte
uint32_t 	timer = 0;				// we use this to flash the led
uint32_t	timerEOF;				// determines when no more data is received

#define LED 				13		// let's make the led flash while uploading
#define FLASH_TIME 			500		// flash once a second if uploading
#define TIME_OUT 			4000	// if no data is coming in after 4 seconds we're done uploading
#define BOOT_UPLOAD_SPEED 	9600	// We start in upload at 9600 baud
#define UPLOAD_SPEED		115200	// actual upload at 115200
/**
 * @name setup()
 * Initializes the program
 */
void setup()
{
	Serial.begin(BOOT_UPLOAD_SPEED);					// start with 9600 baud
	pinMode(LED, OUTPUT);								// let's flash a lead while uploading
}

/**
 * @name loop()
 * Loops endlessly through the code. The loop acknowledges 2 modes. if false, we are
 * receiving a connect string at 9600 baud. If a TJC is connected it will be detected
 * while the string "TJC" is replaced with "NX". It is the sent to the Editor.
 * IF it is an original "NX", no changes are made and it is sent back asis
 */
void loop()
{
	if (!mode) {
		if (Serial.available()) {						// wait for a character from the NExtion
			c = Serial.read();							// read the Nextion serial
			if (c != 0x00) {							// only non-zero characters
				receiveBuffer[bufferIndex] = c;			// add character to buffer
				bufferIndex++;							// and point to next position
			}
			if (c == 0xFF) {							// count the number of 0xFF characters
				hexFFCtr++;								// we should receive 6 of them
			}
			receiveBuffer[bufferIndex] = '\0';			// terminate our received string
			//
			// check if we have received the full message
			//
			if (hexFFCtr == 6) {						// we received a complete message
				TJCptr = strstr(receiveBuffer, "TJC");	// search it for "TJC" (Chinese Nextion)
				if (TJCptr != NULL) {					// if it exists
					strcpy(TJCptr, "NX");				// replace "TJC" with "NX"
					strcpy(TJCptr+2, TJCptr+3);			// and move the rest of the string one byte left
					bufferIndex--;						// decrease the length of the string
				}
				//
				// send our message to the Editor
				//
				for (uint8_t i = 0; i < bufferIndex; i++) {
					Serial.print(receiveBuffer[i]);
				}
				//
				// The editor should recognize our string so switch to the actual
				// upload process. Give it half a second
				//
				mode = true;							// switch to upload process
				delay(500);								// wait 500mS to finish sending message
				Serial.flush();							// flush our Serial buffers
				Serial.end();							// stop at 9600 baud
				Serial.begin(UPLOAD_SPEED);				// and switch to 115200 baud
				timerEOF = millis();					// start the end of file timer
			}
		}
	} else {											// upload mode
		//
		// check if we are finished uploading
		// this is when we have received no more data since 5 seconds
		//
		if ((millis() - timerEOF) > TIME_OUT) {
			//
			// no more data coming in so switch to startup mode
			//
			mode = false;								// switch to upload process
			delay(500);									// wait 500mS to finish sending message
			Serial.flush();								// flush our Serial buffers
			Serial.end();								// stop at 9600 baud
			Serial.begin(BOOT_UPLOAD_SPEED);			// and switch to 115200 baud

		}
		if (Serial.available() != 0) {					// as long as there are characters from the editor
			//
			// as long as there is data coming in no EOF is reached
			//
			timerEOF = millis();						// keep shifting our end of file timer
			if (timer == 0) {							// if the Flash timer is not set, start it
				timer 	= millis();
			}
			c = Serial.read();							// read byte and
			Serial.write(c);							// send it to the Nextion
			//
			// check for flashing the LED
			//
			if (timer != 0) {
				if ((millis() - timer)  > FLASH_TIME){	// flash the lLED
					digitalWrite(LED, !digitalRead(LED));
					timer = 0;							// if there is still input coming the timer will continue
				}
			}
		}
	}
}
