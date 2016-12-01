/*
 * Move toward the light
 * Compare inputs from left and right shrouded phototransistors (see photo).
 * If you see more light in right detector move to left.
 * If you see more light in left detector move to right.
 * The best position for this temporary setup is detectors pointing slightly outlide
 * of the target \/.
 * Speed here is intentionally slow 
 */

//    servoLeft.detach();                      // Stop servo signals
//    servoRight.detach();   
//  servoLeft.writeMicroseconds(1700);         // Full speed forward
//  servoRight.writeMicroseconds(1300);

// First Complete Run from Road to Garage7/27/2015 10:15 pm
// NOTES
// Noted shift to right (possibly due to optical assy alignment
// (skimmed rubber door gasket slightly but continued
// Started vehicle 3 ft to Left
// Reduced minimum operating level to .05V
// Noted what appeared to be a "start from dark" problem with sensors
//     If you follow the light out to the destination it will work
//     but finding the light from the destination was impossible.
//     Later I noted you seem to be able to excited the sensor
//     and it would remain excited for some time (guess 10 sec).
//     (possible shift to photo diodes/transistor)
// Best alignment seems to be right tube at target, left tube at angle
// pointed to target (angle toward right tube).  Paralled appers to
// not be useful (possibly due to differences in tubes.
// ACTION ITEMS
// 1. Align and test r/l tubes in center with same angle
//      Confirm "center" of both tubes and align or make new tubes.
// 2. possible use of tubes (or left right blocks on source to avoid shifting

//  ALIGNMENT PROCEDURE
// 
// 1.  With tubes in parallel:
//        Used Debug = 2 to set the tubes to equal voltage when pointed and light source. 3.2V at 10ft.
// 2.  Adjusted min threshold to high value (current 2.0 - getting 3.2 out of both tubes.
// 3.  Put tubes as slight angle out at front (maybe 2-3 degrees).//     (might need to automate this again).  balance
//        Adjust using debug 2
// 4.  Move optics (as a whole) to align robot running true to target

#include <Servo.h>                           // Include servo library
 
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
float minValidVoltageSetpoint =2.0;         // Minimum Voltage to operate the optic at
float endTravelVolageSetpoint =4.5;         // Setpoint for end of trip
int startupSuccessful =0;                   // successful start (voltage above minimum etc)
int endTravelFound =0;                      // End of trip discovered
float sensorBalance = 1.12;                 // used to balance two sensors
int debug = 3;                              // 2=calibrate optics steering, 3=cal transition counter (of lamp)
long printline = 0;                         // Use to control periodic printing/resets

//  Used for averaging inputs for freq detection and steering
float analogInValsA1;
float analogInValsA2;
float analogInValsA3;
float analogInValsA4;
float analogAvValA;

float analogInValsB1;
float analogInValsB2;
float analogInValsB3;
float analogInValsB4;
float analogAvValB;
float analogInAvDif;

float analogInValsC1;
float analogInValsC2;
float analogInValsC3;
float analogInValsC4;
float analogAvValC;

// Use to pick highest voltage optical inputs for A and B
float Volts1 = 0.0;  //Used for A
float Volts2 = 0.0;
float Volts3 = 0.0;
float Volts4 = 0.0;
float Volts5 = 0.0;  //Used for B
float Volts6 = 0.0;
float Volts7 = 0.0;
float Volts8 = 0.0;

float ratioTarget = 1.2; // defines minimum change that could 
                         // be called a transition of lamp soruce


float voltsA = 0.0;       // Peak or average signal from optical device A
float voltsB = 0.0;       // Peak or average signal from optical device B
float ratioA = 0.0;       // Ratio of change of input A
float ratioB = 0.0;       // Ratio of change of input B

