int adcPin = 33; // select the input pin for the potentiometer
int ledPin = 32; // select the pin for the LED
int adcIn = 0;   // variable to store the value coming from the sensor

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 12 bit precission for LEDC timer
#define LEDC_TIMER_12_BIT  12

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 

// Float for Reference Voltage
float ref_voltage = 5.0;

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void setup()
{
    Serial.begin(115200); // init serial to 9600b/s
    
    Serial.println("Sliding Potentiometer Test Code!!");
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttachPin(ledPin, LEDC_CHANNEL_0);
}
void loop()
{
    // read the value from the sensor:
    adcIn = analogRead(adcPin);
    //if(adcIn >= 500) digitalWrite(ledPin,HIGH);  // if adc in &gt; 500, led light
    //else digitalWrite(ledPin, LOW);
    brightness = map(adcIn, 0, 4095, 0, 255);
    ledcAnalogWrite(LEDC_CHANNEL_0, brightness);
    Serial.print("Brightness: "); Serial.print(brightness);
    Serial.print(" ADC In: ");
    Serial.print(adcIn);
    Serial.print(" Voltage: "); 
    adc_voltage = (adcIn * ref_voltage ) / 4095;
    in_voltage = adc_voltage ;
    Serial.print(in_voltage,2); Serial.println("V");
    delay(100);
}
