#include "RabbitRobot.h" //this is our custom library
UltraSonic ping(8, 9); 
void setup() {
Serial.begin(9600);
ping.begin();

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
