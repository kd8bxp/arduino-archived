/* Atomic Halloween - Halloween animations for M5Atom
 * Copyright (C) 2020 Silent Software (Benjamin Brown)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 /* Oct 13, 2021 slightly modified to work with the Arduino IDE
  Mainly moved the animations to functions, within the main sketch
  otherwise, I got compile errors related to fastled and the functions
  being in .cpp files.
  Original anims directory is left, and original code is there, it's just
  being used at the moment.
  
  -LeRoy Miller, KD8BXp Oct 13, 2021
  All credit to Atomic Halloween - Halloween animations for M5Atom
 * Copyright (C) 2020 Silent Software (Benjamin Brown)
  */
   

#include "src/M5Atom/M5Atom.h"
//#include "anims/pumpkin.h"
//#include "anims/skeleton.h"
//#include "anims/spaceInvader.h"
//#include "anims/ghost.h"

CRGB leds[25];
void (*anims[4])(CRGB* leds) {pumpkin, skeleton, spaceInvader, ghost};

void setup()
{
    M5.begin(false, false, true);
    FastLED.addLeds<WS2812B, 27>(leds, 25);
}

uint8_t animIndex = 0;
int iterations = 1;

/**
 * This code is all a bit of fun for my 1 and 5 year olds
 * - the icons were designed and the program written in a 
 * couple of hours before Halloween - hope you get some
 * enjoyment out of it too!
 * 
 * Feel free to submit new MRs for icons, improved playback,
 * different holidays etc. It's all a bit of pointless fun :)
 * 
 * Nb. if you're snooping for quality code this is not it - 
 * magic numbers, no tests, no code comments etc...!
 */
void loop()
{
   
    (*anims[animIndex])(leds);
    if (iterations % 10 == 0 ) {
       ++animIndex;
       animIndex%=4; 
    }
    ++iterations;
    
}

/* Atomic Halloween - Halloween animations for M5Atom
 * Copyright (C) 2020 Silent Software (Benjamin Brown)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

void ghost(CRGB* leds)
{
    FastLED.clear();
    for (int i = 0; i < 25; ++i)
    {
        leds[i] = 0x444400;
    }
    leds[0] = 0x000000;
    leds[4] = 0x000000;
    leds[11] = 0x00DD00;
    leds[13] = 0x00DD00;
    leds[21] = 0x000000;
    leds[23] = 0x000000;
    FastLED.show();
    delay(500);
    leds[10] = 0x00DD00;
    leds[12] = 0x00DD00;
    leds[11] = 0x444400;
    leds[13] = 0x444400;
    FastLED.show();
    delay(500);
    leds[11] = 0x00DD00;
    leds[13] = 0x00DD00;
    leds[10] = 0x444400;
    leds[12] = 0x444400;
    FastLED.show();
    delay(500);
    leds[12] = 0x00DD00;
    leds[14] = 0x00DD00;
    leds[11] = 0x444400;
    leds[13] = 0x444400;
    FastLED.show();
    delay(500);
}

void pumpkin(CRGB* leds)
{
    FastLED.clear();
    for (int i = 0; i < 25; ++i)
    {
        leds[i] = 0x80FF00;
    }
    leds[0] = 0x000000;
    leds[2] = 0xAA0000;
    leds[4] = 0x000000;
    leds[20] = 0x000000;
    leds[24] = 0x000000;
    leds[6] = 0x000000;
    leds[8] = 0x000000;
    FastLED.show();
    delay(500);
    leds[16] = 0x000000;
    leds[17] = 0x000000;
    leds[18] = 0x000000;
    leds[6] = 0x00DD00;
    leds[8] = 0x00DD00;
    leds[16] = 0x000000;
    leds[17] = 0x000000;
    leds[18] = 0x000000;
    leds[20] = 0x80FF00;
    leds[24] = 0x80FF00;
    delay(500);
}

void skeleton(CRGB* leds)
{
    FastLED.clear();
    for (int i = 0; i < 25; ++i)
    {
        leds[i] = 0x444444;
    }
    leds[6] = 0x000000;
    leds[8] = 0x000000;
    leds[15] = 0x000000;
    leds[17] = 0x000000;
    leds[19] = 0x000000;
    leds[20] = 0x000000;
    leds[24] = 0x000000;
    FastLED.show();
    delay(500);
    leds[15] = 0x444444;
    leds[16] = 0x000000;
    leds[17] = 0x000000;
    leds[18] = 0x000000;
    leds[19] = 0x444444;
    leds[20] = 0x444444;
    leds[24] = 0x444444;
    FastLED.show();
    delay(500);
}

void spaceInvader(CRGB* leds) {
    FastLED.clear();
    for (int i = 0; i < 25; ++i)
    {
        leds[i] = 0x22FF22;
    }
    leds[0] = 0x000000;
    leds[2] = 0x000000;
    leds[4] = 0x000000;
    leds[11] = 0x000055;
    leds[13] = 0x000055;
    leds[15] = 0x000000;
    leds[19] = 0x000000;
    for (int i = 21; i < 24; ++i)
    {
        leds[i] = 0x000000;
    }
    FastLED.show();
    delay(500);
    for (int i = 0; i < 25; ++i)
    {
        leds[i] = 0x22FF22;
    }
    leds[6] = 0x000055;
    leds[8] = 0x000055;
    leds[10] = 0x000000;
    leds[14] = 0x000000;
    for (int i = 16; i < 19; ++i)
    {
        leds[i] = 0x000000;
    }
    for (int i = 20; i < 25; ++i)
    {
        leds[i] = 0x000000;
    }
    FastLED.show();
    delay(500);
}
