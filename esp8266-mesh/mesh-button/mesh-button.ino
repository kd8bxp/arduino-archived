#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>

#define BUTTON D3

//Debounce Stuff

int buttonState = LOW;

// the current and previous readings from the input pin
int thisButtonState = LOW;
int lastButtonState = LOW;

// time is measured in milliseconds and will quickly exceed limitations of an integer, so we use a long for these two
unsigned long lastDebounceTime = 0;  // the time the button state last switched
unsigned long debounceDelay = 50;    // the state must remain the same for this many millis to register the button press


String manageRequest(String request);

/* Create the mesh node object */
ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String request)
{
	/* Print out received message */
	Serial.print("received: ");
	Serial.println(request);

	
}

void setup()
{
	Serial.begin(9600);
	delay(10);
  pinMode(BUTTON, INPUT_PULLUP);

	Serial.println();
	Serial.println();
	Serial.println("Setting up mesh node...");

	/* Initialise the mesh node */
	mesh_node.begin();
}

void loop()
{

 thisButtonState = digitalRead(BUTTON);
  if (thisButtonState != lastButtonState) {
    // reset the timer
    lastDebounceTime = millis();
  }
   if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed
    if (thisButtonState != buttonState) {
      buttonState = thisButtonState;

      // only toggle the LED if the buttonState has switched from LOW to HIGH
      if (buttonState == HIGH) {
        mesh_node.attemptScan("1");
        Serial.println("Button.");
      }
    }
  }

  // persist for next loop iteration
  lastButtonState = thisButtonState;


  
	/* Accept any incoming connections */
	mesh_node.acceptRequest();

	/* Scan for other nodes and send them a message */
/*char request[60];
	sprintf(request, "Mesh_Node%d.", ESP.getChipId());
	mesh_node.attemptScan(request);
	//delay(1000);
  */
}
