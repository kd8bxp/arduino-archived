/*  Serial Reading Examples  (Evil_Chemist - 01/04/15)

Three ways to read the serial buffer:
  => Byte by byte w/o storing data
  => Byte by byte stored into a variable
  => Read to variable as a block looking for terminator, size or timeout
*/

char serByte;
char serBuffer[128];
char serInString[128];
int serIndex=0;


void setup() {
  Serial.begin(9600);
  Serial.println("Let's go!!\n");
  }

void loop () {
  
  // Check for bytes in the serial buffer
  
  if(Serial.available()) {    

    // Clear the buffers before reading the new data
    
    memset(serBuffer, 0, sizeof(serBuffer));
    memset(serInString, 0, sizeof(serInString));

    /*  Serial.print(Serial.available());
        Serial.println(" bytes available.");
    */

    Serial.println("\nYou said: ");

    //  Read buffer and print byte by byte until it's empty
    	/*  while (Serial.available()>0){
		      serByte = Serial.read();	
    	      Serial.print(serByte); 	
    	      }
		 */
 	
 	// Read the serial buffer byte by byte into a variable 
    	/*   while (Serial.available()){ 
    	       serByte = Serial.read();             
               serInString[serIndex] = serByte;
               serIndex++;
		       }

			Serial.write(serInString);  
			Serial.print("\n\n");
			serIndex=0;
       */	

	// Read buffer in as a block; Use # as terminator
		Serial.readBytesUntil('#',serBuffer, 128);
     	Serial.println(serBuffer);
    	Serial.println();

  }
  delay(1000);
}