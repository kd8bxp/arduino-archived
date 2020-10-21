#define CHANNEL 0 //Used for Tone Generation
#define TONEPIN 25
//Pin USED for buzzer

void setup() {
 ledcSetup(CHANNEL, 2000, 8);
ledcAttachPin(TONEPIN, CHANNEL);
}

void loop() {
  //my message
  ledcWriteTone(CHANNEL, 800);
    delay(100);
    ledcWriteTone(CHANNEL, 1200);
    delay(100);
    ledcWriteTone(CHANNEL, 0);

    delay(5000);
    //beacon/cq
      ledcWriteTone(CHANNEL, 345);
      delay(100);
      ledcWriteTone(CHANNEL, 600);
      delay(100);
      ledcWriteTone(CHANNEL, 0);

      delay(5000);
      //wx
      ledcWriteTone(CHANNEL, 600);
    delay(100);
    ledcWriteTone(CHANNEL, 1200);
    delay(100);
}
