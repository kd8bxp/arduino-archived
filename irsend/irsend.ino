/*
irSend sketch
this code needs an IR LED connected to pin 3
and 5 switches connected to pins 4 - 8
*/
#include <IRremote.h>
// IR remote control library
const int numberOfKeys = 1;
const int firstKey = 4;
// the first pin of the 5 sequential pins connected to buttons
boolean buttonState[numberOfKeys];
boolean lastButtonState[numberOfKeys];
long irKeyCodes[numberOfKeys] = {
    0x18E758A7, //0 key
};

IRsend irsend;
void setup()
{
    for (int i = 0; i < numberOfKeys; i++){
        buttonState[i]=true;
        lastButtonState[i]=true;
        int physicalPin=i + firstKey;
        pinMode(physicalPin, INPUT);
        digitalWrite(physicalPin, HIGH); // turn on pull-ups
    }
    Serial.begin(9600);
}
void loop() {
    for (int keyNumber=0; keyNumber<numberOfKeys; keyNumber++)
    {
        int physicalPinToRead=keyNumber+4;
        buttonState[keyNumber] = digitalRead(physicalPinToRead);
        if (buttonState[keyNumber] != lastButtonState[keyNumber])
        {
            if (buttonState[keyNumber] == LOW)
            {
                irsend.sendSony(irKeyCodes[keyNumber], 32);
                Serial.println("Sending");
            }
            lastButtonState[keyNumber] = buttonState[keyNumber];
        }
    }
}
