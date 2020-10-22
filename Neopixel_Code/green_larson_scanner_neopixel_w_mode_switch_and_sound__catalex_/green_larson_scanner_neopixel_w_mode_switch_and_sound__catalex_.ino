#include <Adafruit_NeoPixel.h>
#include <TimedAction.h>

#define N_LEDS 12
#define PIN     9

/*
 * COLOR1 is least bright
 * COLOR2 is medium bright
 * COLOR3 is brightest
 */

//GREEN 
#define COLOR1 0x0F1F00 
#define COLOR2 0x0F3F00
#define COLOR3 0x0FFF00

/*
//PURPLE
#define COLOR1 0x1F001F
#define COLOR2 0x3F003F
#define COLOR3 0xFF00FF
*/

/*
 * mode 0 - knight rider 80s style slow scan
 * mode 1 - knight rider 00s style fast scan
 * mode 2 - strobe color
 * mode 3 - knight rider 80s style medium scan
 * mode 4 - knight rider 80s style fast scan
 * mode 5 - knight rider 00s style medium scan
 * mode 6 - knight rider 00s style slow scan
 */

int mode = 0;
int pos = 0, dir = 1; // Position, direction of "eye"

const int buttonPin = 2;
int buttonState;
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

void sound();
TimedAction timedAction = TimedAction(1200,sound);
#include <SoftwareSerial.h>

#define ARDUINO_RX 5//should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 6//connect to RX of the module
SoftwareSerial mySerial(ARDUINO_RX, ARDUINO_TX);

static int8_t Send_buf[8] = {0} ;

#define CMD_PLAY_W_INDEX 0X03
#define CMD_SET_VOLUME 0X06
#define CMD_SEL_DEV 0X09
  #define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_SINGLE_CYCLE 0X19
  #define SINGLE_CYCLE_ON 0X00
  #define SINGLE_CYCLE_OFF 0X01
#define CMD_PLAY_W_VOL 0X22



Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(0, modeChange, CHANGE);
  strip.begin();
   mySerial.begin(9600);
  delay(500);//Wait chip initialization is complete
    sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
}

void modeChange() {
  delay(500);
  mode++;
}

void loop() {
  /*
int reading = digitalRead(buttonPin);
if (reading != lastButtonState) {
  lastDebounceTime = millis();
}
if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        mode++;
      }
    }
}
*/
if (mode > 6) {mode = 0;}
if (mode == 0){
  //sendCommand(CMD_PLAY_W_VOL, 0X0E01);
  mode0();}
if (mode == 1)  {
  //sendCommand(CMD_PLAY_W_VOL, 0X0E02);
  mode1();}
if (mode == 2) {
  //sendCommand(CMD_PLAY_W_VOL, 0X0E03);
  mode2();}
if (mode == 3) {
  //sendCommand(CMD_PLAY_W_VOL, 0X0E04);
  mode3();}
if (mode == 4) {
  //sendCommand(CMD_PLAY_W_VOL, 0X0E05);
  mode4();}
if (mode == 5) {
  //sendCommand(CMD_PLAY_W_VOL, 0X0E06);
  mode5();}
if (mode == 6) {
  //sendCommand(CMD_PLAY_W_VOL, 0X0E07);
  mode6();}
}

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for(uint8_t i=0; i<8; i++)//
  {
    mySerial.write(Send_buf[i]) ;
  }
}

 void mode0() { 
  int j;
  timedAction.check();

  if (dir == 1 ) {
  strip.setPixelColor(pos - 2, COLOR1); 
  strip.setPixelColor(pos - 1, COLOR2); 
  strip.setPixelColor(pos    , COLOR3); 
    }
  
  strip.show();
  
  delay(100);

  // Rather than being sneaky and erasing just the tail pixel,
  // it's easier to erase it all and draw a new one next time.
  for(j=-2; j<= 2; j++) strip.setPixelColor(pos+j, 0);

  // Bounce off ends of strip
  pos += dir;
  if(pos < 0) {
    pos = 1;
    dir = -dir;
  } else if(pos >= strip.numPixels()) {
    pos = strip.numPixels() - 2;
    dir = -dir;
  }
  
  if (dir < 0) {
  strip.setPixelColor(pos    , COLOR3); 
  strip.setPixelColor(pos + 1, COLOR2); 
  strip.setPixelColor(pos + 2, COLOR1); 
  }
}

void mode1() {
  timedAction.check();
  int half = strip.numPixels()/2;
  for(int j=0; j<=half; j++) {
    strip.setPixelColor(j, COLOR3);
    strip.setPixelColor(j+1,COLOR2);
    strip.setPixelColor(strip.numPixels()-j, COLOR3);
    strip.setPixelColor(strip.numPixels()-(j+1),COLOR2);
    strip.show();
    delay(10);
    strip.setPixelColor(j, 0);
    strip.setPixelColor(strip.numPixels()-j, 0);
    strip.show();
    delay(10);
  }
    for (int j=half; j>=0; j--) {
      strip.setPixelColor(j, COLOR3);
      strip.setPixelColor(strip.numPixels()-j,COLOR3);
      strip.show();
      delay(10);
      strip.setPixelColor(j,0);
      strip.setPixelColor(strip.numPixels()-j,0);
      strip.show();
      delay(10);
    }
  
  }

