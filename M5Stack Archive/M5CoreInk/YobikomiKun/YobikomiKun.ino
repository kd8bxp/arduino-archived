#include "src/M5CoreInk/M5CoreInk.h"
#include "pitches.h"
#include "image_yobikomi.h"

Ink_Sprite InkPageSprite(&M5.M5Ink);

#define NOTE_1 2000
#define NOTE_2 NOTE_1 / 2
#define NOTE_4 NOTE_1 / 4
#define NOTE_4P NOTE_1 / 4 +  NOTE_1 / 8
#define NOTE_8 NOTE_1 / 8


int musicData[][2]={
{NOTE_G5, NOTE_8},
{NOTE_G5, NOTE_4},
{NOTE_A5, NOTE_8},
{NOTE_G5, NOTE_8},
{NOTE_E5, NOTE_8},
{NOTE_G5, NOTE_4},
{NOTE_G5, NOTE_8},
{NOTE_G5, NOTE_4},
{NOTE_A5, NOTE_8},
{NOTE_G5, NOTE_8},
{NOTE_E5, NOTE_8},
{NOTE_G5, NOTE_4},

{NOTE_C5, NOTE_8},
{NOTE_C5, NOTE_8},
{NOTE_C5, NOTE_8},
{NOTE_D5, NOTE_8},
{NOTE_E5, NOTE_4P},
{NOTE_C5, NOTE_8},

{NOTE_E5, NOTE_4P},
{NOTE_G5, NOTE_8},
{NOTE_G5, NOTE_2},

{NOTE_C5, NOTE_8},
{NOTE_C5, NOTE_8},
{NOTE_C5, NOTE_8},
{NOTE_D5, NOTE_8},
{NOTE_E5, NOTE_2},

{NOTE_C5, NOTE_8},
{NOTE_C5, NOTE_8},
{NOTE_C5, NOTE_8},
{NOTE_D5, NOTE_8},
{NOTE_E5, NOTE_2},

{NOTE_D5, NOTE_8},
{NOTE_D5, NOTE_8},
{NOTE_D5, NOTE_8},
{NOTE_C5, NOTE_8},
{NOTE_D5, NOTE_4},
{NOTE_E5, NOTE_4},

{NOTE_G5, NOTE_4},
{NOTE_F5, NOTE_4},
{NOTE_E5, NOTE_4},
{NOTE_D5, NOTE_4},

{NOTE_G5, NOTE_8},
{NOTE_G5, NOTE_4},
{NOTE_A5, NOTE_8},
{NOTE_G5, NOTE_8},
{NOTE_E5, NOTE_8},
{NOTE_G5, NOTE_4},

{NOTE_G5, NOTE_8},
{NOTE_G5, NOTE_4},
{NOTE_A5, NOTE_8},
{NOTE_G5, NOTE_8},
{NOTE_E5, NOTE_8},
{NOTE_G5, NOTE_4},

{NOTE_C5, NOTE_1},
{0, NOTE_1},
};

/*
 *
G8 G4 A8 G8 E8 G4
G8 G4 A8 G8 E8 G4
C8 C8 C8 D8 E4+ C8 
E4+ G8 G2
C8 C8 C8 D8 E2 
C8 C8 C8 D8 E2
D8 D8 D8 C8 D4 E4
G4 F4 E4 D4
G8 G4 A8 G8 E8 G4
G8 G4 A8 G8 E8 D4 C1 R1
 * 
 */



void setup() {
    M5.begin();
    if( !M5.M5Ink.isInit())
    {
        Serial.printf("Ink Init faild");
        while (1) delay(100);   
    }
    M5.M5Ink.clear();

    if( InkPageSprite.creatSprite(0,0,200,200,true) != 0 )
    {
        Serial.printf("Ink Sprite creat faild");
    }
    InkPageSprite.drawBuff(0,0,200,89,(uint8_t*)image_yobikomi);
    InkPageSprite.pushSprite();
    
    M5.Speaker.begin();
    delay(5000);
    playYobikomi();
}

void loop() {

}

void playYobikomi(){
  int musicSize = sizeof(musicData)/sizeof(*musicData);
  for(int index = 0;index < musicSize;index++){
    if(musicData[index][0] > 0){
      M5.Speaker.tone(musicData[index][0],musicData[index][1]);
    }
    delay(musicData[index][1]-100);
    M5.Speaker.mute();
    delay(100);
    Serial.printf("%d:%d\n",musicData[index][0],musicData[index][1]);
  }
  M5.Speaker.mute();
}
