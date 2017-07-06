#include <Wire.h>
 
const byte  mcp_address=0x20; // I2C Address of MCP23017 Chip
const byte  GPIOA=0x12; // Register Address of Port A
//const byte  GPIOB=0x13; // Register Address of Port B
 
//const byte sequence[16] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b00000000, 0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b000001000}, 0b00000100, 0b00000010 };
const byte sequence[16] = {0b011011011,  0b11011011,  0b10110110,  0b01101101,  0b00001000,  0b00000100,  0b00000010, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b00000000};
//The setup function is called once at startup of the sketch
void setup()
{
    Wire.begin();
    Wire.beginTransmission(mcp_address);
    Wire.write((byte)0x00);
    Wire.write((byte)0x00); // set all of bank A to outputs
    Wire.write((byte)0x00); // set all of bank B to outputs
    Wire.endTransmission();
}
 
void loop()
{
    int pos = 0;
    for (pos=0; pos < 4; pos++)
    {
        Wire.beginTransmission(mcp_address);
        Wire.write(GPIOA);
        Wire.write((byte)sequence[pos]);  // value to send - all HIGH
        Wire.endTransmission();
        delay(600);
    }
}
