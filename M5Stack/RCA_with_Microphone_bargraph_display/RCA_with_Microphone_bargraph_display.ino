//The RCA Unit works with The Core unit that has the black connector on it
// it uses the I2S bus, and that is on the black connector
// it may work with other controllers that use PIN 26
// maybe a small adaptor could be made for the StickC

#include "src/M5Unified/M5Unified.h"
#include "src/M5GFX/M5UnitRCA.h"
#include "src/arduinoFFT/arduinoFFT.h"

arduinoFFT FFT = arduinoFFT();
//M5UnitRCA gfx_rca;

static constexpr size_t BAR_COUNT = 8; //64;
static int max_y[BAR_COUNT];
static int prev_y[BAR_COUNT];
static uint32_t colors[BAR_COUNT];

#define SAMPLES 1024 //512              // Must be a power of 2
#define SAMPLING_FREQUENCY 40000 
// Hz, must be 40000 or less due to ADC conversion time.
// Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.

int micpin = 34; //change this to 36 if you are using the fc-04

 M5UnitRCA gfx_rca ( 216                            // logical_width
                   , 144                            // logical_height
                   , 256                            // output_width
                   , 160                            // output_height
                   , M5UnitRCA::signal_type_t::NTSC  // signal_type
                   , 25                             // GPIO pin
                   );
// signal_type: can be selected from NTSC / NTSC_J / PAL / PAL_M / PAL_N.
// GPIO pin: can be selected from 25 / 26

static constexpr int color_list[5]   = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_PURPLE, TFT_YELLOW};
static int color_index               = 0;
static unsigned long last_clear_time = 0;
static bool is_pin_25                = true;

struct eqBand {
  const char *freqname;
  uint16_t amplitude;
  int peak;
  int lastpeak;
  uint16_t lastval;
  unsigned long lastmeasured;
};
 
eqBand audiospectrum[8] = {
  //Adjust the amplitude values to fit your microphone
  { "125Hz", 500, 0, 0, 0, 0},
  { "250Hz", 200, 0, 0, 0, 0},
  { "500Hz", 200, 0, 0, 0, 0},
  { "1KHz",  200, 0, 0, 0, 0},
  { "2KHz",  200, 0, 0, 0, 0},
  { "4KHz",  100, 0, 0, 0, 0},
  { "8KHz",  100, 0, 0, 0, 0},
  { "16KHz", 50,  0, 0, 0, 0}
};
 
unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
uint16_t tft_width  = 255; //216; //256; //320; // ILI9341_TFTWIDTH;
uint16_t tft_height = 120; //144; //160; //240; // ILI9341_TFTHEIGHT;
uint8_t bands = 8; //8;
uint8_t bands_width = floor( tft_width / bands );
uint8_t bands_pad = bands_width - 10;
uint16_t colormap[255]; // color palette for the band meter (pre-fill in setup)
 

void setup(void) {
    auto cfg         = M5.config();
    cfg.external_spk = true;
    cfg.internal_spk = false;

    M5.begin();

    M5.Display.clear(TFT_BLACK);
     M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(YELLOW, BLACK);
  M5.Lcd.setTextSize(1);
  //M5.Lcd.setRotation(0);
sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  delay(2000);
  for(uint8_t i=0;i<tft_height;i++) {
    colormap[i] = M5.Lcd.color565(tft_height-i*.5, i*1.1, 0);
  }
  for (byte band = 0; band <= 7; band++) {
    M5.Lcd.setCursor(bands_width*band + 2, 0);
    M5.Lcd.print(audiospectrum[band].freqname);
  }
     if (M5.getBoard() == m5gfx::boards::board_M5StackCore2) {
         {
             auto spk_cfg        = M5.Speaker.config();
             spk_cfg.i2s_port    = I2S_NUM_1;
             spk_cfg.sample_rate = 96000;

             spk_cfg.pin_data_out = 2;
             spk_cfg.pin_bck      = 19;
             spk_cfg.pin_ws       = 0;  // LRCK

             spk_cfg.stereo        = true;
             spk_cfg.buzzer        = false;
             spk_cfg.use_dac       = false;
             spk_cfg.magnification = 16;
             M5.Speaker.config(spk_cfg);
         }
     } else {
         auto spk_cfg        = M5.Speaker.config();
         spk_cfg.i2s_port    = I2S_NUM_1;
         spk_cfg.sample_rate = 96000;

         spk_cfg.pin_data_out = 15;
         spk_cfg.pin_bck      = 13;
         spk_cfg.pin_ws       = 0;  // LRCK

         spk_cfg.stereo        = true;
         spk_cfg.buzzer        = false;
         spk_cfg.use_dac       = false;
         spk_cfg.magnification = 16;
         M5.Speaker.config(spk_cfg);
     }

     M5.Speaker.begin();
     M5.Speaker.setVolume(128);
     M5.Speaker.setChannelVolume(0, 255);
     M5.Speaker.setChannelVolume(1, 255);

    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();



     for (int x = 0; x < BAR_COUNT; ++x)
  {
    prev_y[x] =  gfx_rca.height();
    max_y[x] =  gfx_rca.height();
    int r=0,g=0,b=0;
    switch (x >> 4)
    {
    case 0:
      b = 255;
      g = x*0x11;
      break;
    case 1:
      b = 255 - (x&15)*0x11;
      g = 255;
      break;
    case 2:
      g = 255;
      r = (x&15)*0x11;
      break;
    case 3:
      r = 255;
      g = 255 - (x&15)*0x11;
      break;
    }
    colors[x] =  gfx_rca.color888(r,g,b);
  }

    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
}

