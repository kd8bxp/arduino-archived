#include "src/U8g2/U8x8lib.h"
#include <SPI.h>

U8X8_SH1107_64X128_4W_HW_SPI u8x8(14, /* dc=*/ 27, /* reset=*/ 33);

void setup() {
  u8x8.begin();
}

static uint8_t *rotate90(uint8_t *buf)
{
  static uint8_t rbuf[8];
  uint8_t i, h;
  uint8_t *p;
  uint8_t j;
  for( i = 0; i < 8; i++)
    rbuf[i] = 0;
  for( i = 0; i < 8; i++ )
  {
    h = buf[i];
    p = rbuf;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
  }
  return rbuf;
}

void u8x8_draw_glyph_90(u8x8_t *u8x8, uint8_t x, uint8_t y, uint8_t encoding)
{
  static uint8_t buf[8];
  u8x8_get_glyph_data(u8x8, encoding, buf, 0);
  u8x8_DrawTile(u8x8, x, y, 1, rotate90(buf));
}

void u8x8_draw_string_90(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s)
{
  while(*s != '\0')
   u8x8_draw_glyph_90(u8x8, x, y++, *s++); 
}

void loop(void)
{
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,1,"Hello World");
  u8x8_draw_string_90(u8x8.getU8x8(), 0, 2, "Hello World");
  
  u8x8.refreshDisplay();    // only required for SSD1606/7  
  delay(2000);
}
