// Images can be converted to XBM format by using the online converter here:
// https://www.online-utility.org/image/convert/to/XBM

// The output must be pasted in a header file, renamed and adjusted to appear
// as as a const unsigned char array in PROGMEM (FLASH program memory).

// The xbm format adds padding to pixel rows so they are a whole number of bytes
// In this example 50 pixel width means 56 bits = 7 bytes
// the 50 height then means array uses 50 x 7 = 350 bytes of FLASH
// The library ignores the padding bits when drawing the image on the display.

// Example of the correct format is shown below

#include <pgmspace.h>  // PROGMEM support header

// Espressif logo 50 x 50 pixel array in XBM format
#define logoWidth  80  // logo width
#define logoHeight 160  // logo height

// Image is stored in this array
PROGMEM const unsigned char logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0D, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x1D, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8E, 0x19, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x9E, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xBE, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xAC, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x31, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x31, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x31, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xD8, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x98, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xE0, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x3C, 0xC0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x27, 0x40, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0x61, 0xC0, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x60, 0xC0, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0xC0, 0x60, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0x80, 0xC0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x10, 0x60, 0x80, 0x81, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x08, 0x60, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 
  0x40, 0x80, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xC0, 0xFF, 
  0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xC0, 0xFF, 0x01, 0x04, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x40, 0xFF, 0x01, 0x04, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0xC0, 0xFF, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x01, 0xC0, 0x07, 0x03, 0x18, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 
  0xC0, 0x02, 0x03, 0x10, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x80, 0x02, 
  0x03, 0x20, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x80, 0x06, 0x03, 0x20, 
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x80, 0x06, 0x02, 0x20, 0x00, 0x00, 
  0x00, 0x00, 0x20, 0x00, 0x80, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 
  0x20, 0x00, 0x80, 0x00, 0x06, 0x40, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 
  0x80, 0xFF, 0x06, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0xFF, 
  0x06, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0xFF, 0x06, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0xFD, 0x06, 0x80, 0x01, 0x00, 
  0x00, 0x00, 0x08, 0x00, 0x80, 0x81, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00, 
  0x08, 0x00, 0x00, 0x01, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 
  0x00, 0x01, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x7F, 
  0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xFF, 0x05, 0x00, 
  0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xFF, 0x05, 0x60, 0x03, 0x00, 
  0x00, 0x00, 0x04, 0x00, 0x00, 0xFF, 0x05, 0xE0, 0x03, 0x00, 0x00, 0x00, 
  0x02, 0x00, 0x00, 0xEF, 0x0D, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 
  0x00, 0x0E, 0x0C, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x0F, 
  0x0C, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x0F, 0x0C, 0xE0, 
  0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x0F, 0x0C, 0xC0, 0x07, 0x00, 
  0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x0C, 0xC0, 0x07, 0x00, 0x00, 0x00, 
  0x02, 0x00, 0x00, 0x43, 0x0C, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x40, 
  0x00, 0xC3, 0x0D, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0xC3, 
  0x0D, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x83, 0x09, 0xE0, 
  0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 0x09, 0xC0, 0x0F, 0x00, 
  0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 0x08, 0x80, 0x0F, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0x03, 0x08, 0xC0, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 
  0x00, 0x03, 0x08, 0xC0, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 
  0x08, 0xC0, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 0x08, 0xC0, 
  0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 0x08, 0xC0, 0x0F, 0x00, 
  0x00, 0x00, 0x01, 0x00, 0x00, 0x1F, 0x08, 0x80, 0x0F, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0xFF, 0x08, 0x80, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x10, 
  0x00, 0xFF, 0x09, 0x80, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFF, 
  0x09, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xEF, 0x09, 0x00, 
  0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xEF, 0x09, 0x00, 0x0E, 0x00, 
  0x00, 0x00, 0x01, 0x20, 0x00, 0xAF, 0x0D, 0x00, 0x0C, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0xBF, 0x0D, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 
  0x00, 0xFD, 0x0D, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xFD, 
  0x0D, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xD9, 0x0D, 0x00, 
  0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xD0, 0x0D, 0x00, 0x0C, 0x00, 
  0x00, 0x00, 0x01, 0x00, 0x00, 0x81, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 
  0x03, 0x00, 0x00, 0x05, 0x0C, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x02, 0x00, 
  0x00, 0x1D, 0x0C, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x7D, 
  0x0C, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0xFD, 0x04, 0x00, 
  0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0xF9, 0x05, 0x00, 0x07, 0x00, 
  0x00, 0x00, 0x02, 0x00, 0x80, 0xF1, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 
  0x02, 0x00, 0x80, 0xC1, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 
  0x80, 0x81, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x80, 0x01, 
  0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x80, 0xC2, 0x06, 0x00, 
  0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x80, 0xCE, 0x06, 0x00, 0x03, 0x00, 
  0x00, 0x00, 0x04, 0x00, 0x80, 0xDE, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 
  0x08, 0x00, 0x80, 0xFE, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 
  0x80, 0xFC, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x80, 0xF8, 
  0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0xC0, 0xF0, 0x02, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xC0, 0xE0, 0x02, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x10, 0x00, 0xC0, 0x40, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x30, 0x00, 0x40, 0x42, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x40, 0x63, 0x03, 0x40, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x40, 0x53, 
  0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0xC0, 0x3B, 0x01, 0x20, 
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0xC0, 0x3B, 0x01, 0x20, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x00, 0xE0, 0xB9, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x00, 0x60, 0xFF, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
  0x60, 0xFF, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x60, 0xEF, 
  0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x60, 0xEF, 0x00, 0x04, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0xE7, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0C, 0x20, 0xF0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x08, 0x20, 0xE0, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
  0x30, 0xE0, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x40, 
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x40, 0x60, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x10, 0x40, 0x30, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0x11, 0x60, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x1B, 0x60, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x1E, 0x20, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xE0, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0xE0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x31, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x8C, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xEC, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xEC, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x11, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x19, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBA, 0x19, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x8F, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x87, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x86, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  };
