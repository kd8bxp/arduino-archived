// Code Example for jolliFactory Voice Input 8X Bi-color LED Matrices Scrolling Text Display via SPI example 1.0
   
   
//SPI connections between Arduino Nano/UNO and the displays are MOSI (Pin 11), SCK (Pin 13) and SS (Pin 10) at the Arduino side and Din, CLK and Load pins at the LED Matrix Driver Module respectively.        
#include <SPI.h>     
     
       
#define GREEN 0                          
#define RED 1                            

#define offREDoffGREEN 0
#define offREDonGREEN 1
#define onREDoffGREEN 2

int SPI_CS = 10;// This SPI Chip Select pin controls the MAX7219
int bi_maxInUse = 8; //No. of Bi-color LED Matrix used
     
     
//These variables used in ISR to be declared as volatile to prevent problem with compiler optimization.     
volatile int z=0; 
volatile int y=0;
volatile boolean receiveComplete = false;
     
int altShutDownCount = 0;
int scrollSpeed = 3;

int MAXbrightnessCount = 0;
int maxInShutdown=RED; // indicates which LED Matrix color is currently off

int colorMode = '1';  // default color (1 = RED, 2 = GREEN, 3 = ORANGE, 4 = blank off)
int temp_colorMode = '1';
    
int brightnessLevel = '9';  // default brightness level (1 to 9)
int SetbrightnessValue = (15 * (brightnessLevel-48))/9;  //Re-adjust values to meet MAX7219 Brightness Level range between 0 to 15
int last_SetbrightnessValue = 0;
int MAX72xxbrightnessValue = 15;
    
boolean newMsg = false;
boolean newCmd = false;

char msg[255] = " 1 2 3 4 5 6 7 8 9 0  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z   *  P o w e r e d  B y  j o l l i F a c t o r y  * ";
int msgsize = strlen(msg);

char temp_msg[255];
int temp_msgsize = strlen(temp_msg);



// The character set courtesy of cosmicvoid.

