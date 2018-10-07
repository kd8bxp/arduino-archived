
int val11;
int val2;

void setup()
{

Serial.begin(9600);
Serial.println("Emartee.Com");
Serial.println("Voltage: ");
Serial.print("V");
}
void loop()
{
float temp;
val11=analogRead(1);
temp=(val11/4.092)/10;
val11=(int)temp * 10;//
val2=((val11%100)/10);
Serial.print("Raw Reading: ");
Serial.println(temp);
Serial.println("Corrected Reading: ");
Serial.println(val2);
delay(1000);
}