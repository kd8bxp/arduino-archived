/* -------------------------------------------------
Copyright (c)
Arduino project by Tech Talkies YouTube Channel
https://www.youtube.com/@techtalkies1
-------------------------------------------------*/

// https://techtalkies.in/2024/11/10/arduino-audio-spectrum-visualizer/?i=1
// Small modifications, and changes for use with the Kano Pixel Kit by LeRoy Miller, KD8BXP June 1, 2025


#include "src/Adafruit_NeoMatrix/Adafruit_NeoMatrix.h"
#include "src/arduinoFFT/arduinoFFT.h"

#define SAMPLES 128          // Must be a power of 2
#define SAMPLING_FREQ 20000 // 40000 //20000  // Hz, must be 40000 or less due to ADC conversion time.
#define AMPLITUDE 51        // Adjust sensitivity here
#define AUDIO_IN_PIN 39 //A0      // Pin for Audio signal
#define LED_PIN 4            // Pin for Neopixel matrix
#define NOISE 75 //20 //500            // Noise threshold
#define NUM_BANDS 8          // Do not change this for this project
#define TOP 8                // 8 for the 8x8 matrix. Can be lower but not higher

// Sampling and FFT stuff
unsigned int sampling_period_us;
byte peak[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  // The length of these arrays must be >= NUM_BANDS
int prevBarHeights[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int bandValues[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
float vReal[SAMPLES];
float vImag[SAMPLES];
unsigned long newTime;

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, SAMPLES, SAMPLING_FREQ); /* Create FFT object */

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 16, LED_PIN,
                                               NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG/*PROGRESSIVE*/,
                                               NEO_GRB + NEO_KHZ800);

uint16_t colors[] = {
  matrix.Color(150, 0, 0),
  matrix.Color(150, 150, 0),
  matrix.Color(0, 150, 0),
  matrix.Color(0, 0, 150),
  matrix.Color(0, 150, 150),
  matrix.Color(150, 0, 150),
  matrix.Color(200, 50, 0),
  matrix.Color(0, 200, 50),
};
uint16_t peakColor = matrix.Color(100, 100, 100);

void setup() {
pinMode(2, OUTPUT);
digitalWrite(2, HIGH);
pinMode(AUDIO_IN_PIN, INPUT_PULLUP);
  matrix.begin();
  matrix.setBrightness(15);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
}

void loop() {
  matrix.clear();
  // Reset bandValues[]
  for (int i = 0; i < NUM_BANDS; i++) {
    bandValues[i] = 0;
  }

  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN) * 1.5;
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) {
      /* Slow down processing for visual convenience */
    }
  }

  // Compute FFT
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  // Analyse FFT results
  for (int i = 2; i < (SAMPLES / 2); i++) {
    if (vReal[i] > NOISE) {  //Check the noise threshold

      //16 bands, 12kHz top band
      if (i <= 2) bandValues[0] += (int)vReal[i];
      if (i > 2 && i <= 3) bandValues[1] += (int)vReal[i];
      if (i > 3 && i <= 5) bandValues[2] += (int)vReal[i];
      if (i > 5 && i <= 7) bandValues[3] += (int)vReal[i];
      if (i > 7 && i <= 9) bandValues[4] += (int)vReal[i];
      if (i > 9 && i <= 13) bandValues[5] += (int)vReal[i];
      if (i > 13 && i <= 18) bandValues[6] += (int)vReal[i];
      if (i > 18 && i <= 25) bandValues[7] += (int)vReal[i];
      if (i > 25 && i <= 36) bandValues[8] += (int)vReal[i];
      if (i > 36 && i <= 50) bandValues[9] += (int)vReal[i];
      if (i > 50 && i <= 69) bandValues[10] += (int)vReal[i];
      if (i > 69 && i <= 97) bandValues[11] += (int)vReal[i];
      if (i > 97 && i <= 135) bandValues[12] += (int)vReal[i];
      if (i > 135 && i <= 189) bandValues[13] += (int)vReal[i];
      if (i > 189 && i <= 264) bandValues[14] += (int)vReal[i];
      if (i > 264) bandValues[15] += (int)vReal[i];
    }
  }

  // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++) {
    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((prevBarHeights[band] * 1) + barHeight) / 2;

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    } else {
      if (peak[band] > 0) peak[band] -= 1;
    }

    drawBars(band, barHeight);
    drawPeak(band);

    // Save oldBarHeights for averaging later
    prevBarHeights[band] = barHeight;
  }
  matrix.show();
}

void drawBars(int band, int barHeight) {
  for (int x = band; x < band + 1; x++) {
    for (int y = TOP; y >= TOP - barHeight; y--) {
      matrix.drawPixel(x, y, colors[x]);
    }
  }
}

void drawPeak(int band) {
  int peakHeight = TOP - peak[band] - 1;
  for (int x = band; x < band + 1; x++) {
    matrix.drawPixel(x, peakHeight, peakColor);
  }
}
