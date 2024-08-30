//view  <robonii.h> for custom board defines

/*
  This code prints the Aschi table to the serial port, before sending a count every second.
  The Serial port is also polled for bytes received.  A value between '1' and '5' will turn on 
  the corresponding LED.
 */
int inByte = 0;         // incoming serial byte


void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(D3_Green, OUTPUT);     
  pinMode(D2_Green, OUTPUT);  
    pinMode(D1_Green, OUTPUT);  

  //pinMode(D1_Green, OUTPUT);  
 // PORTK.DIRSET = 0x01; //TIAN
  analogWrite(10, 100);
  

    digitalWrite(D1_Green, LOW);   // set the LED on
   Serial.begin(115200);   
   Serial.println("  ");  
   Serial.println("  ");  
Serial.println("-----------  Serial Started at 115200 BAUD: -----------");  
  
  PrintAschi();
  digitalWrite(D1_Green, HIGH);   // set the LED off
  Serial.println(" ===============================================  ");
  PrintCommands();
}

int TXdata = 47;
int dumByte = 0;


void loop() {
  digitalWrite(D2_Green, HIGH);   // set the LED off
  

     
    /* Put data into buffer, sends the data */
    Serial.print(TXdata++);
    
    //Make sure data stays within visible chars
    if (TXdata > 90) 
    {
      TXdata = 47;
    }
    
    if (Serial.available() > 0)
    {
      Serial.println(" ");
      Serial.print("ByteRead: ");
      
       while (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
   //Print the data received as decimal values
    Serial.print(inByte);
    Serial.print(" ");
    Serial.println(" ");
    //Check if byte between 1 and 5 to switch on LED's
    if ((inByte <= '5')&&(inByte >= '1'))
    {
      dumByte = inByte - '1';
      if ((dumByte>=0)&&(dumByte<5));
      pinMode((D1_Red+dumByte*2), OUTPUT);
      digitalWrite((D1_Red+dumByte*2), LOW);   // set the LED on
      
      
    }
     }
     //Put it on its own line
     Serial.println(" ");
    }
    
    
    
   // Serial.println(" ");
    
  // wait for a second
  digitalWrite(D2_Green, LOW);    // set the LED on
  digitalWrite(D1_Green, LOW);    // set the LED off
  
  analogWrite(11, 10);
  delay(1000);       
  // wait for a second
  
  //Serial.begin(9600); 
 // while(1);
}

void PrintAschi()
{
  int thisByte = 33; 
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!';  

while (thisByte < 127)
{
  // prints value unaltered, i.e. the raw binary version of the 
  // byte. The serial monitor interprets all bytes as 
  // ASCII, so 33, the first number,  will show up as '!' 
  Serial.write(thisByte);    

  Serial.print(", dec: "); 
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);      
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:

  // Serial.print(thisByte, DEC);  


  Serial.print(", hex: "); 
  // prints value as string in hexadecimal (base 16):
  Serial.print(thisByte, HEX);     

  Serial.print(", oct: "); 
  // prints value as string in octal (base 8);
  Serial.print(thisByte, OCT);     

  Serial.print(", bin: "); 
  // prints value as string in binary (base 2) 
  // also prints ending line break:
  Serial.println(thisByte, BIN);   

 
  // go on to the next character
  thisByte++;  
} 
  
}


void PrintCommands()
{
  Serial.println("Type a value between 1 and 5 to switch on the corresponding Red LED's");   
}