byte Font8x5[104*8] =
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02, 0x00,
 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A, 0x00,
 0x0E, 0x15, 0x05, 0x0E, 0x14, 0x15, 0x0E, 0x00,
 0x13, 0x13, 0x08, 0x04, 0x02, 0x19, 0x19, 0x00,
 0x06, 0x09, 0x05, 0x02, 0x15, 0x09, 0x16, 0x00,
 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x04, 0x02, 0x01, 0x01, 0x01, 0x02, 0x04, 0x00,
 0x01, 0x02, 0x04, 0x04, 0x04, 0x02, 0x01, 0x00,
 0x00, 0x0A, 0x15, 0x0E, 0x15, 0x0A, 0x00, 0x00,
 0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01,
 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
 0x10, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01, 0x00,
 0x0E, 0x11, 0x19, 0x15, 0x13, 0x11, 0x0E, 0x00,
 0x04, 0x06, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00,
 0x0E, 0x11, 0x10, 0x0C, 0x02, 0x01, 0x1F, 0x00,
 0x0E, 0x11, 0x10, 0x0C, 0x10, 0x11, 0x0E, 0x00,
 0x08, 0x0C, 0x0A, 0x09, 0x1F, 0x08, 0x08, 0x00,
 0x1F, 0x01, 0x01, 0x0E, 0x10, 0x11, 0x0E, 0x00,
 0x0C, 0x02, 0x01, 0x0F, 0x11, 0x11, 0x0E, 0x00,
 0x1F, 0x10, 0x08, 0x04, 0x02, 0x02, 0x02, 0x00,
 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E, 0x00,
 0x0E, 0x11, 0x11, 0x1E, 0x10, 0x08, 0x06, 0x00,
 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x02, 0x01,
 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x00,
 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x00, 0x00,
 0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01, 0x00,
 0x0E, 0x11, 0x10, 0x08, 0x04, 0x00, 0x04, 0x00,
 0x0E, 0x11, 0x1D, 0x15, 0x0D, 0x01, 0x1E, 0x00,
 0x04, 0x0A, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x00,
 0x0F, 0x11, 0x11, 0x0F, 0x11, 0x11, 0x0F, 0x00,
 0x0E, 0x11, 0x01, 0x01, 0x01, 0x11, 0x0E, 0x00,
 0x07, 0x09, 0x11, 0x11, 0x11, 0x09, 0x07, 0x00,
 0x1F, 0x01, 0x01, 0x0F, 0x01, 0x01, 0x1F, 0x00,
 0x1F, 0x01, 0x01, 0x0F, 0x01, 0x01, 0x01, 0x00,
 0x0E, 0x11, 0x01, 0x0D, 0x11, 0x19, 0x16, 0x00,
 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11, 0x00,
 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00,
 0x1C, 0x08, 0x08, 0x08, 0x08, 0x09, 0x06, 0x00,
 0x11, 0x09, 0x05, 0x03, 0x05, 0x09, 0x11, 0x00,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x00,
 0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11, 0x00,
 0x11, 0x13, 0x13, 0x15, 0x19, 0x19, 0x11, 0x00,
 0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00,
 0x0F, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x01, 0x00,
 0x0E, 0x11, 0x11, 0x11, 0x15, 0x09, 0x16, 0x00,
 0x0F, 0x11, 0x11, 0x0F, 0x05, 0x09, 0x11, 0x00,
 0x0E, 0x11, 0x01, 0x0E, 0x10, 0x11, 0x0E, 0x00,
 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00,
 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00,
 0x11, 0x11, 0x11, 0x11, 0x0A, 0x0A, 0x04, 0x00,
 0x41, 0x41, 0x41, 0x49, 0x2A, 0x2A, 0x14, 0x00,
 0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11, 0x00,
 0x11, 0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x00,
 0x1F, 0x10, 0x08, 0x04, 0x02, 0x01, 0x1F, 0x00,
 0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x00,
 0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x10, 0x00,
 0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07, 0x00,
 0x00, 0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00,
 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x06, 0x08, 0x0E, 0x09, 0x0E, 0x00,
 0x01, 0x01, 0x0D, 0x13, 0x11, 0x13, 0x0D, 0x00,
 0x00, 0x00, 0x06, 0x09, 0x01, 0x09, 0x06, 0x00,
 0x10, 0x10, 0x16, 0x19, 0x11, 0x19, 0x16, 0x00,
 0x00, 0x00, 0x06, 0x09, 0x07, 0x01, 0x0E, 0x00,
 0x04, 0x0A, 0x02, 0x07, 0x02, 0x02, 0x02, 0x00,
 0x00, 0x00, 0x06, 0x09, 0x09, 0x06, 0x08, 0x07,
 0x01, 0x01, 0x0D, 0x13, 0x11, 0x11, 0x11, 0x00,
 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00,
 0x04, 0x00, 0x06, 0x04, 0x04, 0x04, 0x04, 0x03,
 0x01, 0x01, 0x09, 0x05, 0x03, 0x05, 0x09, 0x00,
 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00,
 0x00, 0x00, 0x15, 0x2B, 0x29, 0x29, 0x29, 0x00,
 0x00, 0x00, 0x0D, 0x13, 0x11, 0x11, 0x11, 0x00,
 0x00, 0x00, 0x06, 0x09, 0x09, 0x09, 0x06, 0x00,
 0x00, 0x00, 0x0D, 0x13, 0x13, 0x0D, 0x01, 0x01,
 0x00, 0x00, 0x16, 0x19, 0x19, 0x16, 0x10, 0x10,
 0x00, 0x00, 0x0D, 0x13, 0x01, 0x01, 0x01, 0x00,
 0x00, 0x00, 0x0E, 0x01, 0x06, 0x08, 0x07, 0x00,
 0x00, 0x02, 0x07, 0x02, 0x02, 0x02, 0x04, 0x00,
 0x00, 0x00, 0x11, 0x11, 0x11, 0x19, 0x16, 0x00,
 0x00, 0x00, 0x11, 0x11, 0x11, 0x0A, 0x04, 0x00,
 0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0A, 0x00,
 0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00,
 0x00, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x08, 0x06,
 0x00, 0x00, 0x0F, 0x08, 0x06, 0x01, 0x0F, 0x00,
 0x04, 0x02, 0x02, 0x01, 0x02, 0x02, 0x04, 0x00,
 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00,
 0x01, 0x02, 0x02, 0x04, 0x02, 0x02, 0x01, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x1C, 0x2A, 0x49, 0x49, 0x41, 0x22, 0x1C, 0x00,
 0x1C, 0x22, 0x51, 0x49, 0x41, 0x22, 0x1C, 0x00,
 0x1C, 0x22, 0x41, 0x79, 0x41, 0x22, 0x1C, 0x00,
 0x1C, 0x22, 0x41, 0x49, 0x51, 0x22, 0x1C, 0x00,
 0x1C, 0x22, 0x41, 0x49, 0x49, 0x2A, 0x1C, 0x00,
 0x1C, 0x22, 0x41, 0x49, 0x45, 0x22, 0x1C, 0x00,
 0x1C, 0x22, 0x41, 0x4F, 0x41, 0x22, 0x1C, 0x00,
 0x1C, 0x22, 0x45, 0x49, 0x41, 0x22, 0x1C, 0x00};


