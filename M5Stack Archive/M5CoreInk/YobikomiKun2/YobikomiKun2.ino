#include "src/M5CoreInk/M5CoreInk.h"
#include "pitches.h"
#include "image_yobikomi.h"

Ink_Sprite InkPageSprite(&M5.M5Ink);

#define NOTE_1 16
#define NOTE_2 NOTE_1 / 2
#define NOTE_4 NOTE_1 / 4
#define NOTE_4P NOTE_1 / 4 +  NOTE_1 / 8
#define NOTE_8 NOTE_1 / 8

//16分音符を単位として、3和音分のデータを12小節分保持する。
int musicData[3][12 * 16];
int musicSize = 12 * 16;

//主旋律
int musicData1[][2] = {
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

//伴奏１
int musicData2[][2] = {
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},

  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},

  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},

  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},

  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},

  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},
  {0, NOTE_8},
  {NOTE_A4, NOTE_8},

  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},

  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},

  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},

  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},
  {0, NOTE_8},
  {NOTE_G4, NOTE_8},

  {NOTE_G4, NOTE_1},

  {0, NOTE_1},
};

//伴奏２
int musicData3[][2] = {
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},

  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},

  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},

  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},

  {0, NOTE_8},
  {NOTE_F4, NOTE_8},
  {0, NOTE_8},
  {NOTE_F4, NOTE_8},
  {0, NOTE_8},
  {NOTE_F4, NOTE_8},
  {0, NOTE_8},
  {NOTE_F4, NOTE_8},

  {0, NOTE_8},
  {NOTE_F4, NOTE_8},
  {0, NOTE_8},
  {NOTE_F4, NOTE_8},
  {0, NOTE_8},
  {NOTE_F4, NOTE_8},
  {0, NOTE_8},
  {NOTE_F4, NOTE_8},

  {0, NOTE_8},
  {NOTE_D4, NOTE_8},
  {0, NOTE_8},
  {NOTE_D4, NOTE_8},
  {0, NOTE_8},
  {NOTE_D4, NOTE_8},
  {0, NOTE_8},
  {NOTE_D4, NOTE_8},

  {0, NOTE_8},
  {NOTE_D4, NOTE_8},
  {0, NOTE_8},
  {NOTE_D4, NOTE_8},
  {0, NOTE_8},
  {NOTE_D4, NOTE_8},
  {0, NOTE_8},
  {NOTE_D4, NOTE_8},

  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},

  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},
  {0, NOTE_8},
  {NOTE_E4, NOTE_8},

  {NOTE_E4, NOTE_1},

  {0, NOTE_1},
};

/*

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

*/



void setup() {
  M5.begin();
  if ( !M5.M5Ink.isInit())
  {
    Serial.printf("Ink Init faild");
    while (1) delay(100);
  }
  M5.M5Ink.clear();

  if ( InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0 )
  {
    Serial.printf("Ink Sprite creat faild");
  }
  InkPageSprite.drawBuff(0, 0, 200, 89, (uint8_t*)image_yobikomi);
  InkPageSprite.pushSprite();

  M5.Speaker.begin();
  delay(5000);
  makeMusicData();
  playYobikomi();
}

void loop() {

}

void makeMusicData() { //16分音符に分解
  int musicIndex = 0;
  for (int index = 0; index < sizeof(musicData1) / sizeof(*musicData1); index++) {
    for (int noteCount = 0; noteCount < musicData1[index][1]; noteCount++) {
      if (musicIndex < musicSize) {
        if (noteCount !=  musicData1[index][1] - 1) {
          musicData[0][musicIndex] = musicData1[index][0];
        } else {
          musicData[0][musicIndex] = 0;
        }
      }
      musicIndex++;
    }
  }

  musicIndex = 0;
  for (int index = 0; index < sizeof(musicData2) / sizeof(*musicData2); index++) {
    for (int noteCount = 0; noteCount < musicData2[index][1]; noteCount++) {
      if (musicIndex < musicSize) {
        if (noteCount !=  musicData2[index][1] - 1) {
          musicData[1][musicIndex] = musicData2[index][0];
        } else {
          musicData[1][musicIndex] = 0;
        }
      }
      musicIndex++;
    }
  }
  musicIndex = 0;
  for (int index = 0; index < sizeof(musicData3) / sizeof(*musicData3); index++) {
    for (int noteCount = 0; noteCount < musicData3[index][1]; noteCount++) {
      if (musicIndex < musicSize) {
        if (noteCount !=  musicData3[index][1] - 1) {
          musicData[2][musicIndex] = musicData3[index][0];
        } else {
          musicData[2][musicIndex] = 0;
        }
      }
      musicIndex++;
    }
  }
}

void playYobikomi() {
  for (int index = 0; index < musicSize; index++) {
    Serial.printf("musicData[%d][%d][%d]\n", musicData[0][index], musicData[1][index], musicData[2][index]);
    for (int count = 0; count < 10; count++) {
      if (musicData[0][index] != 0) {
        M5.Speaker.tone(musicData[0][index], 10);
      } else {
        M5.Speaker.mute();
      }
      delay(5);
      if (musicData[1][index] != 0) {
        M5.Speaker.tone(musicData[1][index], 10);
      } else {
        M5.Speaker.mute();
      }
      delay(5);
      if (musicData[2][index] != 0) {
        M5.Speaker.tone(musicData[2][index], 10);
      } else {
        M5.Speaker.mute();
      }
      delay(5);
    }
  }
  M5.Speaker.mute();
}
