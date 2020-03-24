void setup()
{
Serial3.begin(9600);   
Serial.begin(9600);   
delay(100);
Serial3.println("AT+DEFAULT");
}
void loop()
{
if (Serial.available()>0) Serial3.write(Serial.read());
if (Serial3.available()>0) Serial.write(Serial3.read());
}

