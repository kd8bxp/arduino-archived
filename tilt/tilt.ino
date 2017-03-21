/*
Celle Niedersachsen
Latitude: 52° 37' 21.5" N
Longitude: 10° 4' 49.7" E
Magnetic declination: +2° 40'      // important !!!!!!!!!!!!!!!!!!!!!! see line  34 
Declination is POSITIVE (EAST)
Inclination: 67° 47' 
Magnetic field strength: 49432.9 nT 




*/

// For 
//Board  Arduino Mega 2560 and Display 480 x 320     Display 3,2 " Zoll 3.2 TFT 480x320 HX8357 or ILI9481 Arduino 

//use 4  Wires for   GY 80 :  Gnd  +5V   SCL   SDA



// important!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!wichtig!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// mit calibration einstellen        adjust  with calibration 
int xoffset=-45;        // without calibration no or bad function
int yoffset=71;

#define ALTITUDE 60.0 // Altitude in Celle, Deutschland    Fehler korrigiert

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (2.0 + (35.0 / 60.0)) / (180 / M_PI);   //important !!!!!!!!!!!!!!Celle dj8ja




char *(str2(int(z)));                              // zahl in string 00, 01 usw.
char *(str1(byte(z)));                       // byte in string 0, 1 usw.
String (Str2(int(z)));// das Gleich, aber richtiger String


#include <Wire.h>
#include <HMC5883L.h>
#include <ADXL345.h>
#include <SFE_BMP180.h>
SFE_BMP180 pressure;

HMC5883L compass;
ADXL345 accelerometer;

float heading1;
float heading2;
float fax[12];
float fay[12];
float faz[12];

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


#include <UTFT.h>
double x, y, a, a2;
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
UTFT myGLCD(HX8357B, 38, 39, 40, 41);// !!!!!!!!!!important wichtig some displays need here il9481
float xmin, xmax, ymin, ymax;
int xm, ym;

//***************************************************************************************************************
void linie(float x1, float y1, float x2, float y2, word color)  // zeichnet Linie von bis mit color
{
  word farbe = myGLCD.getColor(); myGLCD.setColor(color);
  int xi1, yi1, xi2, yi2;
  xi1 = round((x1 - xmin) / (xmax - xmin) * xm);
  yi1 = ym - round((y1 - ymin) / (ymax - ymin) * ym);
  xi2 = round((x2 - xmin) / (xmax - xmin) * xm);
  yi2 = ym - round((y2 - ymin) / (ymax - ymin) * ym);
  // myGLCD.drawLine( xi1, yi1, xi2, yi2);
  if (xi1 < 0) xi1 = 0; if (yi1 < 0) yi1 = 0; if (xi2 < 0) xi2 = 0; if (yi2 < 0) yi2 = 0;
  if (xi1 > xm - 1) xi1 = xm - 1; if (xi2 > xm - 1) xi2 = xm - 1;
  if (yi1 > ym - 1) yi1 = ym - 1; if (yi2 > ym - 1) yi2 = ym - 1;
  myGLCD.drawLine( xi1, yi1, xi2, yi2);
  myGLCD.setColor(farbe);
}



//***************************************************************************************************************
void punkt(float x, float y, word color)       //zeichnet pixel bei x,y mit Farbe color
{ word farbe = myGLCD.getColor(); myGLCD.setColor(color);
  int xi, yi, i;
  xi = round((x - xmin) / (xmax - xmin) * xm);
  yi = ym - round((y - ymin) / (ymax - ymin) * ym);

  // myGLCD.invPixel( xi, yi); // funktioniert auch
  if (xi < 0) xi = 0; if (yi < 0) yi = 0;
  if (xi > xm - 1) xi = xm - 1;
  if (yi > ym - 1) yi = ym - 1;
  myGLCD.drawPixel(xi, yi);
  myGLCD.setColor(farbe);

}





void kreis(float x, float y, float r, word color)
{
  myGLCD.setColor(color);
  for (float phi = 0; phi <= 2 * PI; phi = phi + 0.1)
  {
    punkt(r * cos(phi) + x, r * sin(phi) + y, color);
  }
}







