#ifndef LedBuffer_h
#define LedBuffer_h

//  0  1  2  3  4
//  5  6  7  8  9
// 10 11 12 13 14
// 15 16 17 18 19
// 20 21 22 23 24

#define LED_BUF_HEAD 0x2
#define LED_WIDTH    0x5
#define LED_HEIGHT   0x5
#define LED_MAX      (LED_WIDTH * LED_HEIGHT)
#define LED_RGB_SIZE 0x3

class LEDBuffer {
  public:
    void fillAll(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata);
    void fillAll(uint32_t RGBdata);
    void fill(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata, uint8_t start_pos, uint8_t end_pos);
    void fill(uint32_t RGBdata, uint8_t start_pos, uint8_t end_pos);
    void set(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata, uint8_t pos);
    void set(uint32_t RGBdata, uint8_t pos);
    void clear(void);
    void display(void);

  private:
    uint8_t DisBuff[2 + LED_WIDTH * LED_HEIGHT * LED_RGB_SIZE];
};

#endif // LedBuffer_h
