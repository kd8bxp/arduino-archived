#include "src/M5StickC/M5StickC.h"
#include "CBACK_DRIVER.h"
#include "defaultEye.h"
#include <Wire.h>
#include "person_sensor.h"

//defines for servos
#define PAN 1  //cback port number
#define TILT 2 //cback port number

//center servo
#define TILT_UP 125 //180 //0 is flat down
#define PAN_CENTER 90 //0 is point to left, 180 is point to right

//defines for Uncanny Eyes
#define SELECT_L_PIN    37 // LEFT  eye chip select pin // rev M5StickC 
#define TRACKING          // If enabled, eyelid tracks pupil
#define IRIS_PIN       37 // Photocell or potentiometer (else auto iris)
#define IRIS_PIN_FLIP  37 // If set, reverse reading from dial/photocell
#define IRIS_SMOOTH       // If enabled, filter input from IRIS_PIN
#define IRIS_MIN      140 // Clip lower analogRead() range from IRIS_PIN
#define IRIS_MAX      160//260 // Clip upper "
#define WINK_L_PIN     37 // Pin for LEFT eye wink button // rev M5StickC 
#define BLINK_PIN      37 // Pin for blink button (BOTH eyes) // rev M5StickC 
#define AUTOBLINK         // If enabled, eyes blink autonomously
 
// Probably don't need to edit any config below this line, -----------------
// unless building a single-eye project (pendant, etc.), in which case one
// of the two elements in the eye[] array further down can be commented out.
// Eye blinks are a tiny 3-state machine.  Per-eye allows winks + blinks.
#define NOBLINK 0     // Not currently engaged in a blink
#define ENBLINK 1     // Eyelid is currently closing
#define DEBLINK 2     // Eyelid is currently opening
#define NUM_EYES 1


typedef struct {
  int8_t   pin;       // Optional button here for indiv. wink
  uint8_t  state;     // NOBLINK/ENBLINK/DEBLINK
  int32_t  duration;  // Duration of blink state (micros)
  uint32_t startTime; // Time (micros) of last state change
} eyeBlink;
 
struct {
  uint8_t     cs;     // Chip select pin
  eyeBlink    blink;  // Current blink state
} eye[] = { SELECT_L_PIN, { WINK_L_PIN, NOBLINK }//, // comment out M5StickC 
           // SELECT_R_PIN, { WINK_R_PIN, NOBLINK }  // comment out M5StickC 
};

//variables

//Uncanny Eyes
uint32_t fstart = 0;  // start time to improve frame rate calculation at startup

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

void task1(void* pvParameters) {  // Define the tasks to be executed in
                                  // thread 1.  定义线程1内要执行的任务
    while (1) {  // Keep the thread running.  使线程一直运行
         #if defined(IRIS_PIN) && (IRIS_PIN >= 0) // Interactive iris
  uint16_t v = 512; //analogRead(IRIS_PIN);// Raw dial/photocell reading
#ifdef IRIS_PIN_FLIP
  v = 1023 - v;
#endif
  v = map(v, 0, 1023, IRIS_MIN, IRIS_MAX); // Scale to iris range
#ifdef IRIS_SMOOTH // Filter input (gradual motion)
  static uint16_t irisValue = (IRIS_MIN + IRIS_MAX) / 2;
  irisValue = ((irisValue * 15) + v) / 16;
  frame(irisValue);
#else // Unfiltered (immediate motion)
  frame(v);
#endif // IRIS_SMOOTH
#else  // Autonomous iris scaling -- invoke recursive function
  newIris = random(IRIS_MIN, IRIS_MAX);
  split(oldIris, newIris, micros(), 10000000L, IRIS_MAX - IRIS_MIN);
  oldIris = newIris;
#endif // IRIS_PIN
delay(1);
    }
}