byte lentbl_S[104] =
{
 2, 2, 3, 5, 5, 5, 5, 2, 
 3, 3, 5, 5, 2, 5, 1, 5, 
 5, 4, 5, 5, 5, 5, 5, 5, 
 5, 5, 1, 2, 4, 4, 4, 5, 
 5, 5, 5, 5, 5, 5, 5, 5, 
 5, 3, 5, 5, 4, 5, 5, 5, 
 5, 5, 5, 5, 5, 5, 5, 7, 
 5, 5, 5, 3, 5, 3, 5, 5, 
 2, 4, 5, 4, 5, 4, 4, 4, 
 5, 2, 3, 4, 2, 6, 5, 4, 
 5, 5, 5, 4, 3, 5, 5, 5, 
 5, 4, 4, 3, 2, 3, 0, 0, 
 7, 7, 7, 7, 7, 7, 7, 7
};

int curcharix = 0;
int curcharbit = 0;
int curcharixsave = 0;
int curcharbitsave = 0;
int curcharixsave2 = 0;
int curcharbitsave2 = 0;
char curchar;



//**********************************************************************************************************************************************************
void setup()
{ 
  // Set up interrupt to monitor for serial incoming data  
  cli();//stop interrupts

  //set timer1 interrupt at 1Hz for checking Serial incoming 
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0

  // set compare match register for 1hz increments
  OCR1A = 1953;// = (16*10^6) / (1*1024) - 1 (must be <65536)  //  second     //Best

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts

  
  
  pinMode(SPI_CS, OUTPUT);

  Serial.begin (9600);
  Serial.println("jolliFactory Voice Input 8X Bi-color LED Matrices Scrolling Text Display via SPI example 1.0");              



  SPI.begin(); //setup SPI Interface

  bi_maxTransferAll(0x0F, 0x00);   // 00 - Turn off Test mode
  bi_maxTransferAll(0x09, 0x00);   //Register 09 - BCD Decoding  // 0 = No decoding
  bi_maxTransferAll(0x0B, 0x07);   //Register B - Scan limit 1-7  // 7 = All LEDS
  bi_maxTransferAll(0x0C, 0x01);   // 01 = on 00 = Power saving mode or shutdown

  setBrightness();
}




