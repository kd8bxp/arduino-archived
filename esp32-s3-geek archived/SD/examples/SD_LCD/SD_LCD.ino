#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <JPEGDecoder.h>

TFT_eSPI tft = TFT_eSPI(135, 240); 
SPIClass sdSPI(HSPI);
#define SD_MISO     37
#define SD_MOSI     35
#define SD_SCLK     36
#define SD_CS       34
 
void drawSdJpeg(const char *filename, int xpos, int ypos);
void jpegRender(int xpos, int ypos);
void jpegInfo();
void showTime(uint32_t msTime);
void SD_read_Time(uint32_t msTime);
 
void setup()
{
  Serial.begin(115200);
  Serial.println();

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(TFT_MAGENTA);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);
  tft.setSwapBytes(true);
  delay(500);
 
  if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
     pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
     digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
   }
  
  // Mount the file system
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  sdSPI.setClockDivider(SPI_CLOCK_DIV2);
  if (!SD.begin(SD_CS, sdSPI))
  {
    Serial.println("Failed to mount the memory card Procedure");
    return;
  }
  uint8_t cardType = SD.cardType();
 
  if (cardType == CARD_NONE)
  {
    Serial.println("Disconnected storage");
    return;
  }
  else if (cardType == CARD_MMC)
  {
    Serial.println("The MMC card is mounted");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("The SDSC card is mounted");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("The SDHC card is mounted");
  }
  else
  {
    Serial.println("An unknown memory card is mounted");
  }
 
  //Print memory card information
  Serial.printf("The total memory card size is: %lluMB \n", SD.cardSize() / (1024 * 1024)); // "/ (1024 * 1024)"Can be replaced with">> 20"
  Serial.printf("The total file system size is: %lluB \n", SD.totalBytes());
  Serial.printf("The used file system size is: %lluB \n", SD.usedBytes());
}
 
void loop() {
 
  // Test the wallpaper
  for(int image_num = 1;image_num<=8;image_num++){
    char FileName[10];
    sprintf(FileName,"/%d.jpg",image_num);
    drawSdJpeg(FileName, 0, 0);     // This draws a jpeg pulled off the SD Card
    delay(1000);
  }
 
  // //Play folder A <--Gif
  // for(int image_num = 1;image_num<=(105-3);image_num+=1){
  //   char FileName[10];
  //   sprintf(FileName,"/A/%d.jpg",image_num);
  //   drawSdJpeg(FileName, 0, 0);     // This draws a jpeg pulled off the SD Card
  // }
  // delay(2000);
 
}
 
void drawSdJpeg(const char *filename, int xpos, int ypos) {
  uint32_t readTime = millis();
  // Open the named file (the Jpeg decoder library will close it)
  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !jpegFile ) {
    Serial.print("ERROR: File \"");
    Serial.print(filename);
    Serial.println ("\" not found!");
    return;
  }
 
  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");
 
  // Use one of the following methods to initialise the decoder:
  boolean decoded = JpegDec.decodeSdFile(jpegFile);  // Pass the SD file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)
  SD_read_Time(millis() - readTime);
 
  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();
    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}
 
//####################################################################################################
// Draw a JPEG on the TFT, images will be cropped on the right/bottom sides if they do not fit
//####################################################################################################
// This function assumes xpos,ypos is a valid screen coordinate. For convenience images that do not
// fit totally on the screen are cropped to the nearest MCU size and may leave right/bottom borders.
void jpegRender(int xpos, int ypos) {
  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();
 
  //jpegInfo(); // Print information from the JPEG file (could comment this line out)
 
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;
 
  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);
  
  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = (mcu_w<(max_x % mcu_w)?mcu_w:(max_x % mcu_w));
  uint32_t min_h = (mcu_h<(max_y % mcu_h)?mcu_h:(max_y % mcu_h));
 
  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;
 
  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;
 
  // Fetch data from the file, decode and display
  while (JpegDec.read()) {    // While there is more data in the file
    pImg = JpegDec.pImage ;   // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)
 
    // Calculate coordinates of top left corner of current MCU
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;
 
    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;
 
    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;
 
    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }
 
    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;
 
    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    else if ( (mcu_y + win_h) >= tft.height())
      JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  }
 
  tft.setSwapBytes(swapBytes);
 
  showTime(millis() - drawTime); //The time it takes to bring the picture to the screen (ms)
}
 
void jpegInfo() {
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print("Width      :");
  Serial.println(JpegDec.width);
  Serial.print("Height     :");
  Serial.println(JpegDec.height);
  Serial.print("Components :");
  Serial.println(JpegDec.comps);
  Serial.print("MCU / row  :");
  Serial.println(JpegDec.MCUSPerRow);
  Serial.print("MCU / col  :");
  Serial.println(JpegDec.MCUSPerCol);
  Serial.print("Scan type  :");
  Serial.println(JpegDec.scanType);
  Serial.print("MCU width  :");
  Serial.println(JpegDec.MCUWidth);
  Serial.print("MCU height :");
  Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}
 
void showTime(uint32_t msTime) {
  Serial.print(F(" JPEG drawn in "));
  Serial.print(msTime);
  Serial.println(F(" ms "));
}
 
void SD_read_Time(uint32_t msTime) {
  Serial.print(F(" SD JPEG read in "));
  Serial.print(msTime);
  Serial.println(F(" ms "));
}