//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "src/Painless_Mesh/painlessMesh.h"
#include <FastLED.h>

#define PIN   14 //27
#define LED_NUM 64 //25
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

FASTLED_USING_NAMESPACE
CRGB leds[LED_NUM];

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
 led_set(100, 0, 0);//red
 led_set(0, 0, 0);

 led_set(0, 100, 0);//green
 led_set(0, 0, 0);

 led_set(0, 0, 100);//blue
 led_set(0, 0, 0);
 yield();
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting....");
FastLED.addLeds<WS2812,PIN,GRB>(leds, LED_NUM).setCorrection(TypicalLEDStrip);
led_set(0,0,0);
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
Serial.println("Loop.....");
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();
  yield();
}

void led_set(int R, int G, int B) {
  CRGB color = CRGB(R,G,B);
  for (int i = 0; i < LED_NUM; i++) {
    leds[i] = color;
  
   // delay(20);
  }
  FastLED.show();
   delay(50);
}
