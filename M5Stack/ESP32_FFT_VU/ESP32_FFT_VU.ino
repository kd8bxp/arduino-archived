
#include "src/M5Stack/M5Stack.h"
#include "src/M5GFX/M5UnitRCA.h"
#include "arduinoFFT.h"

M5UnitRCA gfx_rca;

#define SAMPLES         1024          // Must be a power of 2
#define SAMPLING_FREQ   40000         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE       1000          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define AUDIO_IN_PIN    34            // Signal in on this pin
#define NUM_BANDS       16            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define NOISE           500           // Used as a crude noise filter, values below this are ignored
const uint8_t kMatrixWidth = 240;                          // Matrix width
const uint8_t kMatrixHeight = 144;                         // Matrix height

#define BAR_WIDTH      (kMatrixWidth  / (NUM_BANDS - 1))  // If width >= 8 light 1 LED width per bar, >= 16 light 2 LEDs width bar etc
#define TOP            (kMatrixHeight - 0)                // Don't allow the bars to go offscreen

// Sampling and FFT stuff
unsigned int sampling_period_us;
byte peak[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};              // The length of these arrays must be >= NUM_BANDS
int oldBarHeights[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bandValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

void setup() {

    M5.begin();
    M5.Power.begin();
    
    M5.Lcd.begin();
    M5.Lcd.fillScreen(BLACK);
    M5.Speaker.begin();
    M5.Speaker.setVolume(128);
    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();
    Serial.begin(115200);
  

}


void loop(void) {

  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }

  // Compute FFT
  FFT.DCRemoval();
  //FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Windowing(FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  // Analyse FFT results
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter
          //16 bands, 12kHz top band
      if (i<=2 )           bandValues[0]  += (int)vReal[i];
      if (i>2   && i<=3  ) bandValues[1]  += (int)vReal[i];
      if (i>3   && i<=5  ) bandValues[2]  += (int)vReal[i];
      if (i>5   && i<=7  ) bandValues[3]  += (int)vReal[i];
      if (i>7   && i<=9  ) bandValues[4]  += (int)vReal[i];
      if (i>9   && i<=13 ) bandValues[5]  += (int)vReal[i];
      if (i>13  && i<=18 ) bandValues[6]  += (int)vReal[i];
      if (i>18  && i<=25 ) bandValues[7]  += (int)vReal[i];
      if (i>25  && i<=36 ) bandValues[8]  += (int)vReal[i];
      if (i>36  && i<=50 ) bandValues[9]  += (int)vReal[i];
      if (i>50  && i<=69 ) bandValues[10] += (int)vReal[i];
      if (i>69  && i<=97 ) bandValues[11] += (int)vReal[i];
      if (i>97  && i<=135) bandValues[12] += (int)vReal[i];
      if (i>135 && i<=189) bandValues[13] += (int)vReal[i];
      if (i>189 && i<=264) bandValues[14] += (int)vReal[i];
      if (i>264          ) bandValues[15] += (int)vReal[i];
    }

 // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++) {

    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((oldBarHeights[band] * 1) + barHeight) / 2;

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    }

//draw Bars
rainbowBars(band, barHeight);
//draw Peaks 
//whitePeak(band);

// Save oldBarHeights for averaging later
    oldBarHeights[band] = barHeight;
 // Decay peak
 // EVERY_N_MILLISECONDS(60) {
   // for (byte band = 0; band < NUM_BANDS; band++)
   //   if (peak[band] > 0) peak[band] -= 1;
//    colorTimer++;
 // }

  // Used in some of the patterns
  //EVERY_N_MILLISECONDS(10) {
  //  colorTimer++;
  //}

 // EVERY_N_SECONDS(10) {
 //   if (autoChangePatterns) buttonPushCounter = (buttonPushCounter + 1) % 6;
//  }

  gfx_rca.display();
}
    
 
}
}

void rainbowBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = TOP; y >= TOP - barHeight; y--) {
      //matrix->drawPixel(x, y, CHSV((x / BAR_WIDTH) * (255 / NUM_BANDS), 255, 255));
      gfx_rca.fillRect(x, y, BAR_WIDTH, 1, TFT_BLACK);
      gfx_rca.display();
      gfx_rca.fillRect(x, y, BAR_WIDTH, 1, TFT_WHITE);
    }
  }
}
