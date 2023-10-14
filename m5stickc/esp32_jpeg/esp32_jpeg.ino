//#include <SD.h>
#include "src/M5StickC/M5StickC.h"
#include "src/JPEGDEC.h"
//#include "test_images/gray_road.h"
//#include "test_images/f6t.h"
#include "ncc1701.h"


JPEGDEC jpeg;

//uint8_t ucTXBuf[1024];
static uint8_t *ucTXBuf;

//File file;
// pixel drawing callback
int drawMCU(JPEGDRAW *pDraw)
{
  int iCount = pDraw->iWidth * pDraw->iHeight;
//  Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  spilcdSetPosition(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, 1);
//  spilcdWriteDataBlock((uint8_t *)pDraw->pPixels, iCount*2, 1);
  spilcdWaitDMA();
  memcpy(ucTXBuf, pDraw->pPixels, iCount*sizeof(uint16_t));
  spilcdWriteDataDMA(iCount*2);  
  return 1;
} /* drawMCU() */

void setup()
{
char szTemp[128];

  Serial.begin(115200);
//  spilcdSetTXBuffer(ucTXBuf, sizeof(ucTXBuf));
  ucTXBuf = spilcdGetDMABuffer();
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
} /* setup() */
void loop() {
long lTime;

  //spilcdFill(0,1);

  if (jpeg.open((uint8_t *)thumb_test, sizeof(thumb_test), drawMCU))
  {
    Serial.println("Successfully opened JPEG image");
    Serial.printf("Image size: %d x %d, orientation: %d, bpp: %d\n", jpeg.getWidth(),
      jpeg.getHeight(), jpeg.getOrientation(), jpeg.getBpp());
    if (jpeg.hasThumb())
       Serial.printf("Thumbnail present: %d x %d\n", jpeg.getThumbWidth(), jpeg.getThumbHeight());
    jpeg.setPixelType(BIG_ENDIAN_PIXELS);
    lTime = micros();
    if (jpeg.decode(40,100,JPEG_SCALE_QUARTER | JPEG_EXIF_THUMBNAIL))
    {
      lTime = micros() - lTime;
      Serial.printf("Successfully decoded image in %d us\n", (int)lTime);
    }
    jpeg.close();
  }
  
  delay(10000);
}