//**********************************************************************************************************************************************************
void loop()
{
  int i,j,k;

  while(1)
  {  
    //**********************************************************************************************************************************************************
    //Start of Serial available Check
    //If there is serial receiving data and data received is complete
    if ((z > 0) and (receiveComplete == true)){  
      y=0;
      receiveComplete = false;

      newMsg = true;
      newCmd = false;

      i = z;
      z = 0;  //initialize it back for the ISR for serial receive


      temp_msg[i]=' '; // Add a space to end of string        
      
      // Append spaces behind message
      for (int x=0;x<bi_maxInUse*2;x++)
       temp_msg[i++]=' '; // Add another space to end of string        
     
      temp_msg[i++]='\0'; // Null terminate the string        
      temp_msgsize = strlen(temp_msg);

    
    
    
      if (strncmp(temp_msg, " set", 4) == 0)  //Commands received    
      {  
        newCmd = true;
        newMsg = false;

        if (strncmp(temp_msg, " set red", 8) == 0)  //Commands received   
        {
          temp_colorMode = '1';  
          colorMode = temp_colorMode;
        }          
        else if (strncmp(temp_msg, " set green", 10) == 0)  //Commands received   
        {
          temp_colorMode = '2';                    
          colorMode = temp_colorMode;
        }
        else if (strncmp(temp_msg, " set orange", 11) == 0)  //Commands received   
        {
          temp_colorMode = '3';                    
          colorMode = temp_colorMode;
        }
        else if (strncmp(temp_msg, " set blank", 10) == 0)  //Commands received   
        {
          temp_colorMode = '4';                    
          colorMode = temp_colorMode;
        }
        
         

        if (strncmp(temp_msg, " set darker", 11) == 0)  //Commands received   
        {
            SetbrightnessValue = SetbrightnessValue - 1;
                 
            if (SetbrightnessValue < 0)
              SetbrightnessValue = 0;
        }          
        else if (strncmp(temp_msg, " set brighter", 13) == 0)  //Commands received   
        {
            SetbrightnessValue = SetbrightnessValue + 1;
                 
            if (SetbrightnessValue > 15)
              SetbrightnessValue = 15;
        }
        else if (strncmp(temp_msg, " set darkest", 12) == 0)  //Commands received   
        {
              SetbrightnessValue = 0;
        }
        else if (strncmp(temp_msg, " set brightest", 14) == 0)  //Commands received   
        {
              SetbrightnessValue = 15;
        }

        setBrightness();            




        if (strncmp(temp_msg, " set slower", 11) == 0)  //Commands received   
        {
             scrollSpeed = scrollSpeed - 1;        
             if (scrollSpeed < 1)
               scrollSpeed = 1;
        }          
        else if (strncmp(temp_msg, " set faster", 11) == 0)  //Commands received   
        {
            scrollSpeed = scrollSpeed + 1;
        
            if (scrollSpeed > 9)
              scrollSpeed = 9;
        }
        else if (strncmp(temp_msg, " set slowest", 12) == 0)  //Commands received   
        {
               scrollSpeed = 1;
        }
        else if (strncmp(temp_msg, " set fastest", 12) == 0)  //Commands received   
        {
              scrollSpeed = 9;
        }

     
        
      }   
    }
    //**********************************************************************************************************************************************************
    //End of Serial available Check
    
        
    curcharixsave2 = curcharix;
    curcharbitsave2 = curcharbit;

    for (i=15;i>=0;i=i-2) // Loop through our 8 Bi-color LED Matrices
    //for (i=7;i>=0;i=i-2) // Loop through our 4 Bi-color LED Matrices 
    {
     
      for (j=0;j<8;j++) // Set up rows on current display
      {   
        byte outputbyte = 0;
   
        curchar = msg[curcharix];
        curcharixsave = curcharix;
        curcharbitsave = curcharbit; 
   
        for (k=7;k>=0;k--) // Copy over data for 8 columns to current row and send it to current display  - scroll from right to left
        {                                  
        // If ISR not used for altShutDown - just have the following instructions below to alternate shutdown of MAX7219 chips for RED and GREEN LEDs to prevent 'ghosting' display
        // Instead of using delay to intentionally slow down the scroll, call altshutdown() more often to slow down the scroll during looping
          
          if (altShutDownCount > scrollSpeed * 3){
            altShutDown();
            altShutDownCount = 0;
          }
          
          altShutDownCount++;

                    
          byte currentcharbits;

          // This byte is the bitmap of the current character for the current row
          currentcharbits = Font8x5[((curchar-32)*8)+j];
  
          if (currentcharbits & (1<<curcharbit))
          outputbyte |= (1<<k);
   
          // advance the current character bit of current character
          curcharbit ++;
 
          if (curcharbit > lentbl_S[curchar-32]) // we are past the end of this character, so advance.
          {
            curcharbit = 0;
            curcharix += 1;  
            if (curcharix+1 > msgsize) curcharix=0;
            curchar = msg[curcharix];
          }
        }


        if (i<16)   //8 x Bi-color LED Matrix (need 16 x MAX7219)
        //if (i<8)   //4 x Bi-color LED Matrix (need 8 x MAX7219)
        {   
            bi_maxTransferOne((i+1)/2, j+1, outputbyte);              
        } 
        

        if (j != 7) // if this is not the last row, roll back advancement, if it is, leave the counters advanced.
        {
          curcharix = curcharixsave;
          curcharbit = curcharbitsave;
        } 
      }
    }
 
    curcharix = curcharixsave2;
    curcharbit = curcharbitsave2;
 
 
    // Prepare new text message
    if  (newMsg == true)
    {
      newMsg = false;

      MAXbrightnessCount = 0;
      MAX72xxbrightnessValue = 0;
 
      if (newCmd == true)
      {
        newCmd = false;
   
        if (temp_msg[5]=='*') // To use current text message
        {  
          //do Nothing
        }
        else
        {  
          for (j=0;j<temp_msgsize;j++) // Discard command part from message data
            temp_msg[j]=temp_msg[j+5];
   
          temp_msgsize = strlen(temp_msg);  
      

          for (j=0;j<temp_msgsize;j++)
            msg[j]=temp_msg[j];
   
          msgsize = strlen(temp_msg);   
        }
      }
      else
      {   
        for (j=0;j<temp_msgsize;j++)
          msg[j]=temp_msg[j];
   
        msgsize = strlen(temp_msg);        
      }   

      colorMode = temp_colorMode;
    }
 
    curchar = msg[curcharix];
 
 
    // advance the current character bit of current character   
    curcharbit ++;
    
    if (curcharbit > lentbl_S[curchar-32]) // we are past the end of this character, so advance.
    {
      curcharbit = 0;
      curcharix += 1;
      if (curcharix+1 > msgsize) curcharix=0;
      {
        curchar = msg[curcharix];
      }
    }  
        
    
  }
}




