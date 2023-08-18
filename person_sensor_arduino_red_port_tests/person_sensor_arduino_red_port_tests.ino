#include <Wire.h>
#include "src/M5Stack/M5Stack.h"
#include "person_sensor.h"

// How long to wait between reading the sensor. The sensor can be read as
// frequently as you like, but the results only change at about 5FPS, so
// waiting for 200ms is reasonable.
const int32_t SAMPLE_DELAY_MS = 200;

int box_center_x;
int box_center_y;
int prev_center_x = 1;

int xval;
int yval;
int trimval;
int current_xval;
int prev_xval = 512;

void setup() {
  // You need to make sure you call Wire.begin() in setup, or the I2C access
  // below will fail.
  M5.begin(true,false,true,true);
  //Wire.begin(); //(0,26);
  Serial.begin(115200);
}

void loop() {
  person_sensor_results_t results = {};
  // Perform a read action on the I2C address of the sensor to get the
  // current face information detected.
  if (!person_sensor_read(&results)) {
    Serial.println("No person sensor results found on the i2c bus");
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

    // SERVO POSITIONING
    xval = ((box_center_x * -4) + 1023)*1.2;   // translate sensor information into servo info
    yval = ((box_center_y * -4) + 1023)*1.2;
Serial.print("X, Y: "); Serial.print(xval); Serial.print(","); Serial.println(yval);
}

  
  delay(SAMPLE_DELAY_MS);
}
