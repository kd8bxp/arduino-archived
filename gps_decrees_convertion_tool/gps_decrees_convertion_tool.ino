//#include "math.h"

//decimal degrees probably used from GPS
float lat = 39.536404;
float lon = -84.381162;

/*
 * To Get to something APRS can use
 * 1st convert from decimal degrees to dms
 * Next convert to Degrees Minutes.m
 * 
 */
 
void setup() {
  Serial.begin(115200);
  int latDegrees = lat;
  int lonDegrees = lon;
  //int latMinutes = abs(lat*1000000-latDegrees*1000000);
  //int lonMinutes = abs(lon*1000000-lonDegrees*1000000);
  Serial.println("Decimal Decrees");
  Serial.print(lat,6); Serial.print(","); Serial.println(lon,6);
  //Serial.print(latDegrees); Serial.print(" "); Serial.println(latMinutes);
  //Serial.print(lonDegrees); Serial.print(" "); Serial.println(lonMinutes);
  int latM = abs(((lat - latDegrees)*60));
  int lonM = abs(((lon - lonDegrees)*60));
  //float templatM = latM/60.0F;
  //Serial.print("templatM: "); Serial.println(templatM,6);
  //float templatSS = lat -latDegrees;
  //Serial.print("templatSS: "); Serial.println(templatSS,6);
  //float latS = abs(templatSS - templatM) * 3600;
  float latS = abs((lat - latDegrees - latM/60.0F)) * 3600;
  float lonS = abs((abs(lon - lonDegrees) - lonM/60.0F)) * 3600;
  Serial.println("Degrees Minutes Seconds");
  Serial.print("Lat D: "); Serial.print(latDegrees); Serial.print(" M: "); Serial.print(latM); Serial.print(" S: "); Serial.println(latS);
  Serial.print("Lon D: "); Serial.print(lonDegrees); Serial.print(" M: "); Serial.print(lonM); Serial.print(" S: "); Serial.println(lonS);
float latMM = (latM +(latS/60.0F));
float lonMM = (lonM +(lonS/60.0F));

Serial.println("Degress Minutes.M");
Serial.print("Lat D: "); Serial.print(latDegrees); Serial.print(" MM: "); Serial.println(latMM);
Serial.print("Lon D: "); Serial.print(lonDegrees); Serial.print(" MM: "); Serial.println(lonMM);

String aprsLat, aprsLon, latDirection, lonDirection;

latDegrees = abs(latDegrees);
lonDegrees = abs(lonDegrees);

if ((int)lat > 0) { latDirection = "N"; } else { latDirection = "S"; }
if ((int)lon > 0) { lonDirection = "E"; } else { lonDirection = "W"; }

aprsLat = (String)latDegrees+(String)latMM;
//Serial.print(lonDegrees); Serial.print(" "); Serial.println(numdigits(lonDegrees));
if (numdigits(lonDegrees) < 3) {aprsLon = "0"+(String)lonDegrees+(String)lonMM;} else {aprsLon = (String)lonDegrees+(String)lonMM;}
aprsLat = aprsLat + latDirection;
aprsLon = aprsLon + lonDirection;

Serial.println("APRS: ");
Serial.print("Latitude: "); Serial.println(aprsLat);
Serial.print("Longitude: "); Serial.println(aprsLon);


   
}

void loop() {
  // put your main code here, to run repeatedly:

}

int numdigits(int i)
{
       int digits;

       i = abs(i);
       if (i < 10)
         digits = 1;
       else
         digits = (int)(log10((double)i)) + 1;
       return digits;
}

/*
 * used: https://www.directionsmag.com/site/latlong-converter/
 * and
 * https://www.rapidtables.com/convert/number/degrees-to-degrees-minutes-seconds.html
 * 
 * Other Latitude/Longitude resources
 * https://www.latlong.net/degrees-minutes-seconds-to-decimal-degrees
 * https://www.fcc.gov/media/radio/dms-decimal
 * 
 * APRS online converter
 * http://digined.pe1mew.nl/?How_to:Convert_coordinates
 */
