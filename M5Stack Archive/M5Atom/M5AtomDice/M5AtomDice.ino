#include "src/M5Atom/M5Atom.h"

// M5ATOM Dice Display
//
//

extern const unsigned char AtomImageData[375 + 2];
uint8_t DisBuff[2 + 5 * 5 * 3];

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)   {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}
/*
 * no12 - center
 */
void setDice(char num)
{
    setBuff(0,0,0);   //clear
    
    switch (num) {
      case 1:
        DisBuff[2 + 12 * 3 + 0] = 0xff;
        DisBuff[2 + 12 * 3 + 1] = 0x0;
        DisBuff[2 + 12 * 3 + 2] = 0x0;
        break;
      case 2:
        DisBuff[2 + 4 * 3 + 0] = 0xff;
        DisBuff[2 + 4 * 3 + 1] = 0xff;
        DisBuff[2 + 4 * 3 + 2] = 0xff;
        DisBuff[2 +20 * 3 + 0] = 0xff;
        DisBuff[2 +20 * 3 + 1] = 0xff;
        DisBuff[2 +20 * 3 + 2] = 0xff;
        break;        
      case 3:
        DisBuff[2 + 4 * 3 + 0] = 0xff;
        DisBuff[2 + 4 * 3 + 1] = 0xff;
        DisBuff[2 + 4 * 3 + 2] = 0xff;
        DisBuff[2 +12 * 3 + 0] = 0xff;
        DisBuff[2 +12 * 3 + 1] = 0xff;
        DisBuff[2 +12 * 3 + 2] = 0xff;        
        DisBuff[2 +20 * 3 + 0] = 0xff;
        DisBuff[2 +20 * 3 + 1] = 0xff;
        DisBuff[2 +20 * 3 + 2] = 0xff;
        break;
      case 4:
        DisBuff[2 + 4 * 3 + 0] = 0xff;
        DisBuff[2 + 4 * 3 + 1] = 0xff;
        DisBuff[2 + 4 * 3 + 2] = 0xff;
        DisBuff[2 + 20 * 3 + 0] = 0xff;
        DisBuff[2 + 20 * 3 + 1] = 0xff;
        DisBuff[2 + 20 * 3 + 2] = 0xff;      
        DisBuff[2 + 0 * 3 + 0] = 0xff;
        DisBuff[2 + 0 * 3 + 1] = 0xff;
        DisBuff[2 + 0 * 3 + 2] = 0xff;
        DisBuff[2 + 24 * 3 + 0] = 0xff;
        DisBuff[2 + 24 * 3 + 1] = 0xff;
        DisBuff[2 + 24 * 3 + 2] = 0xff;      
        break;      
      case 5: 
        DisBuff[2 + 4 * 3 + 0] = 0xff;
        DisBuff[2 + 4 * 3 + 1] = 0xff;
        DisBuff[2 + 4 * 3 + 2] = 0xff;
        DisBuff[2 + 20 * 3 + 0] = 0xff;
        DisBuff[2 + 20 * 3 + 1] = 0xff;
        DisBuff[2 + 20 * 3 + 2] = 0xff;      
        DisBuff[2 + 0 * 3 + 0] = 0xff;
        DisBuff[2 + 0 * 3 + 1] = 0xff;
        DisBuff[2 + 0 * 3 + 2] = 0xff;
        DisBuff[2 + 24 * 3 + 0] = 0xff;
        DisBuff[2 + 24 * 3 + 1] = 0xff;
        DisBuff[2 + 24 * 3 + 2] = 0xff;      
        DisBuff[2 +12 * 3 + 0] = 0xff;
        DisBuff[2 +12 * 3 + 1] = 0xff;
        DisBuff[2 +12 * 3 + 2] = 0xff;        
      
        break;            
      case 6:
        DisBuff[2 + 0 * 3 + 0] = 0xff;
        DisBuff[2 + 0 * 3 + 1] = 0xff;
        DisBuff[2 + 0 * 3 + 2] = 0xff;
        DisBuff[2 + 2 * 3 + 0] = 0xff;
        DisBuff[2 + 2 * 3 + 1] = 0xff;
        DisBuff[2 + 2 * 3 + 2] = 0xff;      
        DisBuff[2 + 4 * 3 + 0] = 0xff;
        DisBuff[2 + 4 * 3 + 1] = 0xff;
        DisBuff[2 + 4 * 3 + 2] = 0xff;            
        DisBuff[2 + 20 * 3 + 0] = 0xff;
        DisBuff[2 + 20 * 3 + 1] = 0xff;
        DisBuff[2 + 20 * 3 + 2] = 0xff;
        DisBuff[2 + 22 * 3 + 0] = 0xff;
        DisBuff[2 + 22 * 3 + 1] = 0xff;
        DisBuff[2 + 22 * 3 + 2] = 0xff;      
        DisBuff[2 + 24 * 3 + 0] = 0xff;
        DisBuff[2 + 24 * 3 + 1] = 0xff;
        DisBuff[2 + 24 * 3 + 2] = 0xff;            
        
        break;      
    }
    
  
}


void setup()
{
    M5.begin(true, false, true);
    delay(10);
    setBuff(0x0, 0x00, 0x00);
    M5.dis.displaybuff(DisBuff);
    //Serial.begin(115200);
}

uint8_t num = 1;

void loop()
{
    if (M5.Btn.wasPressed())
    {
        setDice(num);
        M5.dis.displaybuff(DisBuff);
        //Serial.println(num);

        num++;
        if (num >= 7) {
            num = 1;
        }
    }

    delay(50);
    M5.update();
}
