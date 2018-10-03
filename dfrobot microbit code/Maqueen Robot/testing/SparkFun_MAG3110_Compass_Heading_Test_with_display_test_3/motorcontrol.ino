void stop() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
}

void forward(int spd) {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write(spd); //(byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write(spd); //(byte)0x50); //speed
  Wire.endTransmission();
}

void backward() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x01);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x01);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void spinleft() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x01);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void spinright() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x80); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x01);
  Wire.write((byte)0x80); //speed
  Wire.endTransmission();
}

void left() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void right() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
}




