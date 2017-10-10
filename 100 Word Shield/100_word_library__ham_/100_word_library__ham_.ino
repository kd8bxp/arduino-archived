//Alphabet
#define a 0x00  //US spelling
#define b 0x01
#define c 0x02
#define d 0x03
#define e 0x04
#define f 0x05
#define g 0x06
#define h 0x07
#define _i 0x08
#define j 0x09
#define k 0x0a
#define l 0x0b
#define m 0x0c
#define n 0x0d
#define o 0x0e
#define _p 0x0f
#define q 0x10
#define r 0x11
#define s 0x12
#define t 0x13
#define u 0x14
#define v 0x15
#define w 0x16
#define x 0x17
#define y 0x18
#define z 0x19

//Phonetic Alphabet
#define alpha 0x1a
#define bravo 0x1b
#define charlie 0x1c
#define delta 0x1d
#define echo 0x1e
#define foxtrot 0x1f
#define golf 0x20
#define hotel 0x21
#define india 0x22
#define juliet 0x23
#define kilo 0x24
#define lima 0x25
#define mike 0x26
#define november 0x27
#define oscar 0x28
#define papa 0x29
#define quebec 0x2a
#define romeo 0x2b
#define sierra 0x2c
#define tango 0x2d
#define uniform 0x2e
#define victor 0x2f
#define whiskey 0x30
#define xray 0x31
#define yankee 0x32
#define zulu 0x33

//Numbers
#define zero 0x34
#define one 0x35
#define two 0x36
#define three 0x37
#define four 0x38
#define five 0x39
#define six 0x3a
#define seven 0x3b
#define eight 0x3c
#define nine 0x3d
#define ten 0x3e
#define eleven 0x3f
#define twelve 0x40
#define thirteen 0x41
#define fourteen 0x42
#define fifteen 0x43
#define sixteen 0x44
#define seventeen 0x45
#define eighteen 0x46
#define nineteen 0x47
#define twenty 0x48
#define thirty 0x49
#define forty 0x4a
#define fifty 0x4b
#define sixty 0x4c
#define seventy 0x4d
#define eighty 0x4e
#define ninety 0x4f
#define hundred 0x50
#define thousand 0x51
#define million 0x52

//Words General
#define ac 0x53
#define amps 0x54
#define band 0x55
#define boardcasting 0x56
#define calling 0x57
#define come 0x58
#define contact 0x59
#define db 0x5a
#define dc 0x5b
#define degrees 0x5c
#define first 0x5d
#define frequency 0x5e
#define giga 0x5f
#define goodbye 0x60
#define hello 0x61
#define here 0x62
#define hertz 0x63
#define _in 0x64
#define incoming 0x65
#define it 0x66
#define key 0x67
#define kilo 0x68
#define last 0x69
#define level 0x6a
#define mega 0x6b
#define off 0x6c
#define ohms 0x6d
#define on 0x6e
#define operator 0x6f
#define _out 0x70
#define over 0x71
#define radio 0x72
#define ready 0x73
#define receiving 0x74
#define rf 0x75
#define sent 0x76
#define sleep 0x77
#define the 0x78
//#define two 0x79
#define transmitting 0x7a
#define volts 0x7b
#define watts 0x7c

//Setup 
#include <SPI.h>    // include the SPI communcation library
// Connect pin#13 (SCLK) To S2 on AP23 
// Connect pin#11 (DATOUT) To S3 on AP23 
// Connect pin#13 (SCLK) To S2 on AP23 
// Connect pin#10 (CS) To S1 on AP23
// GPIO 9 = STOP
// DO from Ap23 optional S4 - Not used here
// ** Add stop bit so that MCU knows then  the chip has stopped talking.  OUT1 is what you'll want to use
#define STOP 9      // The STOP wire is the busy signal of the 
#define PLAY 0x98
#define RAMPUP 0xA8 //COUT ramp up - this value never changes
#define RAMPDOWN 0xB8 //COUT ram down
int cs = 10;

int sentence[] = {k, d, eight, b, x, _p};
int sentence2[] = {kilo, delta, eight, bravo, xray, papa};


void setup() {
  // put your setup code here, to run once:
 pinMode(STOP,INPUT);     // Set the "STOP" GPIO as an input.  This is the busy signal, and is high when the shield is busy playing a word
  SPI.begin();             // Initialize SPI
  SPI.setClockDivider(SPI_CLOCK_DIV32); // low frequency SPI
  pinMode(cs,OUTPUT);    // Chip select pins is an output
  digitalWrite(cs,HIGH); // Set chip select to be HIGH (5v) by default.  The chip on the shield is selected when this line is brought low. 
  SPI.setBitOrder(MSBFIRST);  // OTP requires MSB first
  SPI.setDataMode(SPI_MODE0);  // Use MODE0, as all other modes to not work
  delay(1000);   // One second delay
  Serial.begin(9600);
}

#define arr_len( x ) ( sizeof ( x ) / sizeof (*x) )

void loop() {

  //say the first sentence (kd8bxp)
  Serial.print(arr_len(sentence));
  // put your main code here, to run repeatedly:
for (int i = 0; i < arr_len(sentence); i++) {
  say(sentence[i]);
}

delay(1000);

//say the second sentence (kilo delta eight bravo xray papa)
 Serial.println(arr_len(sentence2));
  // put your main code here, to run repeatedly:
for (int i = 0; i < arr_len(sentence2); i++) {
  say(sentence2[i]);
}

delay(1000);

//say all the words. using the words as counters
for (int i = a; i < watts; i++) {
  say(i);
}

delay(1000);

//we can just call say(xxx)

//say(red);
//say(alert);
//delay(1000);

while(1);
}

void say(int value)    // Calling this function reads words individually
{
  // ramp up
  digitalWrite(cs,LOW);
  SPI.transfer(RAMPUP);
  SPI.transfer(0x00);
  digitalWrite(cs,HIGH);
  delay(7);
  // Transmit Data
  digitalWrite(cs,LOW);
  SPI.transfer(PLAY);
  SPI.transfer(value);
  digitalWrite(cs,HIGH);
  delay(5);
  while (digitalRead(STOP) == HIGH) { 
       {}
    } 
  //delay(5);
  // ramp down
  digitalWrite(cs,LOW);
  SPI.transfer(RAMPDOWN);
  SPI.transfer(0x00);
  digitalWrite(cs,HIGH);
  delay(10);
  // YOU REALLY NEED TO ADD A STOP INPUT HERE, OR ELSE YOU'RE REALLY WASTING POWER!
}
