static byte bColorToByte[4] = {0, 0x49, 0x92, 0xff};

void reverse()
{int BUFFERSIZE = 4;
  for (int i=0, j = BUFFERSIZE-1; i< BUFFERSIZE/2; i++, j--)
  {
    int temp = bColorToByte[i];
    bColorToByte[i] = bColorToByte[j];
    bColorToByte[j] = temp;
  }
}


void setup() {
  delay(5000);
  Serial.begin(115200);
  Serial.print(bColorToByte[0]);
  Serial.print(" "); Serial.print(bColorToByte[1]); Serial.print(" ");
  Serial.print(bColorToByte[2]); Serial.print(" "); Serial.println(bColorToByte[3]);
  reverse();
  Serial.print(bColorToByte[0]);
  Serial.print(" "); Serial.print(bColorToByte[1]); Serial.print(" ");
  Serial.print(bColorToByte[2]); Serial.print(" "); Serial.println(bColorToByte[3]);
  reverse();
  Serial.print(bColorToByte[0]);
  Serial.print(" "); Serial.print(bColorToByte[1]); Serial.print(" ");
  Serial.print(bColorToByte[2]); Serial.print(" "); Serial.println(bColorToByte[3]);

}

void loop() {
  // put your main code here, to run repeatedly:

}
