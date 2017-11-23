/* Modified by LeRoy Miller (c) 2017, modification shows how to do the base project with just one Arduino UNO
 *  
 *  Highly based on https://www.instructables.com/id/Add-Lights-and-Spooky-Music-to-Your-Jack-O-Lantern/
 *  By More than the Sum
 *  
 */

#include <Adafruit_NeoPixel.h>
#include <TimedAction.h>

void candle();
void RGBLoop();
void song();
TimedAction candleTrigger = TimedAction(100, candle);
TimedAction RGBTriggerLoop = TimedAction(10, RGBLoop);
TimedAction SongTrigger = TimedAction(5, song);

//initialize the candle, and variables related to the candle
//modified from: https://codebender.cc/sketch:67754#Neo%20Candle.ino
#define lPIN 8    //this is the pin the candle LEDs are connected to
int numpix = 4;  //set the number of Pixels you have/want in the candle - more = brighter
int randNumber, color, wait, randpix;
boolean x = true;
unsigned long prevCandleTime;  //additional variables used to operate candle
int totalPixels = 8;
Adafruit_NeoPixel cstrip = Adafruit_NeoPixel(totalPixels, lPIN, NEO_GRB + NEO_KHZ800);  //define the candle led strip

//initialize the eyes, and variables used to control eyes
// modified from: http://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#fadeinout_rgb
//#define fadePin 8   //this is the pin the glowing eye LEDs are connected to
#define NUM_LEDS 4  //set the number of Pixels you have/want in the eyes - more = brighter
int fadeWait = 3;  // sets speed of eye fade.  smaller = faster, bigger = slower
unsigned long currentFadeTime = 0; int j = 0; int k = 0; boolean fadeIn = true;
//Adafruit_NeoPixel fadeStrip = Adafruit_NeoPixel(NUM_LEDS, fadePin, NEO_GRB + NEO_KHZ800); //define the led strip for the eyes

#define speakerPin 3 //change to match pin for speaker
int numTones = 291; //change this to reflect the number of notes in your song
int cs6 = 1109; // frequencies of the notes used in the song
int fs5 = 740;
int d6 = 1175;
int c6 = 1047;
int f5 = 698;
int b5 = 988;
int e5 = 659;
int bf5 = 932;
int ef5 = 622;
int b4 = 494;
int g5 = 784;
int songWait = 200;  // length of the notes

int tones[] = { cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5, //notes in the song
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5, 
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                cs6, fs5, fs5, cs6, fs5, fs5, cs6, fs5,  d6, fs5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                 c6,  f5,  f5,  c6,  f5,  f5,  c6,  f5, cs6,  f5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                 b5,  e5,  e5,  b5,  e5,  e5,  b5,  e5,  c6,  e5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                bf5, ef5, ef5, bf5, ef5, ef5, bf5, ef5,  b5, ef5,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4,
                fs5,  b4,  b4, fs5,  b4,  b4, fs5,  b4,  g5,  b4};
        
unsigned long current_song_time = 0; unsigned long prev_song_time = 0; int note_count = 0; int on_off = 0; //variables for running the song


void setup() {
  randomSeed(analogRead(2));  //Set random number generator seed from analog pin 2
  
  cstrip.begin();
  cstrip.show(); // Initialize all candle pixels to 'off'
  
  //fadeStrip.begin();
  //fadeStrip.show(); // Initialize all eye pixels to 'off'

}

void loop() {
  candleTrigger.check();
  RGBTriggerLoop.check();
  SongTrigger.check();
}

void candle(){
  if(x){
    wait = random(80, 120);  //set a random wait period
    randpix = random(4, totalPixels + 1); //choose a random number of pixels
    color = random(0, 2); //Pick either yellow or orange
    for (int i = 4; i < totalPixels / 2; i++){ //so it leaves a certain number of yellow pixels on (number of pixels/3)
      cstrip.setPixelColor(i, 255, 120, 0);  //set the number of pixels to turn on and color value (yellowish)
    }
    cstrip.show(); //turn pixels on
  }
  
  if (color == 0){   //if red was chosen
    flickred(cstrip.Color(115, 50, 0), wait, randpix); // call flickred and pass it the red (orangeish) color values - change values to change color
  } else {  // otherwise use yellow
    flickYellow(cstrip.Color(180, 80, 0), wait, randpix); // call flickYellow and pass it the yellow color values (change values to change color), and wait time and random pixel count
  }
}

// Function for when red is chosen
void flickred(uint32_t c, uint8_t wait, uint8_t p) {
  if(x){
    for (int i = 4; i < p - 2; i++){ //loop for given random pixel count (passed from loop)
      cstrip.setPixelColor(i, c);
    }
    cstrip.show();  //turn pixels on
    x = false;
  }else{
    if(millis()-prevCandleTime>wait){
      prevCandleTime = millis();
      for (int i = 4; i < p; i++) {
        cstrip.setPixelColor(i, 0);        //turn pixel off
      }   
      x = true;
    }  
  }
}  //end flickred

// function for when yellow is chosen
void flickYellow(uint32_t c, uint8_t wait, uint8_t p) {
  if(x){
    for (int i = 4; i < p ; i++){ //loop for given random pixel count (passed from loop)
      cstrip.setPixelColor(i, c);
    }
    cstrip.show();  //turn pixels on
    x = false;
  }else{
    if(millis()-prevCandleTime>wait){
      prevCandleTime = millis();
      for (int i = 4; i < p; i++) {
        cstrip.setPixelColor(i, 0);        //turn pixel off
      }
      x = true;
    }
  }
}  //end flickYellow

void RGBLoop(){
  if(millis()-currentFadeTime>fadeWait){
    currentFadeTime = millis();
    if(fadeIn){
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      cstrip.show();
      if(k==255){
        fadeIn=false;
      } else {
        k++;
      }
    } else {  // Fade OUT
      
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      cstrip.show();
      if(k==0){
        fadeIn = true;
        j++;
        j%=3;
      } else {
        k--;
      }
    }
  }
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    cstrip.setPixelColor(i, cstrip.Color(red, green, blue));
  }
  cstrip.show();
}

void song() {
  current_song_time = millis();
  if(on_off == 0)
  {
    if (current_song_time - prev_song_time >= 20){
      if(note_count+1==numTones){
        songWait = 10000;
        on_off++;
        on_off %= 2;
      } else{
        prev_song_time = current_song_time;
        tone(speakerPin, tones[note_count]);
        on_off++;
        on_off %= 2;
      }
    }
  } else {
    if (current_song_time - prev_song_time >= songWait){//delays[note_count]
      prev_song_time = current_song_time;
      noTone(speakerPin);
      note_count++;
      note_count %= numTones;
      on_off++;
      on_off %= 2;
      songWait = 200;
    }
  }
}

