#include "WEMOS_Motor.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include "Ultrasonic.h"
#include <ArduinoJson.h>

Motor M1(0x30, _MOTOR_A, 1000); //Connected to A1 & A2 Right Motor
Motor M2(0x30, _MOTOR_B, 1000); //Connected to B1 & B2 Left Motor
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP Udp;
Ultrasonic ultrasonic(D0); //Using D0 on the D1 Mini Board

int lowSpeed = 60;
int maxSpeed = 100;

//Setup Encoder Pins
const int M1A = 13;
const int M1B = 12;
const int M2A = 14;
const int M2B = 0;

//setup direction flag
volatile int m1dir = 0;
volatile int m2dir = 0;
long m1steps, m2steps;

//setup counters
volatile unsigned long m1count, m2count;

int isRunning  = 0; //Motor Running flag.

//We need some timers. 2 unsigned long timers
unsigned long motorTimer = 0; //Motor Run timer
unsigned long motorCurrentTimer = 0;
long motorStopTime; //This is the check time
unsigned long encoderTimer = 0; //Motor Master Encoder Counter
unsigned long encoderCurrentTimer = 0;

int rotationCount = 360; //one rotation
int setSpeed = 100; //PWM for motor
int target; //how many times to roate wheel
int targetCount = 0;
int targetCountM1 = 0;
int targetCountM2 = 0;

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "ack";  // a string to send back

const char* ssid = "Motel6";
const char* password = "";
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_server = "test.mosquitto.org";

long lastMsg = 0;
char msg[50];
int value = 0;
 
    uint32_t chipid=ESP.getChipId(); 
    char clientid[16];

long RangeInInches;

int mode = 0;
int cmd = 0; //stop command

unsigned long udpTimer = 0;
unsigned long udpCurrentTimer = 0;


