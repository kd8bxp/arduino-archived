/********************************************************
 * PID RelayOutput Example
 * Same as basic example, except that this time, the output
 * is going to a digital pin which (we presume) is controlling
 * a relay.  the pid is designed to Output an analog value,
 * but the relay can only be On/Off.
 *
 *   to connect them together we use &quot;time proportioning
 * control&quot;  it's essentially a really slow version of PWM.
 * first we decide on a window size (5000mS say.) we then 
 * set the pid to adjust its output between 0 and that window
 * size.  lastly, we add some logic that translates the PID
 * output into &quot;Relay On Time&quot; with the remainder of the 
 * window being &quot;Relay Off Time&quot;
 ********************************************************/

#include &lt;PID.h&gt;
#define RelayPin 6

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&amp;Input, &amp;Output, &amp;Setpoint,2,5,1, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;
void setup()
{
  windowStartTime = millis();
  
  //initialize the variables we're linked to
  Setpoint = 100;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  Input = analogRead(0);
  myPID.Compute();

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  if(millis() - windowStartTime&gt;WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if(Output &lt; millis() - windowStartTime) digitalWrite(RelayPin,HIGH);
  else digitalWrite(RelayPin,LOW);

}



