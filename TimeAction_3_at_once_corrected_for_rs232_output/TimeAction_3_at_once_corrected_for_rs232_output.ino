    /*
    ||
    || @file ThreeExamplesAtOnce.pde
    || @version 1.0
    || @author Alexander Brevig
    || @contact alexanderbrevig@gmail.com
    ||
    || @description
    || | This sketch blinks an LED as Blink
    || |             sets a led on or off according to serial buffer as PhysicalPixel
    || |             prints the ascii table as ASCIITable
    || #
    ||
    */
     
    #include <TimedAction.h>
     
    //this initializes a TimedAction object that will change the state of an LED every second.
    TimedAction blinkAction                 =       TimedAction(150,blink);
    //this initializes a TimedAction object that will change the state of an LED
    //according to the serial buffer contents, every 50 milliseconds
    TimedAction physicalPixelAction =       TimedAction(1000,physicalPixel);
    //this initializes a TimedAction object that will write tha ascii table to the serial every ten seconds
    TimedAction asciiTableAction    =       TimedAction(10000,asciiTable);
    TimedAction blink11action = TimedAction(500, blink11);
    TimedAction blink10action = TimedAction(250, blink10);
    TimedAction blink9action = TimedAction(100, blink9);
    TimedAction blink8action = TimedAction(750, blink8);
    
    //pin / state variables
    #define ledPin 13
    #define physicalPin 12
    boolean ledState = false;
    boolean led12state = false;
    boolean led11state = false;
    boolean led10state = false;
    boolean led9state = false;
    boolean led8state = false;
     
    void setup(){
      pinMode(ledPin,OUTPUT);
      pinMode(physicalPin, OUTPUT);
      pinMode(11, OUTPUT);
      pinMode(10, OUTPUT);
      pinMode(9, OUTPUT);
      pinMode(8, OUTPUT);
      
      digitalWrite(ledPin,ledState);
      digitalWrite(physicalPin, led12state);
      digitalWrite(11, led11state);
      digitalWrite(10, led10state);
      digitalWrite(9, led9state);
      digitalWrite(8, led8state);
      
      Serial.begin(9600);
    }
     
    void loop() {
    	check();
    }
     
    void check(){
      blinkAction.check(); //trigger every second
      physicalPixelAction.check(); //trigger every 50 millisecond
      asciiTableAction.check(); //trigger every 10 second
    blink11action.check();
    blink10action.check();
    blink9action.check();
    blink8action.check();
    
    }
     
    //[url=http://arduino.cc/en/Tutorial/Blink]Examples->Digital->Blink[/url]
    void blink(){
      ledState ? ledState=false : ledState=true;
      digitalWrite(ledPin,ledState);
    }
     
    //[url=http://arduino.cc/en/Tutorial/PhysicalPixel]Examples->Digital->PhysicalPixel[/url]
    void physicalPixel()
    {
    	led12state ? led12state=false : led12state=true;
      digitalWrite(12,led12state);
      
     // if (Serial.available()) {
     //  byte val = Serial.read();
     //   if (val == 'H') {
     //     digitalWrite(physicalPin, HIGH);
     //   }
     //   if (val == 'L') {
     //     digitalWrite(physicalPin, LOW);
     //   }
     // }
    }
     
     void blink11() {
     		led11state ? led11state=false : led11state=true;
      digitalWrite(11,led11state);
     }
     
     void blink10() {
     		led10state ? led10state=false : led10state=true;
      digitalWrite(10,led10state);
     }
     
     void blink9() {
     		led9state ? led9state=false : led9state=true;
      digitalWrite(9,led9state);
     }
     
     void blink8() {
     		led8state ? led8state=false : led8state=true;
      digitalWrite(8,led8state);
     }
     
    //[url=http://arduino.cc/en/Tutorial/ASCIITable]Examples->Digital->ASCIITable[/url]
    void asciiTable()
    {
      byte number = 33; // first visible character '!' is #33
      // print until we have printed last visible character '~' #126 ...
      while(number <= 126) {
        //Serial.print(number, BYTE);    // prints value unaltered, first will be '!'
     
        Serial.print(", dec: ");
        Serial.print(number);          // prints value as string in decimal (base 10)
        // Serial.print(number, DEC);  // this also works
     
        Serial.print(", hex: ");
        Serial.print(number, HEX);     // prints value as string in hexadecimal (base 16)
     
        Serial.print(", oct: ");
        Serial.print(number, OCT);     // prints value as string in octal (base 8)
     
        Serial.print(", bin: ");
        Serial.println(number, BIN);   // prints value as string in binary (base 2)
                                                                             // also prints ending line break
        number++; // to the next character
        check();
      }
      
      //asciiTableAction.disable();
    }
     
