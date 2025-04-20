// Based on AI_delta_float_v1.2
//14.04.20 by Tim Kälin
 
/*
 * Modified to work with Makeblock Starter Robot, unfortuntally the controller on this robot is UNO based, so not enough memory.
 * 
 * LeRoy Miller, Aug 23, 2022

 Sketch uses 13202 bytes (40%) of program storage space. Maximum is 32256 bytes.
Global variables use 2284 bytes (111%) of dynamic memory, leaving -236 bytes for local variables. Maximum is 2048 bytes.
data section exceeds available space in board
Not enough memory; see https://support.arduino.cc/hc/en-us/articles/360013825179 for tips on reducing your footprint.
Error compiling for board Arduino Uno.

It compiles if using an Arduino Mega2560 - (Makeblock Auriga)

Changed to use the MEShield, on the Mega2560 - this has a draw back of not having motor drivers
I think the best solution to this is too add a 2nd microcontroller.

*/

//More Info: https://www.the-diy-life.com/running-an-artificial-neural-network-on-an-arduino-uno/
//https://www.youtube.com/watch?v=SL0z8HkPOmg

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Wire.h>
//#include "src/makeblock/MeOrion.h"
//#include "src/makeblock/MeShield.h"
#include "src/makeblock/MeAuriga.h"

MeRGBLed led(PORT_8);
MeUltrasonicSensor ultrR(PORT_3);
MeUltrasonicSensor ultrL(PORT_4);

MeDCMotor MotorL(M1);
MeDCMotor MotorR(M2);

int modus = 0;  // 0 = training until double success then drive   1 = drive  2 = test motors with serial, input one of the outputs 0-5

// Aug 23, 2022 modified InitWeights and epsilon learning rate
// to be faster on the Arduino Mega 2560 (This may or may not be a good idea)
const double InitWeights = .41;  
double epsilon = 0.1976;  // learning rate 
double mom = 0.024;  
double success = 0.4;   //training loops until failure rate is lower than this
double bias = 1;

int patern = 25;    // number of test paterns in test_in and test_out
int count = 0;     // counter for print serial and view leds in training session
int count_to = 50; //2000;    //cycles to count for serial out

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

void setup() {
  Serial.begin(115200);
  Serial.println("Start");   
  Wire.begin();
  stop();
 
  
  rst();   // reset all weights to 0, maybe not necessary
  seed(); 
  
  delay(2);
  
}

void loop() {  
  if(modus == 0)     {  training();  } 
  else if(modus == 1){  sensor(); load(); outvalue(); /*set_leds();*/ serial_print_out();  drive(); }
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

void biasrandom()
{
  randomSeed(analogRead(3)); //srand(time(0)); 
  double upper = 900;
  double lower = 0;     
  double tmp = double(random(100))/100;
  bias = 2.0 * (tmp - 0.5 ) * InitWeights;   
}

void learning()  // train the network
{  
  double sum = 0;

  for(int x = 0 ; x < cell_raster_hidden ; x++ ) 
  {    
    Accum = 0.0 ;
    for(int y = 0 ; y < cell_raster_out ; y++ )
    {
      Accum += cells_hidden_out_weight[x][y] * Delta_out[y] ;
    }
  Delta_hidden[x] = Accum * cells_hidden[x] * (1.0 - cells_hidden[x]) ;
  }
  
  for(int x = 0; x < cell_raster_hidden; x++)
  {    
    cells_in_hidden_weight_change[cell_raster_in][x] =  epsilon * Delta_hidden[x] + mom * cells_in_hidden_weight_change[cell_raster_in][x] ;
    cells_in_hidden_weight[cell_raster_in][x] += cells_in_hidden_weight_change[cell_raster_in][x];
    for(int y = 0; y < cell_raster_in; y++)
    {       
      cells_in_hidden_weight_change[y][x] =  epsilon * test_in[p][y] * Delta_hidden[x] + mom * cells_in_hidden_weight_change[y][x] ;
      cells_in_hidden_weight[y][x] +=  cells_in_hidden_weight_change[y][x];    
    }
  }
  
  for(int x = 0; x < cell_raster_out; x++)
  {
    cells_hidden_out_weight_change[cell_raster_hidden][x] = epsilon *  Delta_out[x] + mom * cells_hidden_out_weight_change[cell_raster_hidden][x] ;
    cells_hidden_out_weight[cell_raster_hidden][x] += cells_hidden_out_weight_change[cell_raster_hidden][x];
    for(int y = 0; y < cell_raster_hidden; y++)
    {     
      cells_hidden_out_weight_change[y][x] =  epsilon * cells_hidden[y] * Delta_out[x]  + mom * cells_hidden_out_weight_change[y][x] ;
      cells_hidden_out_weight[y][x] += cells_hidden_out_weight_change[y][x];
    }
  } 
}

void load()  // load data through the network
{
  //  biasrandom();  // test with a random bias
  double sum = 1.0; 
  int c = 0;
  Error = 0.0;
  for(int x = 0; x < cell_raster_hidden; x++)  // input        
  {  
    sum = cells_in_hidden_weight[cell_raster_in][x];
    for(int y = 0; y < cell_raster_in; y++)
    {
      sum += cells_in[y] * cells_in_hidden_weight[y][x];         
    }   
    cells_hidden[x] = 1.0/(1.0 + exp(-sum));   //?
  }  
  for(int x = 0; x < cell_raster_out; x++)  // hidden  
  {
    sum = cells_hidden_out_weight[cell_raster_hidden][x];
    for(int y = 0; y < cell_raster_hidden; y++)
    {
      sum += cells_hidden[y] * cells_hidden_out_weight[y][x];          
    }   
    cells_out[x] =  1.0/(1.0 + exp(-sum));
    Delta_out[x] =  (test_out[p][x] - cells_out[x]) * cells_out[x] * (1.0 -cells_out[x]);  
    Error += 0.5 * (test_out[p][x] - cells_out[x]) * (test_out[p][x] - cells_out[x]);
  }
}

void rst()  
{
  for(int c = 0; c < cell_raster_in; c++)
  {  
    cells_in[c] = 0;  
  }
  for(int c = 0; c < cell_raster_hidden; c++)
  {   
    cells_hidden[c] = 0;    
  } 
  for(int c = 0; c < cell_raster_out; c++)
  {  
    cells_out[c] = 0;
  }   
}

void outvalue()  // get the output with the highest value
{
  double o = cells_out[0]; 
  out = 0; 
  for(int i = 0; i < cell_raster_out; i++)
  {    
    if(cells_out[i] >= o)
    {
      o = cells_out[i];
      out = i; 
    }  
  } 
}

void seed() // random seed the weights
{  
  double upper = 1000;
  double lower = 50;
  for(int x = 0; x < cell_raster_in; x++)
  {
  for (int sy = 0 ; sy < cell_raster_hidden; sy++)
    { randomSeed(analogRead(3)); //srand(time(0));       
      double tmp = double(random(100))/100;
      cells_in_hidden_weight[x][sy] = 2.0 * (tmp - 0.5 ) * InitWeights;     
      cells_in_hidden_weight_change [x][sy] = 0.0;
    }
  }
  
  for(int x = 0; x < cell_raster_hidden; x++)
  {
  for (int sy = 0 ; sy < cell_raster_out; sy++)
    { randomSeed(analogRead(3));        
      double tmp = double(random(100))/100;
      cells_hidden_out_weight[x][sy] = 2.0 * (tmp - 0.5 ) * InitWeights;     
      cells_hidden_out_weight_change[x][sy] = 0.0;
    }
  }       
}
