#include <U8g2lib.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include "Air_Quality_Sensor.h"

U8G2_SH1107_SEEED_128X128_1_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

// Sensor pins
AirQualitySensor sensor(A0);

#define DHTPIN D1
#define DHTTYPE DHT22
#define TOUCH_PIN D2  // Touch sensor pin

DHT dht(DHTPIN, DHTTYPE);

int airQuality = 0;

// Blink parameters
unsigned long nextBlinkTime = 0;
bool isBlinking = false;
unsigned long blinkInterval = 120000; // Interval between blinks (2 minutes)
const unsigned long blinkDuration = 300;  // Duration of each blink (100 milliseconds)
const int blinkCount = 2;                 // Number of blinks per sequence
unsigned long lastBlinkTime = 0;
int currentBlink = 0;


void setup() {
  Serial.begin(115200);
  u8g2.begin();
  dht.begin();
  pinMode(TOUCH_PIN, INPUT);  // Set touch pin as input

  Serial.println("Waiting sensor to init...");
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_helvB12_tr);

  int load = 0;
  while(load < 20)
  {
    u8g2.firstPage(); // Start the page drawing process
    do {
      u8g2.setCursor(10, 50);
      u8g2.print("Loading");
      u8g2.setCursor(10, 70);
      // Print dots dynamically within the page loop to update each frame
      for (int i = 0; i <= load; i++) {
        u8g2.print(".");
      }
    } while (u8g2.nextPage()); // Send the buffer to the display
    delay(1000);
    load++;
  }
  u8g2.clearBuffer(); // Clear buffer after loading is complete

  if (sensor.init()) {
        Serial.println("Sensor ready.");
  } else {
        Serial.println("Sensor ERROR!");
    }
}

void loop() {
  // Read sensor values
  int quality = sensor.slope();
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (quality == AirQualitySensor::FORCE_SIGNAL) {
      airQuality = 3; // very high pollution
  } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
      airQuality = 2; // high pollution
  } else if (quality == AirQualitySensor::LOW_POLLUTION) {
      airQuality = 1; // moderate pollution
  } else if (quality == AirQualitySensor::FRESH_AIR) {
      airQuality = 0; // fresh air
  }
  
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("DHT read error!");
    delay(2000);
    return;
  }

  // Check touch sensor
  bool isTouched = digitalRead(TOUCH_PIN);
  
  if (isTouched) {
    displaySensorData(temp, humidity, airQuality);
  } else {
    int comfortLevel = calculateComfortLevel(airQuality, temp, humidity);
    updateDisplay(comfortLevel);
  }

  // Serial output
  Serial.print("Touch: ");
  Serial.print(isTouched ? "ON | " : "OFF | ");
  Serial.print("Air: ");
  Serial.print(airQuality);
  Serial.print(" | Temp: ");
  Serial.print(temp);
  Serial.print("C | Hum: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(500);
}

int calculateComfortLevel(int air, float temp, float humid) {
  int airLevel = air;
  int tempLevel = (temp >= 20 && temp <= 24) ? 0 : (temp >= 18 && temp <= 26) ? 1 : (temp >= 15 && temp <= 30) ? 2 : 3;
  int humidLevel = (humid >= 40 && humid <= 60) ? 0 : (humid >= 30 && humid <= 70) ? 1 : (humid >= 20 && humid <= 80) ? 2 : 3;
  
  return max(max(airLevel, tempLevel), humidLevel);
}

void updateDisplay(int comfortLevel) {
  unsigned long currentTime = millis();
  
  if (currentTime - lastBlinkTime >= blinkInterval && !isBlinking) {
    isBlinking = true;
    currentBlink = 0;
    
    
     = currentTime; // Start the blink sequence
    blinkInterval = random(60000, 180000); // make the blink interval a random number between 1 and 3 minutes
  }

  if (isBlinking) {
    if (currentBlink < blinkCount * 2) {
      if (currentTime - lastBlinkTime >= (currentBlink % 2 == 0 ? 0 : blinkDuration)) {
        // Toggle blink state
        bool blinkState = (currentBlink % 2 == 0);
        u8g2.firstPage();
        do {
          drawEyes(0, blinkState);
        } while (u8g2.nextPage());
        lastBlinkTime = currentTime;
        currentBlink++;
      }
    } else {
      isBlinking = false;
      lastBlinkTime = currentTime; // Reset for the next blink
    }
  }

  int eyelidHeight = isBlinking ? 60 : comfortLevel * 20;
  if (comfortLevel == 3) eyelidHeight = 54; // Almost closed

  u8g2.firstPage();
  do {
    drawEyes(eyelidHeight, false);
  } while (u8g2.nextPage());
}

void drawEyes(int eyelidHeight, bool isBlinking) {
  const int leftEyeX = 32;
  const int rightEyeX = 96;
  const int eyeY = 64;
  const int eyeRadius = 30;
  const int eyeVerticalRadius = eyeRadius * 1.4; // Make eyes longer vertically
  const int lineWidth = 30;  // Width of the blink line
  const int lineThickness = 4;  // Number of horizontal lines to draw for thickness

  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 128, 128);

  // eyes
  u8g2.setDrawColor(1);

  if (isBlinking) {
    // Draw horizontal lines (multiple for thickness)
    for(int i = 0; i < lineThickness; i++) {
      // Left eye line
      u8g2.drawHLine(leftEyeX - lineWidth/2, eyeY + eyeVerticalRadius - lineThickness/2 + i, lineWidth);
      // Right eye line
      u8g2.drawHLine(rightEyeX - lineWidth/2, eyeY + eyeVerticalRadius - lineThickness/2 + i, lineWidth);
    }
  }

  else{
    // Draw left eye as a filled ellipse, longer vertically
    u8g2.drawFilledEllipse(leftEyeX, eyeY, eyeRadius, eyeVerticalRadius);
    // Draw right eye as a filled ellipse, longer vertically
    u8g2.drawFilledEllipse(rightEyeX, eyeY, eyeRadius, eyeVerticalRadius);

    // pupils
    u8g2.setDrawColor(0);
    // Draw left pupil slightly to the right of the center for cartoonish effect
    u8g2.drawDisc(leftEyeX + 8, eyeY, 8);
    // Draw right pupil slightly to the left of the center for cartoonish effect
    u8g2.drawDisc(rightEyeX - 8, eyeY, 8);

    // eyelids
    u8g2.setDrawColor(0);
    // Top of the eyelid is based on the vertical radius
    int eyeTop = eyeY - eyeVerticalRadius;
    u8g2.drawBox(0, eyeTop, 128, eyelidHeight);
  }
}


void displaySensorData(float temp, float humid, int airQuality) {
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_helvB12_tr);
    u8g2.setCursor(10, 15);
    u8g2.print("Temp: ");
    u8g2.setCursor(10, 35);
    u8g2.print(temp);
    u8g2.print(" C");

    u8g2.setCursor(10, 55);
    u8g2.print("Humidity: ");
    u8g2.setCursor(10, 75);
    u8g2.print(humid);
    u8g2.print(" %");

    String airQ = (airQuality == 0) ? "Clean air" : (airQuality == 1) ? "Low pollution" : (airQuality == 2) ? "High pollution" : "Extremely polluted";
    u8g2.setCursor(10, 95);
    u8g2.print("Air quality: ");
    u8g2.setCursor(10, 115);
    u8g2.print(airQ);
  } while (u8g2.nextPage());
}
