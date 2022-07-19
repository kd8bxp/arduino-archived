//Author: Ralph Heymsfeld
//28/06/2018
//https://www.the-diy-life.com/running-an-artificial-neural-network-on-an-arduino-uno/

#include <math.h>

int En1 = A0;                  //L293D EN1 Correspond Development Board R3 pin A0
int In1 = A1;                  //L293D IN1 Correspond Development Board R3 pin A1
int In2 = A2;                  //L293D IN2 Correspond Development Board R3 pin A2 

int In4 = A3;                  //L293D IN4 Correspond Development Board R3 pin A3
int In3 = A4;                  //L293D IN3 Correspond Development Board R3 pin A4 
int En2 = A5;                  //L293D EN2 Correspond Development Board R3 pin A5
int Left  = 2; //0                 //Left sensor
int Right = 3; //1                //Right sensor
unsigned char pwmval_left  = 0;           //Variable definitions 
unsigned char pwmval_right = 0;

unsigned char pwmval_left_init  = 6;      // Left Electric Motor Duty Ratio Adjustment, adjust between 0 to 20
unsigned char pwmval_right_init = 6;      // Right Electric Motor Duty Ratio Adjustment, adjust between 0 to 20

unsigned char right_pwm = 1;              // Right Electric Motor PWM switch, when value is 1, turn on
unsigned char left_pwm  = 1;              // left  Electric Motor PWM switch, when value is 1, turn on
byte flag = 0;

/******************************************************************
 * Network Configuration - customized per network 
 ******************************************************************/

const int PatternCount = 4;
const int InputNodes = 2;
const int HiddenNodes = 12;
const int OutputNodes = 2;
const float LearningRate = 0.3;
const float Momentum = 0.9;
const float InitialWeightMax = 0.05;
const float Success = 0.0001;

const byte Input[PatternCount][InputNodes] = {
  { 1, 1 },  // 0
  { 0, 1 },  // 1
  { 1, 0 },  // 2
  { 0, 0 },  // 3
  
}; 

const byte Target[PatternCount][OutputNodes] = {
  { 1, 1 },  
  { 0, 1 }, 
  { 1, 0 }, 
  { 0, 0 }, 
 
};

/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r;
int ReportEvery1000;
int RandomizedIndex[PatternCount];
long  TrainingCycle;
float Rando;
float Error;
float Accum;


float Hidden[HiddenNodes];
float Output[OutputNodes];
float HiddenWeights[InputNodes+1][HiddenNodes];
float OutputWeights[HiddenNodes+1][OutputNodes];
float HiddenDelta[HiddenNodes];
float OutputDelta[OutputNodes];
float ChangeHiddenWeights[InputNodes+1][HiddenNodes];
float ChangeOutputWeights[HiddenNodes+1][OutputNodes];

void setup(){
  Serial.begin(115200);
  randomSeed(analogRead(3));
  ReportEvery1000 = 1;
  for( p = 0 ; p < PatternCount ; p++ ) {    
    RandomizedIndex[p] = p ;
  }
   // put your setup code here, to run once:
  pinMode(En1,OUTPUT);                    //Set the pin to output 
  pinMode(In1,OUTPUT);                    //Set the pin to output 
  pinMode(In2,OUTPUT);                    //Set the pin to output  
  pinMode(En2,OUTPUT);                    //Set the pin to output  
  pinMode(In3,OUTPUT);                    //Set the pin to output 
  pinMode(In4,OUTPUT);                    //Set the pin to output  
}  

void loop() {
  if (!flag) {train_loop();}
  else {
    int left_read = digitalRead(Left);
    int right_read = digitalRead(Right);
    //Input[0][0] = digitalRead(Left);
    //Input[0][1] = digitalRead(Right);
    Serial.print((OutputDelta[left_read])); Serial.print(" "); Serial.println((OutputDelta[right_read]));
    if ((Output[left_read]) == 1 && (Output[right_read] == 1)) {Serial.println(" Forward");}
    if (Output[left_read] == 1 && Output[right_read] == 0) {Serial.println(" Left Turn");}
    if (Output[left_read] == 0 && Output[right_read] == 1) {Serial.println(" Right Turn");}
    if (Output[left_read] == 0 && Output[right_read] == 0) {Serial.println(" Backup");}
  }
}


