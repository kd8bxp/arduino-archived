/************************************************************************

  M5StackFire Discovery simple Spectrum example

  The microphone signal is sampled and a frequency analysis is performed.


  Please install the arduinoFFT library.
  You will find it in the library manager or you can get it from github:
  https://github.com/kosme/arduinoFFT


  M5StackFire         September 2018, ChrisMicro

************************************************************************/
#include "src/M5Stack/M5Stack.h"
//#include "src/M5Unified/M5Unified.h"
#include "src/M5GFX/M5UnitRCA.h"
#include "arduinoFFT.h"
//#include <WiFi.h>
#include <driver/dac.h>
#include "defaultEye.h"     // Standard human-ish hazel eye

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
M5UnitRCA gfx_rca;

#define M5STACKFIRE_MICROPHONE_PIN 34
#define M5STACKFIRE_SPEAKER_PIN    25  // speaker DAC, only 8 Bit

#define HORIZONTAL_RESOLUTION 216 //320
#define VERTICAL_RESOLUTION   100 //72 //144 //240
#define POSITION_OFFSET_Y     20 //125
#define SIGNAL_LENGTH         512

double oldSignal[SIGNAL_LENGTH];
double adcBuffer[SIGNAL_LENGTH];
double vImag[SIGNAL_LENGTH];

#define SAMPLINGFREQUENCY   40000
#define SAMPLING_TIME_US    round(1000000 *  (1.0 /SAMPLINGFREQUENCY))
#define ANALOG_SIGNAL_INPUT M5STACKFIRE_MICROPHONE_PIN
#define NOBLINK 0     // Not currently engaged in a blink
#define ENBLINK 1     // Eyelid is currently closing
#define DEBLINK 2     // Eyelid is currently opening

//#define DISPLAY_DC      D3 // Data/command pin for BOTH displays
//#define DISPLAY_RESET   D4 // Reset pin for BOTH displays
#define SELECT_L_PIN    38 // LEFT  eye chip select pin
#define SELECT_R_PIN    39 // RIGHT eye chip select pin
//#define TRACKING          // If enabled, eyelid tracks pupil
#define IRIS_PIN       37 // Photocell or potentiometer (else auto iris)
#define IRIS_PIN_FLIP  37 // If set, reverse reading from dial/photocell
#define IRIS_SMOOTH       // If enabled, filter input from IRIS_PIN
#define IRIS_MIN      140 // Clip lower analogRead() range from IRIS_PIN
#define IRIS_MAX      260 // Clip upper "
#define WINK_L_PIN     39 // Pin for LEFT eye wink button
#define BLINK_PIN      38 // Pin for blink button (BOTH eyes)
#define WINK_R_PIN     37 // Pin for RIGHT eye wink button
#define AUTOBLINK         // If enabled, eyes blink autonomously
 

typedef struct {
  int8_t   pin;       // Optional button here for indiv. wink
  uint8_t  state;     // NOBLINK/ENBLINK/DEBLINK
  int32_t  duration;  // Duration of blink state (micros)
  uint32_t startTime; // Time (micros) of last state change
} eyeBlink;
 
struct {
  uint8_t     cs;     // Chip select pin
  eyeBlink    blink;  // Current blink state
} eye[] = { SELECT_L_PIN, { WINK_L_PIN, NOBLINK },
            SELECT_R_PIN, { WINK_R_PIN, NOBLINK } 
};
 
#define NUM_EYES 2    // pcs eye : 1 = 1 eye, 2 = 2 eye
uint32_t fstart = 0;  // start time to improve frame rate calculation at startup

