/*One_Pixel_ThermoCam_02.ino  Agrucho 4 JAN 2017
Using the MLX90614 temperature sensor, attached to a 5/15" ROD COUPLING NUT
for more directional sensing (lower gain and sensitivity).
A Servo controlled Pan and Tilt is used to scan 30 lines of 40 readings.

Processing is used to display the thermal image: Pro_ThermoCam_01.pde

 Hardware:
  -Arduino NANO.
  - SPT100 Direct Pan and Tilt System from http://www.robotshop.com
  - 2 Standar Servos.
  -MLX90614 temperature sensor
  -5/15" ROD COUPLING NUT
 Wiring:
     MLX90614       NANO
       VIN -------- 5V
       GND -------- GND
       SCL -------- A5
       SDA -------- A4
------------------------------------------------------------------------------*/

#include <Servo.h> 
#include <Wire.h>
#include "Adafruit_MLX90614.h"  //.h and .cpp files in this folder.


#define TILT_SERVO  7
#define PAN_SERVO   8

#define H_SIZE       40   // Horizontal picture elements.
#define V_SIZE       30   // Vertical picture elements.
#define Xcenter     100   // X image center (Servo position in degrees)
#define Ycenter     120   // Y image center (Servo position in degrees)
#define SETTIME     10   // Servo Setting Time in milliseconds.


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float ambTemp, objTemp, diffTemp;

Servo servo_tilt;
Servo servo_pan;

//======================= functions ============================================
/*------------------------------------------------------------------------------
  sendPixel() , Send the coordinates and the temperature measured.
  xPos : x coordinate of the temperature spot.
  yPos : y coordinate of the temperature spot.
  temVal: number representing the temperature value.
------------------------------------------------------------------------------*/
void SendPixel(int xPos, int yPos, int tempVal){
   Serial.print(xPos);
   Serial.print(",");
   Serial.print(yPos);
   Serial.print(",");
   Serial.println(tempVal); 
}

/*------------------------------------------------------------------------------ 
  goto_origin() , Set the Servos to point to the (0,0) coordinates.
------------------------------------------------------------------------------*/
void goto_origin(){
  servo_tilt.write( V_SIZE/2 + Ycenter ); // Tilt servo at top.
  servo_pan.write( Xcenter - H_SIZE/2 );  // Pan servo to left.
  delay(1000);                            // time to reach origin.
}

/*------------------------------------------------------------------------------ 
  readTemp() , dead the differnce between  the onject and the ambient.
  200 samples are added.
  A Better aproach will be using EEPROM's Config Register1
  to change Gain, IRF and IIF Filters. (Not supported by Adafruit's Library)
------------------------------------------------------------------------------*/
float readTemp(){
  diffTemp=0;
  ambTemp= mlx.readAmbientTempC();
  for(int i=0; i<200; i++){
    objTemp= mlx.readObjectTempC();
    diffTemp+= (objTemp - ambTemp);
  }
  return  1*(100+int(diffTemp));   // value range ajusted experimentally.
}

//======================= setup ================================================
void setup() 
{ 
  servo_tilt.attach(TILT_SERVO); 
  servo_pan.attach(PAN_SERVO);
  Serial.begin(115200);
  mlx.begin();
} 

//========================== main loop =========================================
void loop(){

  goto_origin();                                 // Set (0,0) position.

  for (int y=V_SIZE-1 ; y>=0 ; y--) { 
    
    servo_tilt.write(Ycenter + V_SIZE/2 -y );
    delay(SETTIME);                              // time to reach position.   
    for ( int x=0 ; x<H_SIZE ; x++) {            // Scan from Left to Right.
      servo_pan.write(Xcenter - H_SIZE/2 + x);
      delay(SETTIME);                            // time to reach position.
     SendPixel(x, y, readTemp());      
    }
    y--;                                         // Next line.
    servo_tilt.write(Ycenter + V_SIZE/2 - y);
    delay(SETTIME);                              // time to reach position.    
    for ( int x=H_SIZE-1 ; x>=0 ; x--) {         // Scan from Right to Left.
      servo_pan.write(Xcenter - H_SIZE/2 + x);     
      delay(SETTIME);                            // time to reach position.
    SendPixel(x, y, readTemp());
    } 
  }
}

// end of code.
