#include "Arduino.h"
#include "src/M5Atom/M5Atom.h"
#include "LEDBuffer.h"

#define GET_R(x) ((uint8_t)((0x00ff0000 & RGBdata) >> 16))
#define GET_G(x) ((uint8_t)((0x0000ff00 & RGBdata) >> 8))
#define GET_B(x) ((uint8_t)(0x000000ff & RGBdata))

void LEDBuffer::fillAll(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = LED_WIDTH; // width
    DisBuff[1] = LED_HEIGHT; // height
    for (int i = 0; i < 25; i++) {
        this->set(Rdata, Gdata, Bdata, i);
    }
}

void LEDBuffer::fillAll(uint32_t RGBdata)
{
    this->fillAll(GET_R(RGBdata), GET_G(RGBdata), GET_B(RGBdata));
}

void LEDBuffer::fill(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata, uint8_t start_pos, uint8_t end_pos)
{
    DisBuff[0] = LED_WIDTH;
    DisBuff[1] = LED_HEIGHT;
    for (int i = start_pos; i < end_pos; i++) {
        this->set(Rdata, Gdata, Bdata, i);
    }
}

void LEDBuffer::fill(uint32_t RGBdata, uint8_t start_pos, uint8_t end_pos)
{
    this->fill(GET_R(RGBdata), GET_G(RGBdata), GET_B(RGBdata), start_pos, end_pos);
}

void LEDBuffer::set(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata, uint8_t pos)
{
    DisBuff[LED_BUF_HEAD + pos*LED_RGB_SIZE]   = Rdata;
    DisBuff[LED_BUF_HEAD + pos*LED_RGB_SIZE+1] = Gdata;
    DisBuff[LED_BUF_HEAD + pos*LED_RGB_SIZE+2] = Bdata;
}

void LEDBuffer::set(uint32_t RGBdata, uint8_t pos)
{
    this->set(GET_R(RGBdata), GET_G(RGBdata), GET_B(RGBdata), pos);
}


void LEDBuffer::clear(void)
{
    DisBuff[0] = LED_WIDTH;
    DisBuff[1] = LED_HEIGHT;
    for (int i = 0; i < LED_MAX; i++) {
        this->set(0, 0, 0, i);
    }
}

void LEDBuffer::display(void)
{
    M5.dis.displaybuff(DisBuff);
}
