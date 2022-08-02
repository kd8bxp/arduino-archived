// JPEGDEC example for Adafruit GFX displays

#include "src/JPEGDEC.h"
// Sample image (truncated) containing a 320x240 Exif thumbnail
// if an image doesn't display, it may have been created using progressive, open gimp, export as a jpg, remove progressive.

//#include "thumb_test.h"
//#include "ncc1701.h" //http://clipart-library.com/clip-art/uss-enterprise-silhouette-11.htm
#include "batman.h" //http://clipart-library.com/clipart/batman-clip-art_19.htm

#include "src/M5CoreInk/M5CoreInk.h"

#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200

Ink_Sprite InkPageSprite(&M5.M5Ink);

JPEGDEC jpeg;

int JPEGDraw(JPEGDRAW *pDraw)
{
if( InkPageSprite.creatSprite(pDraw->x,pDraw->y,pDraw->iWidth,pDraw->iHeight,true) != 0 )
    {
        Serial.printf("Ink Sprite creat faild");
    }
    InkPageSprite.drawBuff(pDraw->x,pDraw->y,pDraw->iWidth,pDraw->iHeight,(uint8_t*)pDraw->pPixels);
    InkPageSprite.pushSprite();
  //M5.Lcd.drawBitmap((int16_t)pDraw->x, (int16_t)pDraw->y, (int16_t)pDraw->iWidth, (int16_t)pDraw->iHeight, pDraw->pPixels);
  return 1;
} /* JPEGDraw() */

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting...");
  
M5.begin();
if( !M5.M5Ink.isInit())
    {
        Serial.printf("Ink Init faild");
        while (1) delay(100);   
    }
    M5.M5Ink.clear();
} /* setup() */

void loop() {
int i;
long lTime;
int iOption[4] = {0, JPEG_SCALE_HALF, JPEG_SCALE_QUARTER, JPEG_SCALE_EIGHTH};
int iCenterX[4] = {0,0,0,0};
int iCenterY[4] = {0,0,0,0};

  for (i=0; i<4; i++)
  {
    M5.M5Ink.clear();
//M5.Lcd.fillScreen(TFT_BLACK);
    //M5.Lcd.startWrite(); // Not sharing TFT bus on PyPortal, just CS once and leave it
    //if (jpeg.openFLASH((uint8_t *)thumb_test, sizeof(thumb_test), JPEGDraw))
    //if (jpeg.openFLASH((uint8_t *)ncc1701, sizeof(ncc1701), JPEGDraw))
    if (jpeg.openFLASH((uint8_t *)batman, sizeof(batman), JPEGDraw))
    
    {
      lTime = micros();
      if (jpeg.hasThumb()) {jpeg.decode(iCenterX[i],iCenterY[i],JPEG_EXIF_THUMBNAIL | iOption[i]); } else { jpeg.decode(iCenterX[i],iCenterY[i], iOption[i]); }
            {
        lTime = micros() - lTime;
        Serial.printf("%d x %d image, decode time = %d us\n", jpeg.getWidth() >> i, jpeg.getHeight() >> i, (int)lTime);
      }
      jpeg.close();
    }
    delay(2000); // pause between images
  } // for i
} /* loop() */
