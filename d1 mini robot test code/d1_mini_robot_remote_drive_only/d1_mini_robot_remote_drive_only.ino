//v3.0.3 

#include "WEMOS_Motor.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager


Motor M1(0x30, _MOTOR_A, 1000); //Connected to A1 & A2 Right Motor
Motor M2(0x30, _MOTOR_B, 1000); //Connected to B1 & B2 Left Motor
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP Udp;

int lowSpeed = 60;
int maxSpeed = 100;

//Setup Encoder Pins 
//It appears the interrupt handler needs the GPIO numbers
#define M1A 13
#define M1B 12
#define M2A 14
#define M2B  0

//setup direction flag
//volatile int m1dir = 0;
//volatile int m2dir = 0;
//long m1steps, m2steps;

int isRunning  = 0; //Motor Running flag.

//We need some timers. 2 unsigned long timers
//unsigned long motorTimer = 0; //Motor Run timer
//unsigned long motorCurrentTimer = 0;
//long motorStopTime; //This is the check time
//unsigned long encoderTimer = 0; //Motor Master Encoder Counter
//unsigned long encoderCurrentTimer = 0;

//int rotationCount = 360; //one rotation
int setSpeed = 100; //PWM for motor
//int target; //how many times to roate wheel
//int targetCount = 0;
//int targetCountM1 = 0;
//int targetCountM2 = 0;

unsigned int localPort = 8888;      // local UDP port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "ack";  // a string to send back (I think this is a hold over from the library example and not used here)

const char* ssid = "Motel6";
const char* password = "";
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_server = "test.mosquitto.org";

uint32_t chipid=ESP.getChipId(); //attempt to make a unique ID for MQTT Broker 
char clientid[16]; //unique ID for MQTT Broker - also used as the Publish Topic

//volatile long RangeInInches;
//int ultrasonicPin = 16;

int mode = 0; //Default mode is UDP remote control
int cmd = 0; //stop command

//int snap2;
//int snap3;
//int diff;
//long duration;// the Pulse time received;
char tempArr[200];
String ipaddress;
char ipchar[16];

