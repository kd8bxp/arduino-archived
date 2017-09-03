#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 2, NEO_GRB + NEO_KHZ800);
int brightness = 150;

//define some colors
#define Green (0,255,0)
#define Blue  (0,0,255)
#define Orange (255,153,0)
#define Purple (102, 51, 204)

//Setup MP3 player
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
#define CMD_NEXT_SONG 0X01

#define mp3blue 0X0F08
#define mp3find 0X0F09
#define mp3found 0X0F0B
#define mp3green 0X0F0D
#define mp3orange 0X0F0E
#define mp3purple 0X0F10
#define mp3tryagain 0X0F11
#define mp3yes 0X0F12

void setup() {

Serial3.begin(9600);
delay(500);
strip.begin();
strip.show();
strip.setBrightness(brightness);
sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
delay(200);//wait for 200ms
 //sendCommand(CMD_PLAY_W_VOL, 0X0F0E);//play the first song with volume 15 class

}

void loop() {
  // put your main code here, to run repeatedly:
//colorWipe(strip.Color Green, 50);
sendCommand(CMD_PLAY_W_VOL, mp3find);
delay(800);
sendCommand(CMD_PLAY_W_VOL, mp3blue);
colorWipe(strip.Color Blue, 50);
delay(1000);

sendCommand(CMD_PLAY_W_VOL, mp3find);
delay(800);
sendCommand(CMD_PLAY_W_VOL, mp3green);
colorWipe(strip.Color Green, 50);
delay(1000);

sendCommand(CMD_PLAY_W_VOL, mp3find);
delay(800);
sendCommand(CMD_PLAY_W_VOL, mp3orange);
colorWipe(strip.Color Orange, 50);
delay(1000);

sendCommand(CMD_PLAY_W_VOL, mp3find);
delay(800);
sendCommand(CMD_PLAY_W_VOL, mp3purple);
colorWipe(strip.Color Purple, 50);
delay(1000);

}

void colorWipe(uint32_t c, uint8_t wait) {
  for (int t=0; t<50; t++) {
for(uint16_t i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, c);
strip.show();
strip.setPixelColor(i, 0,0,0);
delay(10); //delay(wait);
}
strip.setPixelColor(strip.numPixels(), 0,0,0);
strip.show();
  }
  
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
    Serial3.write(Send_buf[i]) ;
  }
}


