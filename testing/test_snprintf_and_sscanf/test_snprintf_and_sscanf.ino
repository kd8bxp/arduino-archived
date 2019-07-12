int packetnum = 200;
float vcc=5.743;
int uptime = 293;
#define callsign "KD8BXP"
#define comment "low power esp32"
char radiopacket[70];
#define beacon "beacon"
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
char msg[52];
sprintf(msg, "%s VCC=%d.%03d count=%d uptime=%ld",comment, (int) vcc, (int) (vcc*1000)%1000, packetnum, uptime);
sprintf(radiopacket,"%s %s %s", beacon,callsign,msg);

Serial.print("RadioPacket: ");
Serial.println(radiopacket);
String from;
String to;
String comment2;
sscanf(radiopacket, "%s %s %s", &to, &from, &comment2);
Serial.print("To: ");
Serial.println(to);
Serial.print("From: ");
Serial.println(from);
Serial.print("Comment: ");
Serial.println(comment2);
}

void loop() {
  // put your main code here, to run repeatedly:

}
