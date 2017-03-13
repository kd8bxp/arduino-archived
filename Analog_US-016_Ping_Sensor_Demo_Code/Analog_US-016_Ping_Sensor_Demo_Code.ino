void setup (){
    pinMode(A0, INPUT);
    Serial.begin (9600);
}

void loop (){
  
    int ADCValue = analogRead (0);
    ADCValue *= 3;
    Serial.print ( "Present Length is:");
    Serial.print (ADCValue, DEC);
    Serial.print ( "mm  ");
    Serial.print((ADCValue / 304.8), DEC);
    Serial.println(" ft");
    delay (500); // delay 1S
}