void task1(void* pvParameters) {  // Define the tasks to be executed in
                                  // thread 1.  定义线程1内要执行的任务
    while (1) {  // Keep the thread running.  使线程一直运行
           #if defined(IRIS_PIN) && (IRIS_PIN >= 0)   // Interactive iris
  uint16_t v = 512; //analogRead(IRIS_PIN);// Raw dial/photocell reading
#ifdef IRIS_PIN_FLIP
  v = 1023 - v;
#endif
  v = map(v, 0, 1023, IRIS_MIN, IRIS_MAX); // Scale to iris range
#ifdef IRIS_SMOOTH // Filter input (gradual motion)
  static uint16_t irisValue = (IRIS_MIN + IRIS_MAX) / 2;
  irisValue = ((irisValue * 15) + v) / 16;
  frame(irisValue);
#else  // Unfiltered (immediate motion)
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
/*  auto cfg         = M5.config();
    cfg.external_spk = true;
    cfg.internal_spk = false;
  */  
    M5.begin();
    M5.Power.begin();
    // WiFi.mode(WIFI_OFF);
    // btStop();
    //dacWrite(M5STACKFIRE_SPEAKER_PIN,0);  // make sure that the speaker is quite
    dac_output_disable(DAC_CHANNEL_1);
    M5.Lcd.begin();
    M5.Lcd.fillScreen(BLACK);
    //M5.Lcd.fillRect(10, 1, 150, 160, BLACK);
    //M5.Lcd.setCursor(0, 0);
    //M5.Lcd.setTextColor(GREEN);  // M5.Lcd.setTextSize(3);
    //M5.Lcd.setTextSize(1);

    //M5.Lcd.println("analog input MIC");
    //M5.Lcd.print("max. frequency: ");
    //M5.Lcd.print(SAMPLINGFREQUENCY / 2);
    //M5.Lcd.println(" Hz");
    //M5.Lcd.setTextSize(2);
/*    if (M5.getBoard() == m5gfx::boards::board_M5StackCore2) {
         {
             auto spk_cfg        = M5.Speaker.config();
             spk_cfg.i2s_port    = I2S_NUM_1;
             spk_cfg.sample_rate = 96000;

             spk_cfg.pin_data_out = 2;
             spk_cfg.pin_bck      = 19;
             spk_cfg.pin_ws       = 0;  // LRCK

             spk_cfg.stereo        = true;
             spk_cfg.buzzer        = false;
             spk_cfg.use_dac       = false;
             spk_cfg.magnification = 16;
             M5.Speaker.config(spk_cfg);
         }
     } else {
         auto spk_cfg        = M5.Speaker.config();
         spk_cfg.i2s_port    = I2S_NUM_1;
         spk_cfg.sample_rate = 96000;

         spk_cfg.pin_data_out = 15;
         spk_cfg.pin_bck      = 13;
         spk_cfg.pin_ws       = 0;  // LRCK

         spk_cfg.stereo        = true;
         spk_cfg.buzzer        = false;
         spk_cfg.use_dac       = false;
         spk_cfg.magnification = 16;
         M5.Speaker.config(spk_cfg);
     }
*/
     M5.Speaker.begin();
     M5.Speaker.setVolume(128);
//     M5.Speaker.setChannelVolume(0, 255);
//     M5.Speaker.setChannelVolume(1, 255);

    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();

fstart = millis()-1; // Subtract 1 to avoid divide by zero later
  pinMode(37, INPUT);pinMode(38, INPUT);pinMode(39, INPUT);
  xTaskCreatePinnedToCore( task1, "task1", 4096, NULL, 1, NULL, 0);  // Core where the task should run. 
}

// EYE-RENDERING FUNCTION --------------------------------------------------
#define BUFFER_SIZE 256 // 64 to 512 seems optimum = 30 fps for default eye
void drawEye( // Renders one eye.  Inputs must be pre-clipped & valid.
  // Use native 32 bit variables where possible as this is 10% faster!
  uint8_t  e,      // Eye array index; 0 or 1 for left/right：0/1 = left/right
  uint32_t iScale, // Scale factor for iris
  uint32_t scleraX,// First pixel X offset into sclera image：Center of right and left
  uint32_t scleraY,// First pixel Y offset into sclera image：Upper and lower centers
  uint32_t uT,     // Upper eyelid threshold value：Up and down
  uint32_t lT)     // Lower eyelid threshold value：Up and down
  {
  uint32_t screenX, screenY, scleraXsave;
  uint32_t screenX_; // add macsbug
  int32_t  irisX, irisY;
  uint32_t p, a;
  uint32_t d;
  uint32_t pixels = 0;
  uint16_t pbuffer[BUFFER_SIZE]; // This one needs to be 16 bit
  // Set up raw pixel dump to entire screen.  Although such writes can wrap
  // around automatically from end of rect back to beginning, the region is
  // reset on each frame here in case of an SPI glitch.
  if (e == 0){ M5.Lcd.setAddrWindow (  1,0,128,128);} // set left  window
  if (e == 1){ M5.Lcd.setAddrWindow (191,0,128,128);} // set right window
  // Now just issue raw 16-bit values for every pixel...
  scleraXsave = scleraX;
  irisY = scleraY - (SCLERA_HEIGHT - IRIS_HEIGHT ) / 2;  // add : rev.3 2019.06.26
  for(screenY=0; screenY<SCREEN_HEIGHT; screenY++, scleraY++, irisY++) {
    scleraX = scleraXsave;
    irisX = scleraXsave - (SCLERA_WIDTH - IRIS_WIDTH) / 2;
    for(screenX=0; screenX<SCREEN_WIDTH; screenX++, scleraX++, irisX++) {
      if (e == 1){screenX_ =                    screenX;}
      if (e == 0){screenX_ = SCREEN_WIDTH - 1 - screenX;}
      if((pgm_read_byte(lower+screenY * SCREEN_WIDTH + screenX_) <= lT) ||
         (pgm_read_byte(upper+screenY * SCREEN_WIDTH + screenX_) <= uT)){
        p = 0;  //Covered by eyelid
      } else if((irisY < 0) || (irisY >= IRIS_HEIGHT) ||
                (irisX < 0) || (irisX >= IRIS_WIDTH)) { // In sclera
        p = pgm_read_word(sclera + scleraY * SCLERA_WIDTH + scleraX);
      } else {                                          // Maybe iris...
        p = pgm_read_word(polar+irisY*IRIS_WIDTH+irisX);// Polar angle/dist
        d = (iScale * (p & 0x7F)) / 128;                // Distance (Y)
        if(d < IRIS_MAP_HEIGHT) {                       // Within iris area
          a = (IRIS_MAP_WIDTH * (p >> 7)) / 512;        // Angle (X)
          p = pgm_read_word(iris+d*IRIS_MAP_WIDTH+a);   // Pixel = iris
        } else {                                        // Not in iris
          p = pgm_read_word(sclera+scleraY*SCLERA_WIDTH+scleraX);//Pixel=clear
        }
      }
      *(pbuffer + pixels++) = p>>8 | p<<8;
      if (pixels >= BUFFER_SIZE){yield();
          M5.Lcd.pushColors((uint8_t*)pbuffer,pixels*2); pixels=0; //drawEye    
      }
    }
  }
   if (pixels) { M5.Lcd.pushColors(pbuffer, pixels); pixels = 0;}
}
 
// EYE ANIMATION -----------------------------------------------------------
const uint8_t ease[] = { // Ease in/out curve for eye movements 3*t^2-2*t^3
    0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  3,   // T
    3,  3,  4,  4,  4,  5,  5,  6,  6,  7,  7,  8,  9,  9, 10, 10,   // h
   11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 22, 23,   // x
   24, 25, 26, 27, 27, 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39,   // 2
   40, 41, 42, 44, 45, 46, 47, 48, 50, 51, 52, 53, 54, 56, 57, 58,   // A
   60, 61, 62, 63, 65, 66, 67, 69, 70, 72, 73, 74, 76, 77, 78, 80,   // l
   81, 83, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 98,100,101,103,   // e
  104,106,107,109,110,112,113,115,116,118,119,121,122,124,125,127,   // c
  128,130,131,133,134,136,137,139,140,142,143,145,146,148,149,151,   // J
  152,154,155,157,158,159,161,162,164,165,167,168,170,171,172,174,   // a
  175,177,178,179,181,182,183,185,186,188,189,190,192,193,194,195,   // c
  197,198,199,201,202,203,204,205,207,208,209,210,211,213,214,215,   // o
  216,217,218,219,220,221,222,224,225,226,227,228,228,229,230,231,   // b
  232,233,234,235,236,237,237,238,239,240,240,241,242,243,243,244,   // s
  245,245,246,246,247,248,248,249,249,250,250,251,251,251,252,252,   // o
  252,253,253,253,254,254,254,254,254,255,255,255,255,255,255,255 }; // n
 
#ifdef AUTOBLINK
uint32_t timeOfLastBlink = 0L, timeToNextBlink = 0L;
#endif
 
void frame( // Process motion for a single frame of left or right eye
  uint32_t        iScale) {     // Iris scale (0-1023) passed in
  static uint32_t frames   = 0; // Used in frame rate calculation
  static uint8_t  eyeIndex = 0; // eye[] array counter
  int32_t         eyeX, eyeY;
  uint32_t        t = micros(); // Time at start of function
  Serial.print((++frames * 1000) / (millis() - fstart));
  Serial.println("fps");        // Show frame rate
  if(++eyeIndex >= NUM_EYES) eyeIndex = 0;// Cycle through eyes, 1 per call
  // Autonomous X/Y eye motion
  // Periodically initiates motion to a new random point, random speed,
  // holds there for random period until next motion.
  static boolean  eyeInMotion      = false;
  static int32_t  eyeOldX=512, eyeOldY=512, eyeNewX=512, eyeNewY=512;
  static uint32_t eyeMoveStartTime = 0L;
  static int32_t  eyeMoveDuration  = 0L;
  int32_t dt = t - eyeMoveStartTime;      // uS elapsed since last eye event
  if(eyeInMotion) {                       // Currently moving?
    if(dt >= eyeMoveDuration) {           // Time up?  Destination reached.
      eyeInMotion      = false;           // Stop moving
      eyeMoveDuration  = random(3000000L);// 0-3 sec stop
      eyeMoveStartTime = t;               // Save initial time of stop
      eyeX = eyeOldX = eyeNewX;           // Save position
      eyeY = eyeOldY = eyeNewY;
    } else { // Move time's not yet fully elapsed -- interpolate position
      int16_t e = ease[255 * dt / eyeMoveDuration] + 1;   // Ease curve
      eyeX = eyeOldX + (((eyeNewX - eyeOldX) * e) / 256); // Interp X
      eyeY = eyeOldY + (((eyeNewY - eyeOldY) * e) / 256); // and Y
    }
  } else {                                // Eye stopped
    eyeX = eyeOldX;
    eyeY = eyeOldY;
    if(dt > eyeMoveDuration) {            // Time up?  Begin new move.
      int16_t  dx, dy;
      uint32_t d;
      do {                                // Pick new dest in circle
        eyeNewX = random(1024);
        eyeNewY = random(1024);
        dx      = (eyeNewX * 2) - 1023;
        dy      = (eyeNewY * 2) - 1023;
      } while((d = (dx * dx + dy * dy)) > (1023 * 1023)); // Keep trying
      eyeMoveDuration  = random(50000,150000); // ~1/14sec
                       //random(72000,144000); // ~1/ 7sec
      eyeMoveStartTime = t;               // Save initial time of move
      eyeInMotion      = true;            // Start move on next frame
    }
  }
 
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
  if (eyeIndex == 0){eyeX = map(eyeX,1023,0,0,SCLERA_WIDTH - 128);}// left
  if (eyeIndex == 1){eyeX = map(eyeX,0,1023,0,SCLERA_WIDTH - 128);}// right
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
 

void showSignal() {
    int n;

    int oldx;
    int oldy;
    int oldSig;
    int x, y;

    for (n = 0; n < SIGNAL_LENGTH / 2; n++) {
        x = n;
        y = map(adcBuffer[n], 0, 512, 85 /*VERTICAL_RESOLUTION*/, POSITION_OFFSET_Y);  //0,512

        if (n > 0) {
            // delete old line element
            //M5.Lcd.drawLine(oldx, oldSig, x, oldSignal[n], BLACK);
            //M5.Lcd.drawLine(oldx, oldy, x, y, GREEN);
            gfx_rca.drawLine(oldx, oldSig, x, oldSignal[n], BLACK);
            gfx_rca.drawLine(oldx, oldy, x , y, GREEN);
            
        }
        oldx         = x;
        oldy         = y;
        oldSig       = oldSignal[n];
        oldSignal[n] = y;
    }
}

double AdcMeanValue = 0;

void loop(void) {
    int n;
    uint32_t nextTime = 0;

    // record signal
    for (n = 1; n < SIGNAL_LENGTH; n++) {
        double v = analogRead(ANALOG_SIGNAL_INPUT);
        AdcMeanValue += (v - AdcMeanValue) * 1.6; //0.001;
        adcBuffer[n] = v - AdcMeanValue;

        // wait for next sample
        while (micros() < nextTime)
            ;
        nextTime = micros() + SAMPLING_TIME_US;
    }

FFT.DCRemoval(adcBuffer, SIGNAL_LENGTH);
    //FFT.Windowing(adcBuffer, SIGNAL_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);                                /* Weigh data */
    FFT.Windowing(adcBuffer, SIGNAL_LENGTH, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);                                /* Weigh data */
    FFT.Compute(adcBuffer, vImag, SIGNAL_LENGTH, FFT_FORWARD); /* Compute FFT */
    FFT.ComplexToMagnitude(adcBuffer, vImag, SIGNAL_LENGTH); /* Compute magnitudes */
    
    

    
  
  
    // int x = FFT.MajorPeak(adcBuffer, SIGNAL_LENGTH, 1000000UL /
    // SAMPLING_TIME_US);//SAMPLINGFREQUENCY
    int x = FFT.MajorPeak(adcBuffer, SIGNAL_LENGTH, SAMPLINGFREQUENCY);

    int maxAmplitudeDB = 0;
    for (n = 1; n < SIGNAL_LENGTH; n++) {
        int a = log10(adcBuffer[n]) * 20 -
                54.186;  // convert amplitude to dB scale, dB relative to
                         // log10(512samples)*20=54.186dB
        if (a > maxAmplitudeDB) {maxAmplitudeDB = a;
        adcBuffer[n] = (a + 30) * 5;  // scale for TFT display
        
        } else {adcBuffer[n] = (a + 30) * 2;}
        vImag[n]     = 0;             // clear imaginary part
    }

    showSignal();

    //M5.Lcd.fillRect(200, 0, 119, 40, BLUE);
    //M5.Lcd.setCursor(210, 1);
    //M5.Lcd.print(x);
    //M5.Lcd.print(" Hz");
    //M5.Lcd.setCursor(210, 21);
    //M5.Lcd.print(maxAmplitudeDB);
    //M5.Lcd.print(" dB");


}
