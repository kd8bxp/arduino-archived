#include "src/M5Atom/M5Atom.h"
#include "CBACK_DRIVER.h"
#include <Wire.h>
#include "person_sensor.h"

//defines for servos
#define PAN 1  //cback port number
#define TILT 2 //cback port number

//center servo
#define TILT_UP 125 //180 //0 is flat down
#define PAN_CENTER 90 //0 is point to left, 180 is point to right


//Person Sensor
// How long to wait between reading the sensor. The sensor can be read as
// frequently as you like, but the results only change at about 5FPS, so
// waiting for 200ms is reasonable.
const int32_t SAMPLE_DELAY_MS = 200;

int box_center_x;
int box_center_y;
int prev_center_x = 1;

int xval = PAN_CENTER;
int yval = TILT_UP;
int trimval;
int current_xval;
int prev_xval = 512;


CBACK_DRIVER Driver;


void setup() {
   M5.begin();
   //Wire1.begin(0,26);
   Wire.begin(26,32);
   Serial.begin(115200);
 
   Driver.Init();

   //Drive servos to center point
   Driver.SetServoAngle(PAN, PAN_CENTER);
   Driver.SetServoAngle(TILT, TILT_UP);
   Driver.SetOutput(0); //??
}

void loop() {
 person_sensor_results_t results = {};
  // Perform a read action on the I2C address of the sensor to get the
  // current face information detected.
  if (!person_sensor_read(&results)) {
    //Serial.println("No person sensor results found on the i2c bus");
    delay(SAMPLE_DELAY_MS);
    return;
  }

  Serial.println("********");
  Serial.print(results.num_faces);
  Serial.println(" faces found");
  for (int i = 0; i < results.num_faces; ++i) {
    const person_sensor_face_t* face = &results.faces[i];
    Serial.print("Face #");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(face->box_confidence);
    Serial.print(" confidence, (");
    Serial.print(face->box_left);
    Serial.print(", ");
    Serial.print(face->box_top);
    Serial.print("), (");
    Serial.print(face->box_right);
    Serial.print(", ");
    Serial.print(face->box_bottom);
    Serial.print("), ");
    if (face->is_facing) {
      Serial.println("facing");
    } else {
      Serial.println("not facing");
    }
  }


for (int i = 0; i < results.num_faces; ++i) {
    const person_sensor_face_t* face = &results.faces[i];
    box_center_x = (face->box_left + ((face->box_right)-(face->box_left))/2);  // turns sensor box into x-axis point for eye-contact
    box_center_y = (face->box_bottom + ((face->box_top)-(face->box_bottom))/2);  // turns sensor box into y-axis point for eye-contact
    
    prev_center_x = box_center_x;

int panDelta;
int tiltDelta;
panDelta = (box_center_x - 128) * 0.1;
tiltDelta = (box_center_y - 128) * 0.1;

xval -= panDelta;
if (xval > 180) { xval = 180; }
else if (xval < 0) {xval = 0; }

yval -= tiltDelta;
if (yval > 180) { yval = 180; }
else if (yval < 0) {yval = 0; }

    // SERVO POSITIONING
//    xval = (PAN_CENTER - box_center_x); // ((box_center_x * -4) + 1023)*1.2;   // translate sensor information into servo info
//    yval = (box_center_y - TILT_UP); //((box_center_y * -4) + 1023)*1.2;
Serial.print("X, Y: "); Serial.print(xval); Serial.print(","); Serial.println(yval);
 Driver.SetServoAngle(PAN, xval);
 Driver.SetServoAngle(TILT, yval); 
 Driver.SetOutput(1); //??
}

  
  delay(SAMPLE_DELAY_MS);
}
