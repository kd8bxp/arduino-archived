/* Aug 15, 2017 (C) LeRoy Miller.
 *  Added the ability for the library to read and write to eeprom to store the
 *  motor pin numbers.
 *  RabbitRobot (int L_PIN, int L_PWM, int R_PIN, int R_PWM, int L_EN, int R_EN); 
RabbitRobot (int L_PIN, int L_PWM, int R_PIN, int R_PWM);
RabbitRobot (String prom); //READ settings from eeprom, CLEAR settings from eeprom
RabbitRobot(String prom, int L_PIN, int L_PWM, int R_PIN, int R_PWM); //UPDATE or WRITE 4 Wire PIN numbers to eeprom settings
RabbitRobot(String prom, int L_PIN, int L_PWM, int R_PIN, int R_PWM, int L_EN, int R_EN); //UPDATE or WRITE 6 WIRE PIN numbers to eeprom settings
 */
#include "RabbitRobot.h" //this is our custom library
UltraSonic ping(8, 9); 
//RabbitRobot bot(5,4,7,6);
RabbitRobot bot("READ");

void setup() {
Serial.begin(9600);
bot.begin();
ping.begin();
bot.forward(200, 1000);
while (bot.IsRunning()) {
  bot.update();
}

}

void loop() {
 ping.ping();
int avgDistance = ping.avgDistance();
 int avgCount = ping.avgCount();
 int targetDistance = ping.targetDistance();
int ping1 = (avgDistance/avgCount);  

Serial.print("Avg Distance: ");
Serial.println(avgDistance);
Serial.print("Avg Count: ");
Serial.println(avgCount);
Serial.print("targetDistance: ");
Serial.println(targetDistance);
Serial.print("Ping1: ");
Serial.println(ping1);

delay(5000);

}
