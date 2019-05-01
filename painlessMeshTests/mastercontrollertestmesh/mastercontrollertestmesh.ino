//************************************************************
// this is a simple example that uses the painlessMesh library
// 
// This example shows how to build a mesh with named nodes
//
//************************************************************
#include "namedMesh.h"

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
#define APIN D3 //button A on OLED
#define RELAYNODE 1

byte relayState = LOW;
 String to = "relayNode1"; // send message to this node
 
Scheduler     userScheduler; // to control your personal task
namedMesh  mesh;

String nodeName = "mastercontroller"; // Name needs to be unique

Task taskSendMessage( TASK_SECOND*30, TASK_FOREVER, []() {
    String msg = String("This is a message from: ") + nodeName + String(" for logNode");
    String to1 = "relayNode1";
    mesh.sendSingle(to1, msg); 
}); // start with a one second interval

void setup() {
  Serial.begin(9600);
pinMode(APIN, INPUT_PULLUP); //A
  attachInterrupt(digitalPinToInterrupt(APIN), displayA, HIGH);
  
  mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);  // set before init() so that you can see startup messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);

  mesh.setName(nodeName); // This needs to be an unique name! 

  mesh.onReceive([](uint32_t from, String &msg) {
    Serial.printf("Received message by id from: %u, %s\n", from, msg.c_str());
  });

  mesh.onReceive([](String &from, String &msg) {
    Serial.printf("Received message by name from: %s, %s\n", from.c_str(), msg.c_str());
  });

  mesh.onChangedConnections([]() {
    Serial.printf("Changed connection\n");
  });

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();
}

void displayA() {
  relayState = !relayState;
  String msg = (String)relayState;
  mesh.sendSingle(to, msg); 
}