long changeCount = 0;      // Hom many transitions did you find?
long changeCountMin = 2;  // Minimum transitions per cycle for your source (souce is blinking)
long changeCountMax = 5;  // Maximum transitions per cycle for your source (souce is blinking)
int freqLimit_Exceeded =0; // Number of lamp transitions below or above limits

 
void setup()                                 // Built-in initialization block
{
  tone(4, 2900, 300);                       // Start tone - .5 second
  delay(300);                               // Delay to finish tone
  tone(4, 3000, 300);                       // Start tone - .5 second
  delay(300);    // Delay to finish tone
  Serial.begin(19200);
}


void loop()                                  // Main loop auto-repeats
{
    
printline = ++printline;
if (printline == 50)
  {
  
    printline=0;
    if (debug > 0)
    {
    Serial.print("  F: "); 
    Serial.println(changeCount);
    }

    freqLimit_Exceeded = 0;
    if (changeCount > changeCountMax || changeCount < changeCountMin)
    {
      freqLimit_Exceeded;
      if (debug > 0)
        {
        beepMulti(50);
        }
    }
    changeCount = 0;
    beepHigh(50);
///    delay(200);    // delay only on print cycle

  }
  
  // Get average input values to detect a transition
  analogInValsA1 = volts(A0);
  analogInValsA2 = volts(A0);
  analogInValsA3 = volts(A0);
  analogInValsA4 = volts(A0);
  analogAvValA   =(analogInValsA1 +  //Average values
                   analogInValsA2 +
                   analogInValsA3 +
                   analogInValsA4) / 4.0;
  delay(10);   //Time delay to ammempt to catch transition
  analogInValsB1 = volts(A0);
  analogInValsB2 = volts(A0);
  analogInValsB3 = volts(A0);
  analogInValsB4 = volts(A0); 
  analogAvValB   =(analogInValsB1 +   //Average values
                   analogInValsB2 +
                   analogInValsB3 +
                   analogInValsB4) / 4.0;  
  ratioA = analogAvValB / analogAvValA;  // What was size of transition

  voltsA = analogAvValC;
  
  analogInValsC1 = volts(A3);
  analogInValsC2 = volts(A3);
  analogInValsC3 = volts(A3);
  analogInValsC4 = volts(A3); 
  analogAvValC   =(analogInValsC1 +
                   analogInValsC2 +
                   analogInValsC3 +
                   analogInValsC4) / 4.0;
  voltsB = analogAvValC; 
  
  if(ratioA > ratioTarget)      // Was the transition big enough 
                                // to count??
  {
    if (debug == 3)
       {
       Serial.print(printline);   //print cycle number in debug
       Serial.print(" B>A ");
       Serial.println(ratioA);
       }
    changeCount = ++changeCount;  // Count valid transitions
  }

  ratioB = analogAvValA / analogAvValB; //tranistion in other direction
  if(ratioB > ratioTarget)
  {
     if (debug == 3)
       {
       Serial.print(printline);
       Serial.print(" A>B ");
       Serial.println(ratioB);
       }
    changeCount = ++changeCount; // Count valid transitions (other direction)
  }
  
// Gather and average voltages and get peaks from both sensors  
 Volts1 =  volts(A0);
 Volts5 =  volts(A3);
 delay(10);
 Volts2 =  volts(A0);
 Volts6 =  volts(A3);
 delay(10);
 Volts3 =  volts(A0);
 Volts7 =  volts(A3);
  delay(10);
 Volts4 =  volts(A0);
 Volts8 =  volts(A3);
  
 voltsA=Volts1;    //Get highet value
 if(Volts2 > voltsA){voltsA = Volts2;}
 if(Volts3 > voltsA){voltsA = Volts3;}
 if(Volts4 > voltsA){voltsA = Volts4;}
 
 voltsB=Volts5;    //Get highest value
 if(Volts6 > voltsA){voltsB = Volts6;}
 if(Volts7 > voltsA){voltsB = Volts7;}
 if(Volts8 > voltsA){voltsB = Volts8;}
  
// voltsA is peak value for A input
// voltsB is peak value for B input
 
// Verify miminum requied voltages and compare sensor values
// and control the drive motors from the data.

  if(voltsA > minValidVoltageSetpoint && 
     voltsB > minValidVoltageSetpoint)     
  {
    if(startupSuccessful == 0)      // Successful Startup detected
    {
      servoLeft.attach(11);                      // Attach left signal to pin 13
      servoRight.attach(12);                     // Attach right signal to pin 12
      startupSuccessful = 1;                     // lock out this section in future
    }
    
    //  Detect End Of Trip (Stop Servos)
    // Stop Servo's and Play Shutdown Tune  High>>>>Lowwww
    if(voltsA > endTravelVolageSetpoint ||
       voltsB > endTravelVolageSetpoint)
    {

      if (debug < 3)
      {
        beepLampReached(300);
      }
      else
      {
        servoLeft.detach();                      // Stop servo signals
        servoRight.detach();
      }
      endTravelFound =1;
    }
    
     //Check for minimum voltage before starting
     //guidance section
     if(voltsA > minValidVoltageSetpoint && 
     voltsB > minValidVoltageSetpoint) 
    {
      if (debug  == 2)
         {
         Serial.print("Volts L: "); 
         Serial.print(voltsA);
         Serial.print("  R: "); 
         Serial.println(voltsB);
         }
      if(voltsA > voltsB)      // LEFT Sensor High (Turn LEFT)
      {
        turnLeft(100);
      }
 
      if(voltsA < voltsB)        // RIGHT Sensor High (TURN RIGHT)
      {
       turnRight(100);
      }
    }
  }
  else
  {      
    if (debug == 2)  // Sensor Input is below minimum value beep beep beep beep
     {
       beepMulti(40);
     }
    
  }  
}



