//#include "src/M5StickC/M5StickC.h"
 
#define RXD2  22 //32 //26 //32
#define TXD2  21 //33 //26
 

 
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
 // M5.begin(true, true, true);
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    //Serial2.listen();
}
 
void loop()
{
    char cmd;
 
    if(Serial2.available())
    {
        cmd = Serial2.read();
        Serial.println(voiceBuffer[cmd - 1]);
    }
}
