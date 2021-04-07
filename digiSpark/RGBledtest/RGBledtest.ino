void setup() {
    // Initialize the LED pins as outputs
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
}
 
void loop() {
    digitalWrite(0, HIGH);  // Turn the red LED On
    delay(1000);            // Wait for a second
    digitalWrite(1, HIGH);  // Turn the green LED On
    delay(1000);            // Wait for a second
    digitalWrite(2, HIGH);  // Turn the blue LED On
    delay(1000);            // Wait for a second
    digitalWrite(0, LOW);   // Turn the red LED off
    digitalWrite(1, LOW);   // Turn the green LED off
    digitalWrite(2, LOW);   // Turn the blue LED off
    delay(1000);            // Wait for one second
}