//Timer1 used to check Serial incoming
//**********************************************************************************************************************************************************
ISR(TIMER1_COMPA_vect) {
  do{
    if (Serial.available()){

      temp_msg[z] = Serial.read();
      
      //Suspect long voice input causing hang
      //Serial.println(temp_msg[z]);

      //if (temp_msg[z] == '\n')  // Complete if Newline found
      if (temp_msg[z] == '#')  // Complete if Newline found
        receiveComplete = true;

      if ((temp_msg[z] == 35) or (temp_msg[z] == 42) or (temp_msg[z] < 32) or (temp_msg[z] > 122)) //Replace with space for *, # and undefined characters not within the font library even carriage return (\r) and newline (\n) characters
      {  
        temp_msg[z] = ' ';
      }

      z++;     

    }//end if serial available

  }//end do
  while (Serial.available() > 0);


  y++;
}    



  
//**********************************************************************************************************************************************************  
void setBrightness(){
      
  // To skip if there is no change in SetbrightnessValue
  if (SetbrightnessValue != last_SetbrightnessValue)
  {  
    bi_maxTransferAll(0x0A, SetbrightnessValue);      //Set Brightness
    bi_maxTransferAll(0x00, 0x00);  //No-op commands

    last_SetbrightnessValue = SetbrightnessValue;  
  }
  
}



//**********************************************************************************************************************************************************  
void bi_maxTransferAll(uint8_t address, uint8_t value) {

  digitalWrite(SPI_CS, LOW); 

    for ( int c=1; c<= bi_maxInUse*2;c++) {
        SPI.transfer(address);  // specify register
        SPI.transfer(value);  // put data
    }

  digitalWrite(SPI_CS, HIGH); 
}



//**********************************************************************************************************************************************************  
/**
 * Transfers data to a MAX7219/MAX7221 register of a particular Bi-color LED Matrix module.
 *
 * @param whichMax The Max72xx to load data and value into
 * @param address The register to load data into
 * @param value Value to store in the register
 */
 
