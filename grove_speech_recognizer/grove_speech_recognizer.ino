#include <SoftwareSerial.h>
 
#define SOFTSERIAL_RX_PIN  2
#define SOFTSERIAL_TX_PIN  3
 
SoftwareSerial softSerial(SOFTSERIAL_RX_PIN,SOFTSERIAL_TX_PIN);
 
const char *voiceBuffer[] =
{
    "Turn on the light",
    "Turn off the light",
    "Play music",
    "Pause",
    "Next",
    "Previous",
    "Up",
    "Down",
    "Turn on the TV",
    "Turn off the TV",
    "Increase temperature",
    "Decrease temperature",
    "What's the time",
    "Open the door",
    "Close the door",
    "Left",
    "Right",
    "Stop",
    "Start",
    "Mode 1",
    "Mode 2",
    "Go",
};
 
void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);
    softSerial.listen();
}
 
void loop()
{
    char cmd;
 
    if(softSerial.available())
    {
        cmd = softSerial.read();
        Serial.println(voiceBuffer[cmd - 1]);
    }
}
