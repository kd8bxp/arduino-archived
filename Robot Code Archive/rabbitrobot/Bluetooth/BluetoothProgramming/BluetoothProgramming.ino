#include <SoftwareSerial.h>
#define bt_power 7
#define bt_wakeup_power 8
#define led_13 13

SoftwareSerial BT(A0, A1); // RX | TX

void setup()
{
  // set the pins to OUTPUT
  pinMode(bt_power, OUTPUT);  
  pinMode(bt_wakeup_power, OUTPUT);
  pinMode(led_13, OUTPUT);
  
  // set the pins to LOW
  digitalWrite(bt_power, LOW);
  digitalWrite(bt_wakeup_power, LOW);
  digitalWrite(led_13, LOW);

  delay(100);
  digitalWrite(bt_wakeup_power, HIGH);
  delay(100);
  digitalWrite(bt_power, HIGH);

  Serial.begin(9600);
  BT.begin(38400);

  Serial.write("For a list of commands, visit: \n");
  Serial.write("Type AT commands  \n\n");

  digitalWrite(led_13, HIGH);
 }

void loop()
{
  if (BT.available())
    Serial.write(BT.read());
  if (Serial.available())
    BT.write(Serial.read());
}
