#include <NewPing.h>

int pingSensorIn;
int triggerDistance = 5; //Your Too Close distance in Inches
int triggerDistance2 = 10; //Getting closer distance (Caution!) in Inches
int triggerDistance3 = 30; //No used 

#define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int redLED = 14;
int greenLED = 15;
int blueLED = 16;

void setup () {
  Serial.begin(9600);

pinMode(redLED, OUTPUT);
pinMode(greenLED, OUTPUT);
pinMode(blueLED, OUTPUT);

digitalWrite (redLED, HIGH);
digitalWrite (greenLED, HIGH);
digitalWrite (blueLED, HIGH);
 
}

void loop () {

               
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
 /* Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_IN); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println(" in");
  */
  pingSensorIn = (uS / US_ROUNDTRIP_IN);
      
      if (pingSensorIn <= triggerDistance)  
      {
      	//To Close - RED Stop!
        digitalWrite(redLED, LOW);
		digitalWrite(greenLED, HIGH);
		digitalWrite(blueLED, HIGH);
		Serial.print("RED: ");
		Serial.print(pingSensorIn);
		Serial.println(" in.");
      }
       if (pingSensorIn > triggerDistance && pingSensorIn < triggerDistance2) 
      {
      	//Almost Close - YELLOW Caution!
        digitalWrite(greenLED, LOW);
		digitalWrite(redLED, LOW);
		digitalWrite(blueLED, HIGH);
		Serial.print("Yelow: ");
		Serial.print(pingSensorIn);
		Serial.println(" in.");
      }
      
		if (pingSensorIn >= triggerDistance2)
		{
		//Clear - GREEN stuff is pretty far from you!	
		digitalWrite(blueLED, HIGH);
		digitalWrite(redLED, HIGH);
		digitalWrite(greenLED, LOW);
		Serial.print("Green: ");
		Serial.print(pingSensorIn);
		Serial.println(" in.");
		}

}

