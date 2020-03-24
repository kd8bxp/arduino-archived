int spinCounter = 100;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
for (int i=0; i<15; i++) {
spinCounter = spinCounter<<1;
Serial.println(spinCounter/100);
     if (spinCounter/100 == 0) { Serial.println(" LEFT");   }
     }

for (int i=0; i<11; i++) {

spinCounter = spinCounter>>1;
Serial.println(spinCounter/100);
         if (spinCounter/100 == 1024) {Serial.println(" RIGHT");  } //need a few more counts
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