float Xh, Yh;
float Xhalt, Yhalt, Xalt, Yalt;
float h1x, h1y, h1xalt, h1yalt;
float h2x, h2y, h2xalt, h2yalt;

float dax, day, daz;

int index = 0;

float sax = 0; float say = 0; float saz = 0;

float temperature;
//**************************************************************************************************************
void setup()
//**************************************************************************************************************
{


  myGLCD.InitLCD(LANDSCAPE);  // Querformat USB-Anschluss Links

  // myGLCD.setFont(SmallFont);
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.setFont(SevenSegNumFont);

  myGLCD.setColor(255, 255, 255);


  xmin = -500;    xmax = 500   ;   ymin = -650 ; ymax = 350;
  // xm = 240; ym = 320;
  xm = 480; ym = 480;
  linie(-300, 0, 300, 0, GREEN);
  linie(0, -200.9, 0, 200.9, GREEN);


  Serial.begin(115200);

  // Initialize ADXL345

  if (!accelerometer.begin())
  {
    delay(500);
  }

  accelerometer.setRange(ADXL345_RANGE_2G);

  // Initialize Initialize HMC5883L
  while (!compass.begin())
  {
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
 // compass.setOffset(-45, 71);   // aus calibration ,vorher 0,0  12 71  ###################################################
  //  compass.setOffset(0, 0);
 compass.setOffset(xoffset, yoffset); 
  //kreis(0,0,300);
  kreis(0, 0, 310, WHITE);
  myGLCD.setColor(BLACK);
  myGLCD.print("          ", LEFT, 40);
  myGLCD.setColor(BLACK);
  myGLCD.setColor(WHITE);
  myGLCD.setFont(BigFont);
  myGLCD.print("0", 230, 0);
  myGLCD.print(" 90", 380, 160);
  myGLCD.print("270", 40, 160);
  myGLCD.print("dj8vi", 40, 300);
  myGLCD.print("30.11.16", 330, 300);

  pressure.begin();

  float pressure = readPressure();

  index = 0;


  sax = 0; say = 0; saz = 0;    // sum for accellaration
  for (int i = 0; i <= 8; i++)
  {
    fax[i] = 0;                  // arrays for 8 values acceleration  init
    fay[i] = 0;
    faz[i] = 0;
  }


}
//**************************************************************************************************************



// No tilt compensation
//**************************************************************************************************************
float noTiltCompensate(Vector mag)
//**************************************************************************************************************
{
  float heading = atan2(mag.YAxis, mag.XAxis);
  /*
    Serial.print(mag.XAxis); Serial.print("  ");
    Serial.print(mag.YAxis); Serial.print("  ");
    Serial.println(mag.ZAxis);
  */
  return heading;
}





// Tilt compensation
//**************************************************************************************************************
float tiltCompensate(Vector mag, Vector normAccel)
//**************************************************************************************************************
{
  // Pitch & Roll

  float roll;
  float pitch;
  float ax, ay, az;
  int maxindex = 8;
  az = normAccel.ZAxis;  //
  // ax = -normAccel.XAxis*az*0.8; //   Korrekturfaktor ?????  try
  // ay = normAccel.YAxis*az*0.99;  //   Korrekturfaktor ??????   try

  ax = -normAccel.XAxis * 0.8; //   Korrekturfaktor ?????  try
  ay = normAccel.YAxis * 0.8; //   Korrekturfaktor ??????   try


  //average of 8 values, low pass




  sax = sax - fax[index];     // one value subtract
  fax[index] = ax;            // new value insert
  sax = sax + fax[index]; // oder ax           new sum
  dax = sax / maxindex;         // average



 say = say - fay[index];     // same for acc y
  fay[index] = ay;
  say = say + fay[index]; // oder ay
  day = say / maxindex;


  saz = saz - faz[index];      // same for acc z , not used
  faz[index] = az;
  saz = saz + faz[index]; // oder az
  daz = saz / maxindex;


  index++;
  if (index >= maxindex) {        //   wrap index
    index = 0;
  }



//   old version, delivers the same
  /*

    for (int i = 0; i <= 6; i++)
    {
     fax[i] = fax[i + 1];
    }
    fax[7] = ax;

    for (int i = 0; i <= 7; i++)
    {
     sax = sax + fax[i];
    }
    dax = sax / 8.0;



    for (int i = 0; i <= 6; i++)
    {
     fay[i] = fay[i + 1];
    }
    fay[7] = ay;

    for (int i = 0; i <= 7; i++)
    {
     say = say + fay[i];
    }
    day = say / 8.0;

    for (int i = 0; i <= 6; i++)
    {
     faz[i] = faz[i + 1];
    }
    faz[7] = az;

    for (int i = 0; i <= 7; i++)
    {
     saz = saz + faz[i];
    }
    daz = saz / 8.0;
  */
 
  
  
  /*
    // low pass filter, you can experiment with filter factor f, f higher (0.8) means more filtering, but slower
    float f=0.7;
    dax=ax*(1-f)+dax*f;  //
    day=ay*(1-f)+day*f;  //
    daz=az*(1-f)+daz*f;  //

  */
  Serial.print(dax,4); Serial.print(" ****"); Serial.print(day,4); Serial.print(" ****"); Serial.println(daz,4);


// another formula, using also acc z
  // roll   = atan2(  day ,  sqrt(sq(dax)+sq(daz)));
  // pitch  = atan2(  dax ,  sqrt(sq(day)+sq(daz)));

 // Serial.print(roll, 5); Serial.print(" ... "); Serial.println(pitch, 5);

  roll = asin(day);
  pitch = asin(dax);
 // Serial.print(roll, 5); Serial.print("  +++"); Serial.println(pitch, 5);

  if (roll > 0.78 || roll < -0.78 || pitch > 0.78 || pitch < -0.78)
  {
    return -1000;
  }

  // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);

  // Tilt compensation
  Xh = mag.XAxis * cosPitch + mag.ZAxis * sinPitch;
  Yh = mag.XAxis * sinRoll * sinPitch + mag.YAxis * cosRoll - mag.ZAxis * sinRoll * cosPitch;

  float heading = atan2(Yh, Xh);

  return heading;
}

// Correct angle
//**************************************************************************************************************
float correctAngle(float heading)
//**************************************************************************************************************
{
  if (heading < 0) {
    heading += 2 * PI;
  }
  if (heading > 2 * PI) {
    heading -= 2 * PI;
  }

  return heading;
}




//**************************************************************************************************************
void loop()
//**************************************************************************************************************
{


  float humidity, pressure;
  pressure = readPressure();
  char pressF[7];
  char tempF[6];
  dtostrf(pressure, 5, 1, pressF);
  dtostrf(temperature, 5, 1, tempF);
  // Serial.println(pressF);

  myGLCD.setColor(RED);
  int  pressline = 5;

  int pressy = pressline / 8.0 * 320.0 - 10;

  myGLCD.print(pressF, LEFT + 160, pressy);
  myGLCD.print("hPa", LEFT + 270, pressy);

  int templine = 3;                          // Zeilennr für Temperatur

  int tempy = templine / 8.0 * 320.0 + 10;

  myGLCD.setFont(BigFont);

  myGLCD.setColor(YELLOW); myGLCD.print(tempF, LEFT + 160, tempy);
  myGLCD.print(" C", LEFT + 250, tempy); myGLCD.print("o", LEFT + 250, tempy - 5);


  // Read vectors
  Vector mag = compass.readNormalize();
  Vector acc = accelerometer.readScaled();

  // Calculate headings
  heading1 = noTiltCompensate(mag);

  heading2 = tiltCompensate(mag, acc);
  linie(-300, 0, 300, 0, GREEN);
  linie(0, -300.0, 0, 300.0, GREEN);

  if (heading2 == -1000)
  {
    heading2 = heading1;
  }


  heading1 += declinationAngle;
  heading2 += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  heading1 = correctAngle(heading1);


  linie(0, 0, h1xalt, h1yalt, BLACK);
  h1x = sin(heading1) * 300;
  h1y = cos(heading1) * 300;
  linie(0, 0, h1x, h1y, RED);

  h1xalt = h1x;
  h1yalt = h1y;




  heading2 = correctAngle(heading2);
  linie(0, 0, h2xalt, h2yalt, BLACK);
  //  kreis(h2xalt,h2yalt,20,BLACK);
  h2x = sin(heading2) * 300;
  h2y = cos(heading2) * 300;
  linie(0, 0, h2x, h2y, YELLOW);

  // kreis(h2x,h2y,20,WHITE);
  h2xalt = h2x;
  h2yalt = h2y;


  // Convert to degrees
  heading1 = heading1 * 180 / M_PI;
  heading2 = heading2 * 180 / M_PI;
  /*
    myGLCD.setColor(BLACK);
    if (heading2<100){ myGLCD.print("   ", LEFT, 40);}   // sonst flimmert es
    myGLCD.setColor(WHITE);
  */

  myGLCD.setFont(SevenSegNumFont);
  if (heading2 >= 0) {

    myGLCD.print(str2(round(heading2)), LEFT, 40);
    // myGLCD.printNumI(round(heading2), LEFT, 40);  //NumF mit keiner Nachkommastele funktioniert nicht
  }
  myGLCD.setFont(BigFont);
  // Output
  // Serial.print(heading1); Serial.print("     :     "); Serial.println(heading2);
  //kreis(0,0,300);
  kreis(0, 0, 310, WHITE);
  // delay(40);
}




//*********************** wandelt integer in zweistelligen String 01, 02 usw *********
char *(str2(int(z)))
{ static  char Tmps[10]; // Variable bleibt erhalten, sonst Rückgabe des Pointers nicht möglich
  // darf nicht 2 sein,mindestens 3 warum, weiß ich nicht, Vorzeichen?
  dtostrf(z, 3, 0, Tmps); // 2 gesamtstellen, Null Nachkommastellen
  if (Tmps[0] == ' ') {
    Tmps[0] = '0';
  }
  if (Tmps[1] == ' ') {
    Tmps[1] = '0';
  }
  return (Tmps);

}


//*********************** wandelt integer in zweistelligen String 01, 02 usw *********
String (Str2(int(z)))
{ String zz;
  static  char Tmps[10]; // Variable bleibt erhalten, sonst Rückgabe des Pointers nicht möglich
  // darf nicht 2 sein,mindestens 3 warum, weiß ich nicht, Vorzeichen?
  dtostrf(z, 2, 0, Tmps); // 2 gesamtstellen, Null Nachkommastellen
  if (Tmps[0] == ' ') {
    Tmps[0] = '0';
  }
  zz = String(Tmps);
  return (zz);

}

//***************************************************************************************************************
float readPressure()
{
  char status;
  double T, P, p0, a;

  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      temperature = T;
      /*
        Serial.print("temperature: ");
        Serial.print(T, 2);
        Serial.print(" deg C, ");
        Serial.print((9.0 / 5.0)*T + 32.0, 2);
        Serial.println(" deg F");
      */


      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          /*
                    // Print out the measurement:
                    Serial.print("absolute pressure: ");
                    Serial.print(P, 2);
                    Serial.print(" mb, ");
                    Serial.print(P * 0.0295333727, 2);
                    Serial.println(" inHg");

                    // The pressure sensor returns abolute pressure, which varies with altitude.
                    // To remove the effects of altitude, use the sealevel function and your current altitude.
                    // This number is commonly used in weather reports.
                    // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
                    // Result: p0 = sea-level compensated pressure in mb

                    p0 = pressure.sealevel(P, ALTITUDE); // we're at 1655 meters (Boulder, CO) Celle
                    Serial.print("relative (sea-level) pressure: ");
                    Serial.print(p0, 2);
                    Serial.print(" mb, ");
                    Serial.print(p0 * 0.0295333727, 2);
                    Serial.println(" inHg");

          */





          p0 = pressure.sealevel(P, ALTITUDE);
          return p0;
        }
      }
    }
  }
}