void bi_maxTransferOne(uint8_t whichMax, uint8_t address, uint8_t value) {

  byte noop_reg = 0x00;    //max7219 No op register
  byte noop_value = 0x00;  //value

  digitalWrite(SPI_CS, LOW); 

  for (int i=bi_maxInUse; i>0; i--)   // Loop through our number of Bi-color LED Matrices 
  {
    if (i==whichMax)
    {
      SPI.transfer(address);  // Send the register address
      SPI.transfer(value);    // Send the value

      SPI.transfer(address);  // Send the register address
      SPI.transfer(value);    // Send the value

    }
    else
    {
      SPI.transfer(noop_reg);    // Send the register address
      SPI.transfer(noop_value);  // Send the value

      SPI.transfer(noop_reg);    // Send the register address
      SPI.transfer(noop_value);  // Send the value
    }
  }

  digitalWrite(SPI_CS, HIGH);
}




//**********************************************************************************************************************************************************  
void bi_maxShutdown(uint8_t cmd) 
{
  byte noop_reg = 0x00;      //max7219_reg_no_op
  byte shutdown_reg = 0x0c;  //max7219_reg_shutdown
  byte col = 0x01;  //shutdown false
  byte col2 = 0x00;  //shutdown true


  if (cmd == offREDoffGREEN)
  {    
    digitalWrite(SPI_CS, LOW);

    for (int c =1; c<= bi_maxInUse; c++)
    {
      SPI.transfer(shutdown_reg);  // Send the register address
      SPI.transfer(col2);          // Send the value

      SPI.transfer(shutdown_reg);  // Send the register address
      SPI.transfer(col2);          // Send the value
    }

    digitalWrite(SPI_CS, HIGH);    
  }
  else if (cmd == offREDonGREEN)
  {
    digitalWrite(SPI_CS, LOW);

    for (int c =1; c<= bi_maxInUse; c++) 
    {
      SPI.transfer(shutdown_reg);  // Send the register address
      SPI.transfer(col);           // Send the value

      SPI.transfer(shutdown_reg);  // Send the register address
      SPI.transfer(col2);          // Send the value
    }

    digitalWrite(SPI_CS, HIGH);
  }
  else if (cmd == onREDoffGREEN)
  {
    digitalWrite(SPI_CS, LOW);

    for (int c =1; c<= bi_maxInUse; c++) 
    {      
      SPI.transfer(shutdown_reg);  // Send the register address
      SPI.transfer(col2);          // Send the value

      SPI.transfer(shutdown_reg);  // Send the register address
      SPI.transfer(col);           // Send the value  
    }

    digitalWrite(SPI_CS, HIGH);
  }



  //No ops register to shift out instructions   
  digitalWrite(SPI_CS, LOW);

  for (int c =1; c<= bi_maxInUse; c++) 
  {      
    SPI.transfer(noop_reg);  // Send the register address
    SPI.transfer(0x00);      // Send the value

    SPI.transfer(noop_reg);  // Send the register address
    SPI.transfer(0x00);      // Send the value
  }

  digitalWrite(SPI_CS, HIGH);
}





//**********************************************************************************************************************************************************
void altShutDown()    //alternate shutdown of MAX7219 chips for RED and GREEN LEDs 
{
  if (colorMode == '3')    //Scrolling in ORANGE
  {
    if(maxInShutdown==RED){
      bi_maxShutdown(onREDoffGREEN);
      maxInShutdown=GREEN;
    } 
    else 
    { 
      bi_maxShutdown(offREDonGREEN);
      maxInShutdown=RED;
    }
  }
  else if (colorMode == '2')   //Scrolling in GREEN
  {
    bi_maxShutdown(offREDonGREEN);
    maxInShutdown=RED;
  }
  else if (colorMode == '1')   //Scrolling in RED
  {
    bi_maxShutdown(onREDoffGREEN);
    maxInShutdown=GREEN;
  }
  else if (colorMode == '4')  //Blank Display
  {
    bi_maxShutdown(offREDoffGREEN);       
    maxInShutdown=GREEN;
  }
} 