void train_loop (){


/******************************************************************
* Initialize HiddenWeights and ChangeHiddenWeights 
******************************************************************/

  for( i = 0 ; i < HiddenNodes ; i++ ) {    
    for( j = 0 ; j <= InputNodes ; j++ ) { 
      ChangeHiddenWeights[j][i] = 0.0 ;
      Rando = float(random(100))/100;
      HiddenWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }
/******************************************************************
* Initialize OutputWeights and ChangeOutputWeights
******************************************************************/

  for( i = 0 ; i < OutputNodes ; i ++ ) {    
    for( j = 0 ; j <= HiddenNodes ; j++ ) {
      ChangeOutputWeights[j][i] = 0.0 ;  
      Rando = float(random(100))/100;        
      OutputWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }
  Serial.println("Initial/Untrained Outputs: ");
  toTerminal();
/******************************************************************
* Begin training 
******************************************************************/

  for( TrainingCycle = 1 ; TrainingCycle < 2147483647 ; TrainingCycle++) {    

/******************************************************************
* Randomize order of training patterns
******************************************************************/

    for( p = 0 ; p < PatternCount ; p++) {
      q = random(PatternCount);
      r = RandomizedIndex[p] ; 
      RandomizedIndex[p] = RandomizedIndex[q] ; 
      RandomizedIndex[q] = r ;
    }
    Error = 0.0 ;
/******************************************************************
* Cycle through each training pattern in the randomized order
******************************************************************/
    for( q = 0 ; q < PatternCount ; q++ ) {    
      p = RandomizedIndex[q];

/******************************************************************
* Compute hidden layer activations
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = HiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) {
          Accum += Input[p][j] * HiddenWeights[j][i] ;
        }
        Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
      }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i++ ) {    
        Accum = OutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          Accum += Hidden[j] * OutputWeights[j][i] ;
        }
        Output[i] = 1.0/(1.0 + exp(-Accum)) ;   
        OutputDelta[i] = (Target[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]) ;   
        Error += 0.5 * (Target[p][i] - Output[i]) * (Target[p][i] - Output[i]) ;
      }

/******************************************************************
* Backpropagate errors to hidden layer
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = 0.0 ;
        for( j = 0 ; j < OutputNodes ; j++ ) {
          Accum += OutputWeights[i][j] * OutputDelta[j] ;
        }
        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]) ;
      }


/******************************************************************
* Update Inner-->Hidden Weights
******************************************************************/


      for( i = 0 ; i < HiddenNodes ; i++ ) {     
        ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i] ;
        HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) { 
          ChangeHiddenWeights[j][i] = LearningRate * Input[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
          HiddenWeights[j][i] += ChangeHiddenWeights[j][i] ;
        }
      }

/******************************************************************
* Update Hidden-->Output Weights
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i ++ ) {    
        ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i] ;
        OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i] ;
          OutputWeights[j][i] += ChangeOutputWeights[j][i] ;
        }
      }
    }

/******************************************************************
* Every 1000 cycles send data to terminal for display
******************************************************************/
    ReportEvery1000 = ReportEvery1000 - 1;
    if (ReportEvery1000 == 0)
    {
      Serial.println(); 
      Serial.println(); 
      Serial.print ("TrainingCycle: ");
      Serial.print (TrainingCycle);
      Serial.print ("  Error = ");
      Serial.println (Error, 5);

      toTerminal();

      if (TrainingCycle==1)
      {
        ReportEvery1000 = 999;
      }
      else
      {
        ReportEvery1000 = 1000;
      }
    }    


/******************************************************************
* If error rate is less than pre-determined threshold then end
******************************************************************/

    if( Error < Success ) break ;  
  }
  Serial.println ();
  Serial.println(); 
  Serial.print ("TrainingCycle: ");
  Serial.print (TrainingCycle);
  Serial.print ("  Error = ");
  Serial.println (Error, 5);

  toTerminal();

  Serial.println ();  
  Serial.println ();
  Serial.println ("Training Set Solved! ");
  Serial.println ("--------"); 
  Serial.println ();
  Serial.println ();  
  ReportEvery1000 = 1;
  flag = 1;
}

// Maybe Duplicate this after it's trained ? I'm not sure 
void toTerminal()
{

  for( p = 0 ; p < PatternCount ; p++ ) { 
    Serial.println(); 
    Serial.print ("  Training Pattern: ");
    Serial.println (p);      
    Serial.print ("  Input ");
    for( i = 0 ; i < InputNodes ; i++ ) {
      Serial.print (Input[p][i], DEC);
      Serial.print (" ");
    }
    Serial.print ("  Target ");
    for( i = 0 ; i < OutputNodes ; i++ ) {
      Serial.print (Target[p][i], DEC);
      Serial.print (" ");
    }
/******************************************************************
* Compute hidden layer activations
******************************************************************/

    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[p][j] * HiddenWeights[j][i] ;
      }
      Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
    }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[HiddenNodes][i] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }
      Output[i] = 1.0/(1.0 + exp(-Accum)) ; 
    }
    Serial.print ("  Output ");
    for( i = 0 ; i < OutputNodes ; i++ ) {       
      Serial.print (Output[i], 5);
      Serial.print (" ");
    }
  }
}

void forward(void)                        //Car forward control function
{     
 digitalWrite(In1,HIGH);                   //Control left wheel forward
 digitalWrite(In2,LOW);
       
 digitalWrite(In3,LOW);                   //control right wheel forward
 digitalWrite(In4,HIGH);
}

void backward(void)                        //Car forward control function
{     
 digitalWrite(In1,LOW);                   //Control left wheel forward
 digitalWrite(In2,HIGH);
       
 digitalWrite(In3,HIGH);                   //control right wheel forward
 digitalWrite(In4,LOW);
}

void left_turn(void)                      //Smart Robot Car turn left
{
 digitalWrite(In1,LOW);                
 digitalWrite(In2,LOW);
       
 digitalWrite(In3,HIGH);                 
 digitalWrite(In4,LOW);
}

void right_turn(void)                     //Smart Robot Car turn right
{
 digitalWrite(In1,HIGH);                
 digitalWrite(In2,LOW);
       
 digitalWrite(In3,LOW);                 
 digitalWrite(In4,LOW);
}

void left_moto(void)                      //The speed of left wheel control function 
{  
 if(left_pwm)
 {
  if(pwmval_left <= pwmval_left_init)
  {digitalWrite(En1,HIGH);}
  else 
  {digitalWrite(En1,LOW);}
  if(pwmval_left >= 20)
  {pwmval_left = 0;}
 }
 else    
 {digitalWrite(En1,LOW);}
}

void right_moto(void)                     //The speed of right wheel control function
{ 
 if(right_pwm)
 { 
  if(pwmval_right <= pwmval_right_init)      
  {digitalWrite(En2,HIGH);}
  else if(pwmval_right > pwmval_right_init)
  {digitalWrite(En2,LOW);}
  if(pwmval_right >= 20)
  {pwmval_right = 0;}
 }
 else    
 {digitalWrite(En2,LOW);}
}
