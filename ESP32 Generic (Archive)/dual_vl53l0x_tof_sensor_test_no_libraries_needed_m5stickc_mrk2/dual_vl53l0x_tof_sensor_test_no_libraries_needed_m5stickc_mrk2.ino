//  the original code by Ted Meyers
//  posted here: https://groups.google.com/d/msg/diyrovers/lc7NUZYuJOg/ICPrYNJGBgAJ

//  if your tof have some problem, please see https://docs.m5stack.com/#/en/unit/tof 

#include <Wire.h>

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define TOF_ADDR 0x29
#define PAHUB_ADDR 0X70

//Right Sensor connected to port 0
//Left Sensor connected to port 5

byte gbuf[16];



struct TOF{
  uint16_t acnt; //ambient count
  uint16_t scnt; //signal count
  uint16_t distance; //distance in mm
  byte internalStatus; //internal status
};

TOF TOFreadings;

uint16_t rightDist;
uint16_t leftDist;

void setup() {
        
  Wire.begin();
  Serial.begin(115200);  // start serial for output
  Serial.println("VLX53LOX test started.");
  
}

void loop() {
  portselect(0);
  Serial.print("Right: ");
  readTOF();
  rightDist = TOFreadings.distance;
  Serial.print(rightDist);
  portselect(5);
  Serial.println(); Serial.print("Left : ");
  readTOF();
  leftDist = TOFreadings.distance;
  Serial.print(leftDist);
  Serial.println("");
  delay(800);
}


void portselectall(uint8_t ports) {  
  Wire.beginTransmission(PAHUB_ADDR);
  Wire.write(ports&0x3f);
  Wire.endTransmission(); 
}


//Hub range is 0 to 5
void portselect(uint8_t i) {
  if (i > 7) return;
  
  Wire.beginTransmission(PAHUB_ADDR);
  Wire.write(1 << i);
  Wire.endTransmission(); 
}

uint16_t bswap(byte b[]) {
  // Big Endian unsigned short to little endian unsigned short
  uint16_t val = ((b[0] << 8) & b[1]);
  return val;
}

uint16_t makeuint16(int lsb, int msb) {
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}

void write_byte_data(byte data) {
  Wire.beginTransmission(TOF_ADDR);
  Wire.write(data);
  Wire.endTransmission();
}

void write_byte_data_at(byte reg, byte data) {
  // write data word at TOF_ADDR and register
  Wire.beginTransmission(TOF_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void write_word_data_at(byte reg, uint16_t data) {
  // write data word at TOF_ADDR and register
  byte b0 = (data &0xFF);
  byte b1 = ((data >> 8) && 0xFF);
    
  Wire.beginTransmission(TOF_ADDR);
  Wire.write(reg);
  Wire.write(b0);
  Wire.write(b1);
  Wire.endTransmission();
}

byte read_byte_data() {
  Wire.requestFrom(TOF_ADDR, 1);
  while (Wire.available() < 1) delay(1);
  byte b = Wire.read();
  return b;
}

byte read_byte_data_at(byte reg) {
  //write_byte_data((byte)0x00);
  write_byte_data(reg);
  Wire.requestFrom(TOF_ADDR, 1);
  while (Wire.available() < 1) delay(1);
  byte b = Wire.read();
  return b;
}

uint16_t read_word_data_at(byte reg) {
  write_byte_data(reg);
  Wire.requestFrom(TOF_ADDR, 2);
  while (Wire.available() < 2) delay(1);
  gbuf[0] = Wire.read();
  gbuf[1] = Wire.read();
  return bswap(gbuf); 
}

void read_block_data_at(byte reg, int sz) {
  int i = 0;
  write_byte_data(reg);
  Wire.requestFrom(TOF_ADDR, sz);
  for (i=0; i<sz; i++) {
    while (Wire.available() < 1) delay(1);
    gbuf[i] = Wire.read();
  }
}

uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg) {
  // Converts the encoded VCSEL period register value into the real
  // period in PLL clocks
  uint16_t vcsel_period_pclks = (vcsel_period_reg + 1) << 1;
  return vcsel_period_pclks;
}

void readTOF() {
   write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);

  byte val = 0;
  int cnt = 0;
  while (cnt < 100) { // 1 second waiting time max
    delay(10);
    val = read_byte_data_at(VL53L0X_REG_RESULT_RANGE_STATUS);
    if (val & 0x01) break;
    cnt++;
  }
  //if (val & 0x01) Serial.println("ready"); else Serial.println("not ready");

  read_block_data_at(0x14, 12);
  TOFreadings = {makeuint16(gbuf[7], gbuf[6]), makeuint16(gbuf[9], gbuf[8]), makeuint16(gbuf[11], gbuf[10]), ((gbuf[0] & 0x78) >> 3)};
   
}
