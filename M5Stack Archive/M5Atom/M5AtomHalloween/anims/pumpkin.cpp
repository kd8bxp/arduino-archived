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
#include "../src/M5Atom/M5Atom.h"

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
