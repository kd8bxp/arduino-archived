//14.04.20 by Tim Kälin
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>

// ultrasonic SR04 sensor pins
#define sensor_A_out 12  
#define sensor_A_in  14
#define sensor_B_out 32
#define sensor_B_in  33

#define LED_PIN      19  // neopixel pin
#define LED_COUNT    27 

#define motA_E 2
#define motA_1 16
#define motA_2 17
#define motB_E 15
#define motB_1 0 
#define motB_2 4

int modus = 0;  // 0 = training until double success then drive   1 = drive  2 = test motors with serial, input one of the outputs 0-5

// setting PWM properties motor
const int freq = 8000;
const int Channel_0 = 0;
const int Channel_1 = 1;
const int Channel_2 = 2;
const int Channel_3 = 3;
const int resolution = 8;   //Resolution 8, 10, 12, 15
int battery = 20;   //calibration for battery voltage/speed. 

const double InitWeights = 0.44;  
double epsilon = 0.0076;  // learning rate 
double mom = 0.024;  
double success = 0.4;   //training loops until failure rate is lower than this
double bias = 1;

int patern = 25;    // number of test paterns in test_in and test_out
int count = 0;     // counter for print serial and view leds in training session
int count_to = 2000;    //cycles to count for serial out

// cells to input from sensor data, done in "sensor"
int input_A_toCell = 0;  
int input_B_toCell = 1;

int s,p = 0;  //  global counters
int out = 0;  //  varable for calculated output

double Error, Err_sum;
double Accum;
double winrate = 0;

const int cell_raster_in = 2 ;    //  inputs
const int cell_raster_out  = 5 ;  //  outputs maybe +1 bias

const int cell_raster_hidden  = cell_raster_out;

double Delta_hidden[cell_raster_hidden];
double Delta_out[cell_raster_hidden];
double cells_in_hidden_weight[cell_raster_in+1] [cell_raster_hidden]  = {   };
double cells_hidden_out_weight[cell_raster_hidden+1][cell_raster_out] = {   };
//double cells_hidden_out_weight[cell_raster_out][cell_raster_hidden] = {   };
double cells_in_hidden_weight_change[cell_raster_in+1] [cell_raster_hidden]  = {   };
double cells_hidden_out_weight_change[cell_raster_hidden+1][cell_raster_out] = {   }; 
//double cells_hidden_out_weight_change[cell_raster_out][cell_raster_hidden] = {   };             
double cells_in[cell_raster_in] = {   }; 
double cells_hidden[cell_raster_hidden] = {   };                
double cells_out[cell_raster_out]   = {   };

// training data sensor in cm, test_in[patern]
double test_in[25][cell_raster_in] = {
  25,14,   //1
  41,33,   //2
  44,44,   //3
  33,41,   //4
  14,25,   //5
  29,22,   //6
  43,33,   //7
  80,90,   //8
  33,43,   //9
  22,29,   //10
  35,26,   //11
  55,35,   //12
  55,55,   //13
  35,55,   //14
  26,35,   //15  
  33,25,   //16
  44,32,   //17
  150,150, //18
  32,44,   //19
  25,33,   //20 
  38,23,   //21
  50,36,   //22
  90,100,  //23
  36,50,   //24
  23,38    //25
};

//  training data supposed output  << < | > >>   ,  test_out[patern]
double test_out[25][cell_raster_out] = {
  1,0,0,0,0,  //1
  0,1,0,0,0,  //2
  0,0,1,0,0,  //3
  0,0,0,1,0,  //4
  0,0,0,0,1,  //5  
  1,0,0,0,0,  //6
  0,1,0,0,0,  //7
  0,0,1,0,0,  //8
  0,0,0,1,0,  //9
  0,0,0,0,1,  //10
  1,0,0,0,0,  //11
  0,1,0,0,0,  //12
  0,0,1,0,0,  //13  
  0,0,0,1,0,  //14 
  0,0,0,0,1,  //15
  1,0,0,0,0,  //16  
  0,1,0,0,0,  //17
  0,0,1,0,0,  //18
  0,0,0,1,0,  //19
  0,0,0,0,1,  //20   
  1,0,0,0,0,  //21
  0,1,0,0,0,  //22 
  0,0,1,0,0,  //23
  0,0,0,1,0,  //24 
  0,0,0,0,1   //25             
};

int incomingByte = 0;

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(230400);
  Serial.println("Start");   
  rst();   // reset all weights to 0, maybe not necessary
  seed(); 
  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.clear(); 
  delay(2);
  pinMode(sensor_A_out, OUTPUT);
  pinMode(sensor_B_out, OUTPUT);
  pinMode(sensor_A_in, INPUT);
  pinMode(sensor_B_in, INPUT);
  pinMode(motA_E, OUTPUT);
  pinMode(motA_1, OUTPUT);  
  pinMode(motA_2, OUTPUT);
  pinMode(motB_E, OUTPUT);  
  pinMode(motB_1, OUTPUT);
  pinMode(motB_2, OUTPUT);
  ledcSetup(Channel_0, freq, resolution);
  ledcAttachPin(motA_E, Channel_0);
  ledcSetup(Channel_1, freq, resolution);
  ledcAttachPin(motB_E, Channel_1);
}

void loop() {  
  if(modus == 0)     {  training();  } 
  else if(modus == 1){  sensor(); load(); outvalue(); leds(); serial_print_out();  drive(); }
  else if(modus == 2){  for(out = 0; out < 5; out++) {serial_in(); leds_out(); drive(); delay(2000);  }
  }
}


//backup working testpatern
/*double test_in[25][cell_raster_in] = {
  24,16,  //1
  50,32,  //2
  44,44,  //3
  32,50,  //4
  16,24,  //5
  40,21,  //6
  50,36,  //7
  100,120,  //8
  36,50, //9
  21,40,  //10
  30,24,  //11
  60,34,  //12
  50,60,  // 13
  34,60,  //14
  24,30,  //15  
  33,25,  //16
  44,35,  //17
  60,50,  //18
  35,44,  //19
  25,33,  //20 
  38,23,  //21
  45,36,  //22
  100,120,  //23
  36,45,  //24
  23,38   //25
};
double test_out[25][cell_raster_out] = {
//<< < | > >> !  
  1,0,0,0,0,  //1
  0,1,0,0,0,  //2
  0,0,1,0,0, //3
  0,0,0,1,0,  //4
  0,0,0,0,1,  //5  
  1,0,0,0,0,  //6
  0,1,0,0,0,  //7
  0,0,1,0,0,  //8
  0,0,0,1,0,  //9
  0,0,0,0,1,  //10
  1,0,0,0,0,  //11
  0,1,0,0,0,  //12
  0,0,1,0,0,  //13  
  0,0,0,1,0,  //14 
  0,0,0,0,1,  //15
  1,0,0,0,0,  //16  
  0,1,0,0,0,  //17
  0,0,1,0,0,  //18
  0,0,0,1,0,  //19
  0,0,0,0,1,  //20   
  1,0,0,0,0,  //21
  0,1,0,0,0,  //22 
  0,0,1,0,0,  //23
  0,0,0,1,0,   //24 
  0,0,0,0,1   //25             
};
*/