void mode2() {
  
  for (int j=0;j<=strip.numPixels();j++){
  strip.setPixelColor(j,COLOR3);
  }
  strip.show();
  delay(100);
  for (int j=0;j<=strip.numPixels();j++){
  strip.setPixelColor(j,COLOR2);
  }
  strip.show();
  delay(100);
  for (int j=0;j<=strip.numPixels();j++){
  strip.setPixelColor(j, COLOR1);
  }
  strip.show();
  delay(100);
}

void mode3() { 
  int j;
timedAction.check();
  if (dir == 1 ) {
  strip.setPixelColor(pos - 2, COLOR1); 
  strip.setPixelColor(pos - 1, COLOR2); 
  strip.setPixelColor(pos    , COLOR3); 
    }
  
  strip.show();
  delay(75);

  // Rather than being sneaky and erasing just the tail pixel,
  // it's easier to erase it all and draw a new one next time.
  for(j=-2; j<= 2; j++) strip.setPixelColor(pos+j, 0);

  // Bounce off ends of strip
  pos += dir;
  if(pos < 0) {
    pos = 1;
    dir = -dir;
  } else if(pos >= strip.numPixels()) {
    pos = strip.numPixels() - 2;
    dir = -dir;
  }
  
  if (dir < 0) {
  strip.setPixelColor(pos    , COLOR3); 
  strip.setPixelColor(pos + 1, COLOR2); 
  strip.setPixelColor(pos + 2, COLOR1); 
  }
}

void mode4() { 
  int j;
timedAction.check();
  if (dir == 1 ) {
  strip.setPixelColor(pos - 2, COLOR1); 
  strip.setPixelColor(pos - 1, COLOR2); 
  strip.setPixelColor(pos    , COLOR3); 
    }
  
  strip.show();
  delay(50);

  // Rather than being sneaky and erasing just the tail pixel,
  // it's easier to erase it all and draw a new one next time.
  for(j=-2; j<= 2; j++) strip.setPixelColor(pos+j, 0);

  // Bounce off ends of strip
  pos += dir;
  if(pos < 0) {
    pos = 1;
    dir = -dir;
  } else if(pos >= strip.numPixels()) {
    pos = strip.numPixels() - 2;
    dir = -dir;
  }
  
  if (dir < 0) {
  strip.setPixelColor(pos    , COLOR3); 
  strip.setPixelColor(pos + 1, COLOR2); 
  strip.setPixelColor(pos + 2, COLOR1); 
  }
}

void mode5() {
  timedAction.check();
  int half = strip.numPixels()/2;
  for(int j=0; j<=half; j++) {
    strip.setPixelColor(j, COLOR3);
    strip.setPixelColor(j+1,COLOR2);
    strip.setPixelColor(strip.numPixels()-j, COLOR3);
    strip.setPixelColor(strip.numPixels()-(j+1),COLOR2);
    strip.show();
    delay(20);
    strip.setPixelColor(j, 0);
    strip.setPixelColor(strip.numPixels()-j, 0);
    strip.show();
    delay(20);
  }
    for (int j=half; j>=0; j--) {
      strip.setPixelColor(j, COLOR3);
      strip.setPixelColor(strip.numPixels()-j,COLOR3);
      strip.show();
      delay(20);
      strip.setPixelColor(j,0);
      strip.setPixelColor(strip.numPixels()-j,0);
      strip.show();
      delay(20);
    }
  
  }

  void mode6() {
    timedAction.check();
  int half = strip.numPixels()/2;
  for(int j=0; j<=half; j++) {
    strip.setPixelColor(j, COLOR3);
    strip.setPixelColor(j+1,COLOR2);
    strip.setPixelColor(strip.numPixels()-j, COLOR3);
    strip.setPixelColor(strip.numPixels()-(j+1),COLOR2);
    strip.show();
    delay(30);
    strip.setPixelColor(j, 0);
    strip.setPixelColor(strip.numPixels()-j, 0);
    strip.show();
    delay(30);
  }
    for (int j=half; j>=0; j--) {
      strip.setPixelColor(j, COLOR3);
      strip.setPixelColor(strip.numPixels()-j,COLOR3);
      strip.show();
      delay(30);
      strip.setPixelColor(j,0);
      strip.setPixelColor(strip.numPixels()-j,0);
      strip.show();
      delay(30);
    }
  
  }

  void sound() {
    sendCommand(CMD_PLAY_W_VOL, 0X0E01);
  }


