//  the original code by Ted Meyers
//  posted here: https://groups.google.com/d/msg/diyrovers/lc7NUZYuJOg/ICPrYNJGBgAJ

//  if your tof have some problem, please see https://docs.m5stack.com/#/en/unit/tof 
// June 10, 2022 slightly modified to use two sensors on Wire & Wire1 buses (LeRoy Miller, KD8BXP)

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define address 0x29

void readRight() {
//Read Sensor A 
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
  //acnt = makeuint16(gbuf[7], gbuf[6]);
  //scnt = makeuint16(gbuf[9], gbuf[8]);
  dist = makeuint16(gbuf[11], gbuf[10]);
  //DeviceRangeStatusInternal = ((gbuf[0] & 0x78) >> 3);

  //Serial.println("Right: ");
  //Serial.print("ambient count: "); Serial.println(acnt);
  //Serial.print("signal count : "); Serial.println(scnt);
  //Serial.print("distance     : "); Serial.println(dist);
  //Serial.print("status       : "); Serial.println(DeviceRangeStatusInternal);
  
  
  
}

void readLeft() {
//Read Sensor B
  write1_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);

  byte val = 0;
  int cnt = 0;
  while (cnt < 100) { // 1 second waiting time max
    delay(10);
    val = read1_byte_data_at(VL53L0X_REG_RESULT_RANGE_STATUS);
    if (val & 0x01) break;
    cnt++;
  }
  //if (val & 0x01) Serial.println("ready"); else Serial.println("not ready");

  read1_block_data_at(0x14, 12);
  //acnt = makeuint16(gbuf[7], gbuf[6]);
  //scnt = makeuint16(gbuf[9], gbuf[8]);
  dist = makeuint16(gbuf[11], gbuf[10]);
  //DeviceRangeStatusInternal = ((gbuf[0] & 0x78) >> 3);

  //Serial.println("Left: ");
  //Serial.print("ambient count: "); Serial.println(acnt);
  //Serial.print("signal count : "); Serial.println(scnt);
  //Serial.print("distance     : "); Serial.println(dist);
  //Serial.print("status       : "); Serial.println(DeviceRangeStatusInternal);
  
  
  
}


uint16_t bswap(byte b[]) {
  // Big Endian unsigned short to little endian unsigned short
  uint16_t val = ((b[0] << 8) & b[1]);
  return val;
}

uint16_t makeuint16(int lsb, int msb) {
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}


// Lot of redundant code here, this is because I choose to use two I2C buses and not change the addresses of the sensors

void write_byte_data(byte data) {
  Wire.beginTransmission(address);
  Wire.write(data);
  Wire.endTransmission();
}

void write1_byte_data(byte data) {
  Wire1.beginTransmission(address);
  Wire1.write(data);
  Wire1.endTransmission();
}

void write_byte_data_at(byte reg, byte data) {
  // write data word at address and register
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void write1_byte_data_at(byte reg, byte data) {
  // write data word at address and register
  Wire1.beginTransmission(address);
  Wire1.write(reg);
  Wire1.write(data);
  Wire1.endTransmission();
}

void write_word_data_at(byte reg, uint16_t data) {
  // write data word at address and register
  byte b0 = (data &0xFF);
  byte b1 = ((data >> 8) && 0xFF);
    
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(b0);
  Wire.write(b1);
  Wire.endTransmission();
}

void write1_word_data_at(byte reg, uint16_t data) {
  // write data word at address and register
  byte b0 = (data &0xFF);
  byte b1 = ((data >> 8) && 0xFF);
    
  Wire1.beginTransmission(address);
  Wire1.write(reg);
  Wire1.write(b0);
  Wire1.write(b1);
  Wire1.endTransmission();
}

byte read_byte_data() {
  Wire.requestFrom(address, 1);
  while (Wire.available() < 1) delay(1);
  byte b = Wire.read();
  return b;
}

byte read1_byte_data() {
  Wire1.requestFrom(address, 1);
  while (Wire1.available() < 1) delay(1);
  byte b = Wire1.read();
  return b;
}

byte read_byte_data_at(byte reg) {
  //write_byte_data((byte)0x00);
  write_byte_data(reg);
  Wire.requestFrom(address, 1);
  while (Wire.available() < 1) delay(1);
  byte b = Wire.read();
  return b;
}

byte read1_byte_data_at(byte reg) {
  //write_byte_data((byte)0x00);
  write1_byte_data(reg);
  Wire1.requestFrom(address, 1);
  while (Wire1.available() < 1) delay(1);
  byte b = Wire1.read();
  return b;
}

uint16_t read_word_data_at(byte reg) {
  write_byte_data(reg);
  Wire.requestFrom(address, 2);
  while (Wire.available() < 2) delay(1);
  gbuf[0] = Wire.read();
  gbuf[1] = Wire.read();
  return bswap(gbuf); 
}

uint16_t read1_word_data_at(byte reg) {
  write1_byte_data(reg);
  Wire1.requestFrom(address, 2);
  while (Wire1.available() < 2) delay(1);
  gbuf[0] = Wire1.read();
  gbuf[1] = Wire1.read();
  return bswap(gbuf); 
}


void read_block_data_at(byte reg, int sz) {
  int i = 0;
  write_byte_data(reg);
  Wire.requestFrom(address, sz);
  for (i=0; i<sz; i++) {
    while (Wire.available() < 1) delay(1);
    gbuf[i] = Wire.read();
  }
}

void read1_block_data_at(byte reg, int sz) {
  int i = 0;
  write1_byte_data(reg);
  Wire1.requestFrom(address, sz);
  for (i=0; i<sz; i++) {
    while (Wire1.available() < 1) delay(1);
    gbuf[i] = Wire1.read();
  }
}


uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg) {
  // Converts the encoded VCSEL period register value into the real
  // period in PLL clocks
  uint16_t vcsel_period_pclks = (vcsel_period_reg + 1) << 1;
  return vcsel_period_pclks;
}