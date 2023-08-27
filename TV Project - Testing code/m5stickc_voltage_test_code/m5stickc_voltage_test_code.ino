// Define analog input
#define ANALOG_IN_PIN 0 //A0
 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 3.3; //5.0;
 
// Integer for ADC value
int adc_value = 0;
 
void setup(){
  // Setup Serial Monitor
  Serial.begin(9600);
  analogReadResolution(10);
}
 
void loop(){
  // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);
  
  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 1024.0;
  
  // Calculate voltage at divider input
  in_voltage = adc_voltage*(R1+R2)/R2;
  //float temp = map(in_voltage , 0.0, 1024.0, 0.0,80.0); //175
  // Print results to Serial Monitor to 2 decimal places
  //Serial.print("Input Voltage = ");
  Serial.println(in_voltage * 2.0, 2);
  //Serial.println(temp);
  
  // Short delay
  //delay(500);
}
