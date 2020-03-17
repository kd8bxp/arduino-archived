#include <avr/pgmspace.h>
#include "gamby_emulation.h"

extern GambyEmulation gamby;

//Draws the sprite only where pixels are specified
//(saves having to store a mask for now)

void helper_draw_sprite(byte x, byte y, const prog_uchar *sprite, byte frame) {
  unsigned int w = pgm_read_byte_near(sprite);
  unsigned int h = pgm_read_byte_near(++sprite);

  byte this_byte;
  byte this_bit=8;
  byte draw_bit = 0;
  // TODO: This can probably be optimized.
  if (frame > 0) {
    unsigned int pixelsPerFrame = h * w;
    // Round up to the nearest 8 bits
    sprite += ((pixelsPerFrame & 0x7 ? ((pixelsPerFrame &  0xfff8) + 8) : pixelsPerFrame) >> 3) * frame;
  }

  for (byte i=0; i<w; i++) {
    for (byte j=h; j>0; j--) {
      if (this_bit == 8) {
        sprite++;
        this_byte = ~pgm_read_byte_near(sprite);
        this_bit=0;
      }
      
      draw_bit = (this_byte << this_bit) & B10000000;
      
      if (!draw_bit) gamby.setPixel(x+i,y+j,draw_bit);
      this_bit++;
    }
  }
}
