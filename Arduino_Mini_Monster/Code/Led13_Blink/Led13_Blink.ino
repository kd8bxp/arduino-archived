/*Led13_Blink.ino  Agrucho 06 JAN 2017
First Sketch to test the STM32F103C8T6 module.
Experiments:
   Blink the on board LED.
   analogWrite() to control a blue Led.
   Writing 15000 RAM positions, (speed test).
   
(ARM32 Cortex-M3, 72 Mhz, 64K flash, 20K SRAM, 33 I/O pins, 3.3V)
Using the ST-Link V2 for Programming.
Using the USB to Serial adapter module (3.3V level) and Tera Term VT as a Serial Monitor.
Procedure:
    1.-Select:
          Board:         Generic STM32F103C series
          Variant:       STM32F103C8 (20k RAM, 64k Flash)
          Upload method: Serial
          Port:          COM port of the USB-to-serial adapter.
    2.-Compile.
    3.-Set Boot0=1, Boot1=0
    4.-Upload --> automatically RUN the Sketch.                    

To boot automatically from program memory:
    -Set Boot0=0
    -Press RESET button or cycle power.        
*/

#define LED PC13
#define BLUE PB1

char brightness=1;

//uint8_t sample_buffer[15000];  //testing 15k of RAM...

void setup() {
  Serial1.begin(9600);
  pinMode(LED, OUTPUT);
  Serial1.println("START");
 // sample_buffer[14000]=5;  
}

void loop() {
  //for(int i=0; i<15000 ; i++) sample_buffer[i]= (char)i;
  digitalWrite(LED, LOW);       // turns on the LED.
  analogWrite(BLUE, brightness);
  delay(100);
  digitalWrite(LED, HIGH);      // turns off the LED.
  delay(900);
  Serial1.print("Brightness= ");
  Serial1.println((int)brightness); 
  brightness= brightness<<1;    // multiply by 2.
  if(brightness == 0) brightness= 1; 
}
