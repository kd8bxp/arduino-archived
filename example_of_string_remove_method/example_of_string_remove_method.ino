/*
  Example of the String remove() method

 Print on the serial monitor a full string, and then the string with a portion removed. 
 Both removal methods are demonstrated.

 The circuit:
 No external components needed.

 created 10 Nov 2014
 by Arturo Guadalupi

 This example code is in the public domain.
 */

String exString = "Hello World!";    // example string

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // send an intro:
  Serial.println("\n\nString remove() method example");
  Serial.println();
}

void loop() {
  // Print the initial string
  Serial.println("The full string:");
  Serial.println(exString);

  // Removing from an index through the end
  exString.remove(7);   // Remove from from index=7 through the end of the string
  Serial.println("String after removing from the seventh index through the end");
  Serial.println(exString);  // Should be just "Hello W"

  // Removing only a portion in the middle of a string
  exString = "Hello World!";
  exString.remove(2, 6); // Remove six characters starting at index=2
  Serial.println("String after removing six characters starting at the third position");
  Serial.println(exString); // Should be just "Herld!"

  Serial.println();
  Serial.println();

  while(1)
    ; // no need to do it again
}
