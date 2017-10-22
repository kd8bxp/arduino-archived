/*
 Interrupt D7, D6, D5, D3

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
int target = 10; //how many times to roate wheel
int targetCount = 0;
int targetCountM1 = 0;
int targetCountM2 = 0;

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

const char* ssid = "Motel6";
const char* password = "";
const char* mqtt_server = "broker.mqtt-dashboard.com";


long lastMsg = 0;
char msg[50];
int value = 0;
 
    uint32_t chipid=ESP.getChipId(); 
    char clientid[16];
long RangeInInches;

*/
