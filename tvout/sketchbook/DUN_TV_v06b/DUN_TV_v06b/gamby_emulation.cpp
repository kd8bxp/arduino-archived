#include "gamby_emulation.h"

extern TVout TV;

byte GambyEmulation::current_x;
byte GambyEmulation::current_y;
unsigned int GambyEmulation::drawPattern;
byte GambyEmulation::inputs = 0;

void GambyEmulation::init()
{
}

void GambyEmulation::update() {
}

void GambyEmulation::clearScreen() {
}


void GambyEmulation::setPos(byte y, byte x)
{
     current_x = (x * 8);
     current_y = y;
} 
   

void GambyEmulation::print_P(const char *)
{
//   for (int c=0; c<s.length; c++)
//   {
//     drawChar(s[c]);
//  }     
}

   
void GambyEmulation::print(char *) {
 //    String num = Integer.toString(n);
 //    char[] buff = num.toCharArray();
 //    for (int c=0; c<buff.length; c++)
 //    {
 //      drawChar(buff[c]);
 //    }     
}
   
void GambyEmulation::print(unsigned i) {
}

void GambyEmulation::drawChar(char c)
{
  /*
   if (c == '\n')
   {
      current_x += 8;
      current_y = 0;
      return;
    }
     
    if (c < 32) c = 32;
    long d = (long)pgm_read_dword(&font[byte(c) - 32]);  // character data
    byte w = (byte)(d & 0x0F);  // character width
    byte b = (byte)((d >> 4) & 0x07); // character baseline offset
	   
    byte col = 0;
       
    for (; w > 0; --w)
    {
         for (byte j=0; j<5;j++)
         {
           col = (col << 1) | ((d >> 31) & 1);
           d = d << 0x01;
         }  
         col = col << (b+1);

         drawByte((byte)col);
         current_y++;
    }
    current_y++;
    
    */
}
   
void GambyEmulation::setPixel(char x, char y)
{
  TV.set_pixel(x,y,1);
}

void GambyEmulation::setPixel(char x, char y, char p)
{
  TV.set_pixel(x,y,p);
}
   
void GambyEmulation::drawSprite(byte x, byte y, const prog_uchar *sprite)
{
  unsigned int w = pgm_read_byte_near(sprite);
  unsigned int h = pgm_read_byte_near(++sprite);

  byte this_byte;
  byte this_bit=8;
  for (byte i=0; i<w; i++) {
    for (byte j=h; j>0; j--) {
      if (this_bit == 8) {
        sprite++;
        this_byte = ~pgm_read_byte_near(sprite);
        this_bit=0;
      }
      setPixel(x+i,y+j,(this_byte << this_bit) & B10000000);
      this_bit++;
    }
  }

}
   
void GambyEmulation::drawSprite(byte x, byte y, const prog_uchar *sprite, byte frame)
{
  unsigned int w = pgm_read_byte_near(sprite);
  unsigned int h = pgm_read_byte_near(++sprite);
  
  byte this_byte;
  byte this_bit=8;
   
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
      setPixel(x+i,y+j,(this_byte << this_bit) & B10000000);
      this_bit++;
    }
  }     
     
}

void GambyEmulation::drawSprite(byte x, byte y, const prog_uchar *sprite, const prog_uchar *mask) {
  byte w = pgm_read_byte_near(sprite);
  byte h = pgm_read_byte_near(++sprite);

  mask++;
  byte this_byte, mask_byte;
  byte this_bit=8;
  for (byte i=0; i<w; i++) {
    for (byte j=h; j>0; j--) {
      if (this_bit == 8) {
        this_byte = ~pgm_read_byte_near(++sprite);
        mask_byte = ~pgm_read_byte_near(++mask);
        this_bit=0;
      }
      if (!((mask_byte << this_bit) & B10000000))
        setPixel(x+i,y+j, (this_byte << this_bit) & B10000000);
        this_bit++;
    }
  }
}

void GambyEmulation::drawSprite(byte x, byte y, const prog_uchar *sprite, byte frame, 
				   const prog_uchar *mask, byte maskFrame) {
  byte w = pgm_read_byte_near(sprite);
  byte h = pgm_read_byte_near(++sprite);

  mask++;
  byte this_byte, mask_byte;
  byte this_bit=8;

  // TODO: This can probably be optimized.
  if (frame > 0 || maskFrame > 0) {
    // Frame size in bits
    unsigned int frameSize = h * w;
    // Convert frame size to bytes, rounding up
    frameSize =  (frameSize & 0x7 ? ((frameSize &  0xfff8) + 8) : frameSize) >> 3;
    sprite += frame * frameSize;
    mask += maskFrame * frameSize;
  }

  for (byte i=0; i<w; i++) {
    for (byte j=h; j>0; j--) {
      if (this_bit == 8) {
        this_byte = ~pgm_read_byte_near(++sprite);
        mask_byte = ~pgm_read_byte_near(++mask);
        this_bit=0;
      }
      if (!((mask_byte << this_bit) & B10000000))
        setPixel(x+i,y+j, (this_byte << this_bit) & B10000000);
        this_bit++;
    }
  }
}
   
//added for internal functions   
void GambyEmulation::drawByte(byte data)
{     
  for (byte pos=0; pos< 7; pos++)
     {
        if (bitRead(data, pos)) setPixel(current_x+pos, current_y,1);
     }
}
   
void GambyEmulation::rect(char x1, char y1, char x2, char y2)
{
    for (int x = x1; x <= x2; x++) {
       for (int y = y1; y <= y2; y++) {
          setPixel(x, y);
       }
    }
} 

//to do: emulate Gamby inputs with Nunchuck read
void GambyEmulation::readInputs()
{
}