void setup() {
   M5.begin();
   //Wire1.begin(0,26);
   Wire.begin();
   Serial.begin(115200);
   M5.Lcd.fillScreen(TFT_BLACK);
   M5.Lcd.setRotation(1);
   fstart = millis()-1; // Subtract 1 to avoid divide by zero later
   pinMode(37, INPUT);//pinMode(38, INPUT);pinMode(39, INPUT);
   xTaskCreatePinnedToCore( task1, "task1", 4096, NULL, 1, NULL, 0);  // Core where the task should run. 
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

// EYE-RENDERING FUNCTION --------------------------------------------------
#define BUFFER_SIZE 256 // 64 to 512 seems optimum = 30 fps for default eye
void drawEye( // Renders one eye.  Inputs must be pre-clipped & valid.
  // Use native 32 bit variables where possible as this is 10% faster!
  uint8_t  e,       // Eye array index; 0 or 1 for left/right
  uint32_t iScale,  // Scale factor for iris
  uint32_t scleraX, // First pixel X offset into sclera image
  uint32_t scleraY, // First pixel Y offset into sclera image
  uint32_t uT,      // Upper eyelid threshold value
  uint32_t lT) {    // Lower eyelid threshold value
  uint32_t screenX, screenY, scleraXsave;
  int32_t  irisX, irisY;
  uint32_t p, a;
  uint32_t d;
  uint32_t pixels = 0;
  uint16_t pbuffer[BUFFER_SIZE]; // This one needs to be 16 bit
  // Set up raw pixel dump to entire screen.  Although such writes can wrap
  // around automatically from end of rect back to beginning, the region is
  // reset on each frame here in case of an SPI glitch.
  // M5.Lcd.setAddrWindow(x axis, y axis, Horizontal width, Vertical width);
  M5.Lcd.setAddrWindow ( 16, -22, 128, 128 ); // rev M5StickC 
  
  // Now just issue raw 16-bit values for every pixel...
  scleraXsave = scleraX; // Save initial X value to reset on each line
  irisY       = scleraY - (SCLERA_HEIGHT - IRIS_HEIGHT) / 2;
  for(screenY=0; screenY<SCREEN_HEIGHT; screenY++, scleraY++, irisY++) {
    scleraX = scleraXsave;
    irisX   = scleraXsave - (SCLERA_WIDTH - IRIS_WIDTH) / 2;
    for(screenX=0; screenX<SCREEN_WIDTH; screenX++, scleraX++, irisX++) {
      if((pgm_read_byte(lower + screenY * SCREEN_WIDTH + screenX) <= lT) ||
         (pgm_read_byte(upper + screenY * SCREEN_WIDTH + screenX) <= uT)){//Covered by eyelid
        p = 0;
      } else if((irisY < 0) || (irisY >= IRIS_HEIGHT) ||
                (irisX < 0) || (irisX >= IRIS_WIDTH)) { // In sclera
        p = pgm_read_word(sclera + scleraY * SCLERA_WIDTH + scleraX);
      } else {                                          // Maybe iris...
        p = pgm_read_word(polar + irisY * IRIS_WIDTH + irisX);// Polar angle/dist
        d = (iScale * (p & 0x7F)) / 128;                // Distance (Y)
        if(d < IRIS_MAP_HEIGHT) {                       // Within iris area
          a = (IRIS_MAP_WIDTH * (p >> 7)) / 512;        // Angle (X)
          p = pgm_read_word(iris + d * IRIS_MAP_WIDTH + a);// Pixel = iris
        } else {                                        // Not in iris
          p = pgm_read_word(sclera + scleraY * SCLERA_WIDTH + scleraX);//Pixel= clear
        }
      }
      *(pbuffer + pixels++) = p>>8 | p<<8;
      if (pixels >= BUFFER_SIZE){yield();
          M5.Lcd.pushColors((uint8_t*)pbuffer,pixels*2);pixels=0;}//drawEye
    }
  }
   if (pixels) { M5.Lcd.pushColors(pbuffer, pixels); pixels = 0;}
}

 
#ifdef AUTOBLINK
uint32_t timeOfLastBlink = 0L, timeToNextBlink = 0L;
#endif
 
void frame( // Process motion for a single frame of left or right eye
  uint32_t        iScale) {     // Iris scale (0-1023) passed in
  static uint32_t frames   = 0; // Used in frame rate calculation
  static uint8_t  eyeIndex = 0; // eye[] array counter
  int32_t         eyeX, eyeY;
  uint32_t        t = micros(); // Time at start of function
  
  if(++eyeIndex >= NUM_EYES) eyeIndex = 0; // Cycle through eyes, 1 per call
 
  eyeX = 412; 
  eyeY = 412; 
  
  
// Blinking
#ifdef AUTOBLINK
  // Similar to the autonomous eye movement above -- blink start times
  // and durations are random (within ranges).
  if((t - timeOfLastBlink) >= timeToNextBlink) { // Start new blink?
    timeOfLastBlink = t;
    uint32_t blinkDuration = random(36000, 72000); // ~1/28 - ~1/14 sec
    // Set up durations for both eyes (if not already winking)
    for(uint8_t e=0; e<NUM_EYES; e++) {
      if(eye[e].blink.state == NOBLINK) {
         eye[e].blink.state     = ENBLINK;
         eye[e].blink.startTime = t;
         eye[e].blink.duration  = blinkDuration;
      }
    }
    timeToNextBlink = blinkDuration * 3 + random(4000000);
  }
#endif
 
  if(eye[eyeIndex].blink.state) { // Eye currently blinking?
    // Check if current blink state time has elapsed
    if((t - eye[eyeIndex].blink.startTime) >= eye[eyeIndex].blink.duration){
      // Yes -- increment blink state, unless...
      if((eye[eyeIndex].blink.state == ENBLINK) && // Enblinking and...
        ((digitalRead(BLINK_PIN) == LOW) ||        // blink or wink held...
          digitalRead(eye[eyeIndex].blink.pin) == LOW)) {
        // Don't advance state yet -- eye is held closed instead
      } else { // No buttons, or other state...
        if(++eye[eyeIndex].blink.state > DEBLINK) {// Deblinking finished?
          eye[eyeIndex].blink.state = NOBLINK;     // No longer blinking
        } else { // Advancing from ENBLINK to DEBLINK mode
          eye[eyeIndex].blink.duration *= 2;//DEBLINK is 1/2 ENBLINK speed
          eye[eyeIndex].blink.startTime = t;
        }
      }
    }
  } else { // Not currently blinking...check buttons!
    if(digitalRead(BLINK_PIN) == LOW) {
      // Manually-initiated blinks have random durations like auto-blink
      uint32_t blinkDuration = random(36000, 72000);
      for(uint8_t e=0; e<NUM_EYES; e++) {
        if(eye[e].blink.state    == NOBLINK) {
           eye[e].blink.state     = ENBLINK;
           eye[e].blink.startTime = t;
           eye[e].blink.duration  = blinkDuration;
        }
      }
    } else if(digitalRead(eye[eyeIndex].blink.pin) == LOW) { // Wink!
      eye[eyeIndex].blink.state     = ENBLINK;
      eye[eyeIndex].blink.startTime = t;
      eye[eyeIndex].blink.duration  = random(45000, 90000);
    }
  }
 
  // Process motion, blinking and iris scale into renderable values
  // Iris scaling: remap from 0-1023 input to iris map height pixel units
  iScale = ((IRIS_MAP_HEIGHT + 1) * 1024) /
           (1024 - (iScale * (IRIS_MAP_HEIGHT - 1) / IRIS_MAP_HEIGHT));
 
  // Scale eye X/Y positions (0-1023) to pixel units used by drawEye()
  eyeX = map(eyeX, 0, 1023, 0, SCLERA_WIDTH  - 128);
  eyeY = map(eyeY, 0, 1023, 0, SCLERA_HEIGHT - 128);
  if(eyeIndex == 1) eyeX = (SCLERA_WIDTH - 128) - eyeX; // Mirrored display
 
  // Horizontal position is offset so that eyes are very slightly crossed
  // to appear fixated (converged) at a conversational distance.  Number
  // here was extracted from my posterior and not mathematically based.
  // I suppose one could get all clever with a range sensor, but for now...
  eyeX += 4;
  if(eyeX > (SCLERA_WIDTH - 128)) eyeX = (SCLERA_WIDTH - 128);
 
  // Eyelids are rendered using a brightness threshold image.  This same
  // map can be used to simplify another problem: making the upper eyelid
  // track the pupil (eyes tend to open only as much as needed -- e.g. look
  // down and the upper eyelid drops).  Just sample a point in the upper
  // lid map slightly above the pupil to determine the rendering threshold.
  static uint8_t uThreshold = 128;
  uint8_t        lThreshold, n;
 
#ifdef TRACKING
  int16_t sampleX = SCLERA_WIDTH  / 2 - (eyeX / 2), // Reduce X influence
          sampleY = SCLERA_HEIGHT / 2 - (eyeY + IRIS_HEIGHT / 4);
  // Eyelid is slightly asymmetrical, so two readings are taken, averaged
  if(sampleY < 0) n = 0;
  else n = (pgm_read_byte(upper + sampleY * SCREEN_WIDTH + sampleX) +
   pgm_read_byte(upper+sampleY*SCREEN_WIDTH+(SCREEN_WIDTH-1-sampleX)))/2;
  uThreshold = (uThreshold * 3 + n) / 4; // Filter/soften motion
  // Lower eyelid doesn't track the same way, but seems to be pulled upward
  // by tension from the upper lid.
  lThreshold = 254 - uThreshold;
#else // No tracking -- eyelids full open unless blink modifies them
  uThreshold = lThreshold = 0;
#endif
  // The upper/lower thresholds are then scaled relative to the current
  // blink position so that blinks work together with pupil tracking.
  if(eye[eyeIndex].blink.state) { // Eye currently blinking?
    uint32_t s = (t - eye[eyeIndex].blink.startTime);
    if(s >= eye[eyeIndex].blink.duration) s = 255;// At or past blink end
    else s = 255 * s / eye[eyeIndex].blink.duration; // Mid-blink
    s          = (eye[eyeIndex].blink.state == DEBLINK) ? 1 + s : 256 - s;
    n          = (uThreshold * s + 254 * (257 - s)) / 256;
    lThreshold = (lThreshold * s + 254 * (257 - s)) / 256;
  } else {
    n          = uThreshold;
  }
  // Pass all the derived values to the eye-rendering function:
  drawEye(eyeIndex, iScale, eyeX, eyeY, n, lThreshold);
}
 
// AUTONOMOUS IRIS SCALING (if no photocell or dial) -----------------------
#if !defined(IRIS_PIN) || (IRIS_PIN < 0)
// Autonomous iris motion uses a fractal behavior to similate both the major
// reaction of the eye plus the continuous smaller adjustments that occur.
uint16_t oldIris = (IRIS_MIN + IRIS_MAX) / 2, newIris;
 
void split( // Subdivides motion path into two sub-paths w/randimization
  int16_t  startValue, // Iris scale value (IRIS_MIN to IRIS_MAX) at start
  int16_t  endValue,   // Iris scale value at end
  uint32_t startTime,  // micros() at start
  int32_t  duration,   // Start-to-end time, in microseconds
  int16_t  range) {    // Allowable scale value variance when subdividing
 
  if(range >= 8) {     // Limit subdvision count, because recursion
    range    /= 2;     // Split range & time in half for subdivision,
    duration /= 2;     // then pick random center point within range:
    int16_t  midValue = (startValue + endValue - range)/2+random(range);
    uint32_t midTime  = startTime + duration;
    split(startValue, midValue, startTime, duration, range);//First half
    split(midValue  , endValue, midTime  , duration, range);//Second half
  } else {             // No more subdivisons, do iris motion...
    int32_t dt;        // Time (micros) since start of motion
    int16_t v;         // Interim value
    while((dt = (micros() - startTime)) < duration) {
      v = startValue + (((endValue - startValue) * dt) / duration);
      if(v < IRIS_MIN)      v = IRIS_MIN; // Clip just in case
      else if(v > IRIS_MAX) v = IRIS_MAX;
      frame(v);        // Draw frame w/interim iris scale value
    }
  }
}
#endif // !IRIS_PIN
 
