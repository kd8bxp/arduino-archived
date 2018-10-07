#include &lt;AcceleroMMA7361.h&gt;

AcceleroMMA7361 accelero;
int x;
int y;
int z;

void setup()
{
  Serial.begin(9600);
  accelero.begin(13, 12, 11, 10, A0, A1, A2);
  accelero.setARefVoltage(3.3);                   //sets the AREF voltage to 3.3V
  accelero.setSensitivity(LOW);                   //sets the sensitivity to +/-6G
  accelero.calibrate();
}

void loop()
{
  x = accelero.getXVolt();
  y = accelero.getYVolt();
  z = accelero.getZVolt();
  Serial.print(&quot;\nx: &quot;);
  Serial.print(x);
  Serial.print(&quot;mV\ty: &quot;);
  Serial.print(y);
  Serial.print(&quot;mV\tz: &quot;);
  Serial.print(z);
  Serial.print(&quot;mV&quot;);
  delay(500);                                     //make it readable
}
