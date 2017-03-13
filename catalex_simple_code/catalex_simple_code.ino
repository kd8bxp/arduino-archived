/***********************************************************/
//Demo for the Serial MP3 Player by Catalex
//Hardware: Serial MP3 Player *1
//Board:  Arduino UNO R3
//IDE:	  Arduino-1.0
//Function:  To play the first song in the micro sd card.
//Store: http://www.aliexpress.com/store/1199788
//          http://www.dx.com/
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
#define CMD_NEXT_SONG 0X01

void setup() 
{
	mySerial.begin(9600);
	delay(500);//Wait chip initialization is complete
    sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
	delay(200);//wait for 200ms
	sendCommand(CMD_PLAY_W_VOL, 0X0F01);//play the first song with volume 15 class
}
void loop() 
{
//sendCommand(CMD_PLAY_W_INDEX, 0X0001);
delay(1000);
sendCommand(CMD_NEXT_SONG, 0X0000);
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
