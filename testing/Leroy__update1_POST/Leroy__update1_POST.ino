/* RestClient simple GET request
 Current Program running on the Sump Pumb in the Basement
 */

#include "RestClient.h"

RestClient client = RestClient("homecontrol.internetcontrolsystems.com");
//char POSTDATA[]= "{\"status\":1, \"Category\":\"Water\", \"Duration\":95, \"SpecialCondition\": \"Inactive\", \"DeviceName\":\"WemosD1Test\"}";
// char LastSection[] =", \"SpecialCondition\": \"Inactive\"}";
//Setup
int sensorPin = A0;    // The input pin for sensor reading
int WaterHigh = D1;      // Used to turn on the pump through the SPDT Relay contacts
int On_Indicator = D3;  // ON signal for user (red led)
int WaterLow = D4;      // OFF indicator pump off
int sensorValue = 0;  // variable to store the value coming from the sensor
int waitInterval = 20000; // Pump interval and scan cycle - 20 seconds
int ReportCounter = 0; // Number of times before Web Update
int ReportNow = 45; // Number of timing loops before Web Update
int WaterHighDuration = 0; // Initialize Water High
int PumpRunTime = 20; // Normal Pump run-time
String response;
void setup() {
  pinMode(WaterHigh, OUTPUT);
  pinMode(WaterLow, OUTPUT);
  pinMode(On_Indicator, OUTPUT);
  Serial.begin(115200);
  Serial.println("connect to WiFi network");
  yield();
  client.begin("Motel6", "");
  Serial.println("Setup!");
  ESP.wdtDisable();
  delay(1000);
  WaterHighDuration = 45;
  yield();
}

void loop(){
  /*// Pump operation
  // read the input on analog pin 0:
  sensorValue = analogRead(sensorPin);
  
  response = "";
    // Print Analogue value to Serial Monitor
  //Serial.println(sensorValue);
  if(sensorValue > 750) {
      digitalWrite(WaterLow, HIGH);
      digitalWrite(WaterHigh, LOW);
      digitalWrite(On_Indicator,LOW);
   }
  else
   {
    digitalWrite(WaterHigh, HIGH);
    digitalWrite(On_Indicator, HIGH);
    digitalWrite(WaterLow, LOW);
    // Increment on-time counter
    WaterHighDuration += PumpRunTime;
  
  }
  ReportCounter++;
  //Test for report interval
  if(ReportCounter == ReportNow)
  {*/
    // Integer must be converted to string and then character array  
    //char VDATA[4] = {'\0'};
    //String str;
    //str = String(WaterHighDuration);
    //str.toCharArray(VDATA,4);
    char POSTDATA[75];
    //char POSTDATA[]= "{\"status\":1, \"Category\":\"Zpump\", \"Duration\":"; 
    //strcat(POSTDATA, VDATA); //Add the integer to the string
    // strcat(POSTDATA, LastSection);
    snprintf(POSTDATA,75,"{\"status\":1, \"Category\":\"Zpump\", \"Duration\":%d", WaterHighDuration);
    yield();
    Serial.print("POSTDATA: ");
    Serial.println(POSTDATA);
    int statusCode = client.post("/api/home", POSTDATA, &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    Serial.print("Response body from server: ");
    Serial.println(response);  
    ReportCounter = 0;
    WaterHighDuration = 0;
    Serial.println(POSTDATA);
  //}
  yield();
  delay(waitInterval);
  while(1) {
    yield();
    }
  }
 
