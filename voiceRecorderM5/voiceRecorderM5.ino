// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html

/* this example has to be compiled with PSRAM enabled. */
#include "src/M5Stack/M5Stack.h"
#include <driver/adc.h>

#define MICROPHONE             34
#define SPEAKER                25
#define BACKLIGHT              32
#define BUFFER_SIZE            1000*1000
#define SAMPLING_FREQUENCY     22050

#define LOWNOISE               true /* set to false to enable backlight dimming */

TFT_eSPI tft = TFT_eSPI();

// set log level to debug for output
void logMemory() {
  log_d("Used PSRAM: %d from: %d", ESP.getPsramSize() - ESP.getFreePsram(), ESP.getPsramSize());
}

int8_t*                           sampleBuffer;
uint32_t                          allocatedSamples;
uint32_t                          currentSample{0};
uint32_t                          lastSample{0};
static hw_timer_t *               sampleTimer{NULL}; /* only one timer is (re)used for both sampling and playback */
unsigned int                      sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
bool                              samplerRunning{false};

static void IRAM_ATTR _sampleISR() {
  if (currentSample >= BUFFER_SIZE) {
    timerEnd(sampleTimer);
    sampleTimer = NULL;
    lastSample = currentSample;
  }
  else
    sampleBuffer[currentSample++] = adc1_get_raw(ADC1_GPIO34_CHANNEL) >> 1; // shifted 1 bit to the right because we set ADC_WIDTH_BIT_9
}

bool startSampler() {
  if (NULL != sampleTimer) return false;
  delay(170); /* to not record the click from releasing the button */
  currentSample = 0;
  sampleTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(sampleTimer, &_sampleISR, true);
  timerAlarmWrite(sampleTimer, sampling_period_us, true);
  timerAlarmEnable(sampleTimer);
  samplerRunning = true;
  return true;
}

void setup() {
  pinMode(MICROPHONE, INPUT);
  pinMode(SPEAKER, OUTPUT);
  logMemory();

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("M5-Stack voicerecorder", 10, 5, 2);

  if (LOWNOISE) {
    pinMode(BACKLIGHT, OUTPUT);
    digitalWrite(BACKLIGHT, HIGH); // This gives the least noise
  } else {
    ledcAttachPin(BACKLIGHT, 0);
    ledcSetup(0, 1300, 16);
    ledcWrite(0, 0xFFFF / 16);     // Dimming the BACKLIGHT will produce more base noise
  }

  if (!ESP.getPsramSize()) {
    tft.setCursor(55, 40);
    tft.print("NO PSRAM FOUND!");
    while (1) delay(100);
  }

  sampleBuffer = (int8_t*)ps_malloc(BUFFER_SIZE * sizeof(int8_t));

  if (!sampleBuffer) {
    Serial.println("Could not allocate sample buffer - SYSTEM HALTED");
    while (1) delay(1000);
  }

  tft.setCursor(15, 40);
  tft.printf("%3.1fkHz %5.1fkB %6.2fs", SAMPLING_FREQUENCY / 1000.0, (BUFFER_SIZE * sizeof(int8_t)) / 1000.0, BUFFER_SIZE / (float)SAMPLING_FREQUENCY);
  tft.drawString("REC", 45, 200, 2);
  tft.drawString("PLAY", 130, 200, 2);
  tft.drawString("STOP", 220, 200, 2);

  adc1_config_width(ADC_WIDTH_BIT_9);
}

void loop() {
  M5.update();
  if (!sampleTimer && M5.BtnA.pressedFor(5)) startSampler();
  if (!sampleTimer && M5.BtnB.pressedFor(5)) startPlayback();

  uint32_t pos = currentSample;
  tft.setCursor(85, 100);
  tft.printf("%3lu%% %6.2fs", map(pos, 0, BUFFER_SIZE - 1, 0, 100), pos / (float)SAMPLING_FREQUENCY);
  tft.setCursor(60, 130);
  tft.printf(" %7i/%7i", pos, BUFFER_SIZE);

  if (sampleTimer && M5.BtnC.pressedFor(5)) {
    if (samplerRunning) {
      lastSample = currentSample;
      samplerRunning = false;
    }
    timerAlarmDisable(sampleTimer);
    timerEnd(sampleTimer);
    sampleTimer = NULL;
  }
  delay(5);
}

void IRAM_ATTR _playThroughDAC_ISR() {
  if (currentSample >= lastSample) {
    timerEnd(sampleTimer);
    sampleTimer = NULL;
    dacWrite(SPEAKER, 0);
  }
  else
    dacWrite(SPEAKER, sampleBuffer[currentSample++]);
}

bool startPlayback() {
  if (NULL != sampleTimer) return false;
  currentSample = 0;
  sampleTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(sampleTimer, &_playThroughDAC_ISR, true);
  timerAlarmWrite(sampleTimer, sampling_period_us, true);
  timerAlarmEnable(sampleTimer);
  return true;
}