float volts(int adPin)                       // Measures volts at adPin
{                                            // Returns floating point voltage
 return float(analogRead(adPin)) * 5.0 / 1024.0;
}   


void turnLeft(int time)                      // Left turn function
{

  //   servoLeft.writeMicroseconds(1480);        // Left wheel counterclockwise
  //   servoRight.writeMicroseconds(1530);       // Right wheel clockwise  
  
     servoLeft.writeMicroseconds(1480);        // Left wheel counterclockwise
     servoRight.writeMicroseconds(1536);       // Right wheel clockwise
     if (debug == 2)
     {
       beepHigh(200);
       Serial.print("<< LEFT Volts L: "); 
       Serial.print(voltsA);
       Serial.print("  R: "); 
       Serial.println(voltsB);
     }
}

void turnRight(int time)                     // Right turn function
{

//     servoLeft.writeMicroseconds(1494);        // Left wheel counterclockwise
//     servoRight.writeMicroseconds(1530);       // Right wheel clockwise
  
     servoLeft.writeMicroseconds(1490);        // Left wheel counterclockwise
     servoRight.writeMicroseconds(1530);       // Right wheel clockwise
     if (debug == 2)
     {
       beepLow(200);
       Serial.print(">> RIGHT Volts L: "); 
       Serial.print(voltsA);
       Serial.print("  R: "); 
       Serial.println(voltsB);
     } 
}

void beepHigh(int time)                      // 
{
    tone(4, 3000, time);                       //  high pitch beep
    delay(time);
}

void beepLow(int time)                      // low pitch beep
{
    tone(4, 2800, time);                       
    delay(time);
}

void beepMulti(int time)                      // Left turn function
{
    tone(4, 2800, time);                       // Play RIGHT sensor sound
    delay(time);                               // Delay to finish tone
    tone(4, 2800, time);                       // Play RIGHT sensor sound
    delay(time);                               // Delay to finish tone
    tone(4, 2800, time);                       // Play RIGHT sensor sound
    delay(time);                               // Delay to finish tone  
}


void beepLampReached(int time)                      // Left turn function
{
     tone(4, 3000, time);                       // Play high tone for .1 second
      delay(time);                               // Delay to finish tone
      tone(4, 2800, time);                       // Play low tone for .1 second
      delay(time);   
}
