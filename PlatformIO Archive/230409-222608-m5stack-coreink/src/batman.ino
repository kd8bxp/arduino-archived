#include "src/M5CoreInk/M5CoreInk.h"

#include "xbm.h"

Ink_Sprite InkPageSprite(&M5.M5Ink);

void setup() {
    M5.begin();
    if( !M5.M5Ink.isInit())
    {
        Serial.printf("Ink Init faild");
        while (1) delay(100);   
    }
    M5.M5Ink.clear();

    if( InkPageSprite.creatSprite(0,0,200,200,true) != 0 )
    {
        Serial.printf("Ink Sprite creat faild");
    }
    InkPageSprite.drawBuff(0,0,200,124,(uint8_t*)logo);
    InkPageSprite.pushSprite();
    
    
}

void loop() {

}
