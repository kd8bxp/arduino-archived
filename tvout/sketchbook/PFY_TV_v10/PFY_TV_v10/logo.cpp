#include <avr/pgmspace.h>
#include <TVout.h>
#include "logo_bitmap.h"
extern TVout TV;

void logo_draw()
{
   TV.bitmap(15, 15, logo_bitmap);
}