int blink_flag = 0;
void loop(void) {

   for (int i = 0; i < SAMPLES; i++) {
    newTime = micros()-oldTime;
    oldTime = newTime;
    vReal[i] = analogRead(micpin); // A conversion takes about 1uS on an ESP32
    vImag[i] = 0;
    while (micros() < (newTime + sampling_period_us)) { 
      // do nothing to wait
    }
  }
  FFT.DCRemoval(vReal, SAMPLES);
  //FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
 
  for (int i = 2; i < (SAMPLES/2); i++){ 
    // Don't use sample 0 and only first SAMPLES/2 are usable. 
    // Each array eleement represents a frequency and its value the amplitude.
    if (vReal[i] > 1500) { // Add a crude noise filter, 10 x amplitude or more
      byte bandNum = getBand(i);
      if(bandNum!=8) {
        displayBand(bandNum, (int)vReal[i]/audiospectrum[bandNum].amplitude);
      }
    }
  } 

    long vnow = millis();
  for (byte band = 0; band <= 7; band++) {
  //for (byte band = 4; band <= 7; band++) {
    // auto decay every 50ms on low activity bands
    if(vnow - audiospectrum[band].lastmeasured > 50) {
      displayBand(band, audiospectrum[band].lastval>4 ? audiospectrum[band].lastval-4 : 0);
    }
    if (audiospectrum[band].peak > 0) {
      audiospectrum[band].peak -= 2;
      if(audiospectrum[band].peak<=0) {
        audiospectrum[band].peak = 0;
      }
    }
    // only draw if peak changed
    if(audiospectrum[band].lastpeak != audiospectrum[band].peak) {
      // delete last peak
     M5.Lcd.drawFastHLine(bands_width*band,tft_height-audiospectrum[band].lastpeak,bands_pad,BLACK);
     gfx_rca.drawFastHLine(bands_width*band,tft_height-audiospectrum[band].lastpeak,bands_pad,BLACK);
     audiospectrum[band].lastpeak = audiospectrum[band].peak;
     M5.Lcd.drawFastHLine(bands_width*band, tft_height-audiospectrum[band].peak,
                           bands_pad, colormap[tft_height-audiospectrum[band].peak]);
     gfx_rca.drawFastHLine(bands_width*band, tft_height-audiospectrum[band].peak,
                           bands_pad, colormap[tft_height-audiospectrum[band].peak]);                      
    }
    gfx_rca.display();
  }
   

}

void displayBand(int band, int dsize){
  uint16_t hpos = bands_width*band;
  int dmax = 200;
  if(dsize>tft_height-10) {
    dsize = tft_height-10; // leave some hspace for text
  }
  if(dsize < audiospectrum[band].lastval) {
    // lower value, delete some lines
    M5.Lcd.fillRect(hpos, tft_height-audiospectrum[band].lastval,
                    bands_pad, audiospectrum[band].lastval - dsize, BLACK);
    gfx_rca.fillRect(hpos, tft_height-audiospectrum[band].lastval, bands_pad, audiospectrum[band].lastval - dsize, BLACK);                
  }
  if (dsize > dmax) dsize = dmax;
  for (int s = 0; s <= dsize; s=s+4){
    M5.Lcd.drawFastHLine(hpos, tft_height-s, bands_pad, colormap[tft_height-s]);
    gfx_rca.drawFastHLine(hpos, tft_height-s, bands_pad, colormap[tft_height-s]);
  }
  if (dsize > audiospectrum[band].peak) {
    audiospectrum[band].peak = dsize;
  }
  audiospectrum[band].lastval = dsize;
  audiospectrum[band].lastmeasured = millis();
}
 
byte getBand(int i) {
  if (i<=2 )             return 0;  // 125Hz
  if (i >3   && i<=5 )   return 1;  // 250Hz
  if (i >5   && i<=7 )   return 2;  // 500Hz
  if (i >7   && i<=15 )  return 3;  // 1000Hz
  if (i >15  && i<=30 )  return 4;  // 2000Hz
  if (i >30  && i<=53 )  return 5;  // 4000Hz
  if (i >53  && i<=200 ) return 6;  // 8000Hz
  if (i >200           ) return 7;  // 16000Hz
  return 8;
}
