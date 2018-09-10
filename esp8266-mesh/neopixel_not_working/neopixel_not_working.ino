#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>
//#include <Adafruit_NeoPixel.h>

#define PIN   D4
#define LED_NUM 7


String manageRequest(String request);

/* Create the mesh node object */
ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);
//Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String request)
{
	/* Print out received message */
	Serial.println("received: ");
	//Serial.println(request);
 //led_set(10, 0, 0);//red
 //led_set(0, 0, 0);

 // led_set(0, 10, 0);//green
 //led_set(0, 0, 0);

 // led_set(0, 0, 10);//blue
 //led_set(0, 0, 0);
char response[60];
  sprintf(response, "Hello %d.", ESP.getChipId());
  mesh_node.attemptScan(request);

}

void setup()
{
	Serial.begin(9600);
	delay(10);
   
//leds.begin();
 //led_set(0, 0, 0);
	Serial.println();
	Serial.println();
	Serial.println("Setting up mesh node...");
delay(1000);
	/* Initialise the mesh node */
	mesh_node.begin();
   char request[60];
  sprintf(request, "Hello %d.", ESP.getChipId());
  mesh_node.attemptScan(request);
}

void loop()
{
	/* Accept any incoming connections */
	mesh_node.acceptRequest();
 
  yield();
delay(100);
	
}

void led_set(uint8 R, uint8 G, uint8 B) {
  for (int i = 0; i < LED_NUM; i++) {
//    leds.setPixelColor(i, leds.Color(R, G, B));
  //  leds.show();
    delay(20);
  }
}
