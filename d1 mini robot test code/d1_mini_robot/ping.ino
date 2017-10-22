void ping() {

pingcount ++;
  pinMode(ultrasonicPin, OUTPUT);
  digitalWrite(ultrasonicPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ultrasonicPin,LOW);
  pinMode(ultrasonicPin,INPUT);
  duration = pulseIn(ultrasonicPin,HIGH);
  RangeInInches = duration/74/2;

pingavg = pingavg + (RangeInInches);
checkSonar(); //protection from hitting walls or objects.
 
}

void checkSonar() {
volatile unsigned int ping1 = (pingavg/pingcount);

if (ping1 <= STOPDISTANCE && cmd == 1) {
  stop();
  cmd = 0;
}
  
   if (pingcount >= 10) { //Reset Count
    pingavg = 0;
    pingcount = 0;
    Udp.flush();
   return;
}

}
