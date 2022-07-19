#include "SmartResponseXE.h"
#include "RadioFunctions.h"
#include <EEPROM.h>
#include <avr/wdt.h>

char version[20]="HugQuest v1.5";
char messages[6][26];
char command[32] = ">                      \x00R05e\x00";
char netbuff[26] = "";
unsigned int curs = 2;
int ncurs = 0;
int row=0;
uint8_t canaryOffset = 27;

uint8_t rssi[32]; // yeah, what of it?

unsigned long int time_loop=0;
unsigned long int tokens=0;

int infected=false;
char outbuff[26];

// Operation mode constants
#define CONST_MODE_CONSOLE  0
#define CONST_MODE_HEX      1
#define CONST_MODE_RSSI     2
#define CONST_MODE_CHAT     3

// Special keyboard constants
#define CONST_KEY_CLEAR 0xF8
#define CONST_KEY_EXIT  0xF9

// EEPROM constants
#define CONST_MEM_NAME 0x0000000
#define CONST_MEM_HUGS 0x0000100
#define CONST_MEM_HACK 0x0000200

struct global_t{
  int mode = CONST_MODE_CONSOLE;
  char command[26] = ">                      ";
  char canary[5] = "R05e";
  uint8_t name[256];
  uint16_t hexdumpaddr;
  uint32_t rfChannel;
  uint8_t rows;         // screen rows
  uint8_t columns;      // screen columns
} global;

void reset(){
  EEPROM.write(CONST_MEM_NAME,0xFF);
  getName();
}

void setup() {
  // put your setup code here, to run once:
  SRXEInit(0xe7, 0xd6, 0xa2); // initialize display
  SRXEWriteString(0,120,global.command, FONT_LARGE, 3, 0); // draw large black text at x=0,y=120, fg=3, bg=0
  global.rows = 6;

  for(int i = 0; i < global.rows; i++){
    strncpy(messages[i], "                        ",24);
  }
  resetInputBuffer();
  global.mode = CONST_MODE_CONSOLE;
  global.rfChannel = 11;
  rfBegin(global.rfChannel);

  // set up the watchdog timer
  wdt_enable(WDTO_8S);

  // set up the name in memory
  if(getName())
  {

    if(infected)
    {
      omghax();
      return;
    }
    snprintf(outbuff,24,"Welcome back %s!",global.name);
    submit(outbuff);

    snprintf(outbuff,24,"You have %lu HUG tokens.",tokens);
    submit(outbuff);
    submit("Press % or > to submit.");
  }
}

void clearScreen(){
  for(int i = 0; i < global.rows; i++){ submit("                       "); }
  row = 0;
}

int getName()
{
  // flash is totally empty, welcome the new user!
  if(EEPROM.read(CONST_MEM_NAME)==255)
  {
    clearScreen();
    submit(version);
    submit("What is your name?");
    submit("type 'set name <name>'");
    submit("Press % or > to submit.");

    EEPROM.put(CONST_MEM_HUGS,(unsigned long)0);
    EEPROM.put(CONST_MEM_HACK,(int)0);
    return false;
  }

  // how many tokens we got?
  EEPROM.get(CONST_MEM_HUGS,tokens);
  EEPROM.get(CONST_MEM_HACK,infected);
  
  for(int i = 0; i < 4; i++)
  {
    char nameByte = EEPROM.read(CONST_MEM_NAME+i);
    //snprintf(outbuff,20,"%d %02x %03d : %c",i,nameByte,nameByte,nameByte);
    //submit(outbuff);

    if(nameByte >= 0x20 && nameByte <= 0x7A)
    {
      global.name[i] = nameByte;
      //break;
    }
    else
    {
      global.name[i] = 0x00;
    }
  }
  global.name[4]=0;

  return true;
}

int setName(char *name)
{
  if(strlen(name)<=0||strlen(name)>3)
  {
    submit("NAME MUST BE 1-3 CHARS!");
    return false;
  }
  
  for(int i = 0; i < 4; i++)
  {
    EEPROM.write(CONST_MEM_NAME+i,name[i]);
    global.name[i]=name[i];
    if(name[i] == 0x00)
    {
      break;
    }
  }

  return true;
}

void resetInputBuffer()
{
  memcpy(global.command, ">                      ",26);
  curs = 2;
  SRXEWriteString(0,120,global.command, FONT_LARGE, 3, 0);
}

void redraw()
{
  SRXEWriteString(0,0  ,"                         ", FONT_LARGE, 3, 0);
  SRXEWriteString(0,20 ,"                         ", FONT_LARGE, 3, 0);   
  SRXEWriteString(0,40 ,"                         ", FONT_LARGE, 3, 0);
  SRXEWriteString(0,60 ,"                         ", FONT_LARGE, 3, 0);
  SRXEWriteString(0,80 ,"                         ", FONT_LARGE, 3, 0);
  SRXEWriteString(0,100,"                         ", FONT_LARGE, 3, 0); 

  SRXEWriteString(0,0  ,messages[(0+row)%global.rows], FONT_LARGE, 3, 0);
  SRXEWriteString(0,20 ,messages[(1+row)%global.rows], FONT_LARGE, 3, 0);   
  SRXEWriteString(0,40 ,messages[(2+row)%global.rows], FONT_LARGE, 3, 0);
  SRXEWriteString(0,60 ,messages[(3+row)%global.rows], FONT_LARGE, 3, 0);
  SRXEWriteString(0,80 ,messages[(4+row)%global.rows], FONT_LARGE, 3, 0);
  SRXEWriteString(0,100,messages[(5+row)%global.rows], FONT_LARGE, 3, 0); 
}

void handleInput(char* cmd)
{
  if(!memcmp(cmd,"set",3))
  {
      char* item = strtok(cmd," ");
      if(item){
        item = strtok(NULL," ");
        if(item){
          if(!memcmp(item,"name",4))
          {
            item = strtok(NULL," ");
            if(setName(item))
            {
              snprintf(outbuff,24,"Hello '%s', welcome!",item);
              submit(outbuff);
            }
            //getName();
          }
          else if(!memcmp(item,"channel",7))
          {
            item = strtok(NULL," ");
            int newchan=atoi(item);
            if(newchan>=11 && newchan<=26)
            {
              global.rfChannel = newchan;
              rfBegin(global.rfChannel);
 
              snprintf(outbuff,24,"Radio on channel %d",global.rfChannel);
              submit(outbuff); 
            }
            else
            {
              submit("Channel must be 11-26");
            }
          }
          else if(!memcmp(item,"tokens",6))
          {
            submit("No.");
          }
          else{
            submit("Set what? name channel");
          }
        }else{
          submit("Set what? name channel");
        }
      }
  }else if(!memcmp(cmd,"get",3)){
    char* item = strtok(cmd," ");
    if(item){
      item = strtok(NULL, " ");
      if(item){
        if(!memcmp(item,"name",4))
        {
          getName();
          submit((char*)global.name);
        }
        else if(!memcmp(item,"timer",5))
        {
          snprintf(outbuff,20,"Timer is at %lu",time_loop);
          submit(outbuff);
        }
        else if(!memcmp(item,"channel",7))
        {
          snprintf(outbuff,20,"Radio on channel %d",global.rfChannel);
          submit(outbuff);
        }
        else if(!memcmp(item,"tokens",6))
        {
          snprintf(outbuff,24,"You have %lu HUG tokens.",tokens);
          submit(outbuff);
        }

        else
        {
          submit("Get what?");
          submit("name channel tokens");
        }
      }else{
        submit("Get what?");
        submit("name channel tokens");
      }
    }
    }else if(!memcmp(cmd,"hex",3)){
    global.mode = CONST_MODE_HEX;
    global.hexdumpaddr = 0;
    // fill the first 6 lines of the screen
    while(global.hexdumpaddr < (6*4))
    {
        readHexLine(global.hexdumpaddr);
        global.hexdumpaddr += 4;
    }
  }
  else if(!memcmp(cmd,"rssi",4))
  {
    for(int i=0;i<30;++i)rssi[i]=0;
    global.mode = CONST_MODE_RSSI;
    SRXEPowerDown();
    SRXEPowerUp();
  }
  else if(!memcmp(cmd,"help",4))
  {
    submit("Basic commands:");
    submit(" get set hug mine rssi");
    if(tokens>=100)
    {
      submit(" also 'WANNAHUG'..");
    }
  }
  else if(!memcmp(cmd,"resetforreals",13)){reset();}
  else if(!memcmp(cmd,"reset",5)){submit("Try 'resetforreals'");}
  else if(!memcmp(cmd,"version",7)){submit(version);}
  else if(!memcmp(cmd,"ascii",5)){
    for(int x=0;x<256;++x)
    {
      snprintf(outbuff,20,"%02x %03d : %c",x,x,x);
      delay(1000);
      submit(outbuff);
    }
  }
  else if(!memcmp(cmd,"unlock",6))
  {
    if(tokens>=10)
    {
      tokens-=10;
      infected=false;
      EEPROM.put(CONST_MEM_HACK,infected);
      EEPROM.put(CONST_MEM_HUGS,tokens);
      submit("SYSTEM UNLOCKED");      
    }
    else
    {
      submit("You need more HUGs");
    }
  }
  else if(!memcmp(cmd,"mine",4))
  {
    unsigned int mined = time_loop/10000;
    
    tokens = tokens+mined;
    EEPROM.put(CONST_MEM_HUGS,tokens);

    snprintf(outbuff,24,"Mined %u HUG tokens.",mined);
    submit(outbuff);
  }
  else if(!memcmp(cmd,"hug",3))
  {
    rfWrite('h');
    rfWrite('u');
    rfWrite('g');
    rfWrite('\n'); // write the last byte
    submit("Hug sent!!");
    if(tokens>0)
    {
      tokens=tokens-1;
      EEPROM.put(CONST_MEM_HUGS,tokens);
    }
  }
  else if(!memcmp(cmd,"WANNAHUG",8))
  {
    if(tokens<100)
    {
      submit("Who told you that cmd?");
      return;
    }
    
    rfWrite('h');
    rfWrite('a');
    rfWrite('x');
    rfWrite('\n'); // write the last byte
    submit("WANNAHUG sent!!");
  }
  // doesn't look like a command, so let's blast it to the chat!
  else
  {
    // make sure they have their name set
    if(!getName())return;
    
    char outputbuff[26];
    
    memset(outbuff,0,25);
    snprintf(outbuff,24,"%-3s:%s.",global.name,global.command+2);
 
    // transmit
    for(int i=0;i<curs+2;++i){
      rfWrite(outbuff[i]);
    }
    rfWrite('\n'); // write the last byte
        
    submit(outbuff);
  }
}

// submit(char*)
//  Prints the designated string to the screen buffer and forces a redraw
int submit(char* submission)
{
  time_loop=0; // reset the sleep timer
  if(strlen(submission)<3)return false;
  if(strlen(submission)>24)return false;
   
  memcpy(messages[row],"                       ",24);
  messages[row][24]=0;
  memcpy(messages[row],submission, 24);
  row = (row+1)%global.rows;
  redraw();
}

void checkCanary()
{
  //if(memcmp(command+canaryOffset,"R05e",4) != 0) {
  if(memcmp(global.canary,"R05e",4) != 0) {
    for(int i = 0; i < 6; i++){ strcpy(messages[i],"OMGH@XXE!"); }
    //redraw();
  }
}

// readHexLine(uint32_t)
//  Read 4 bytes starting from the specified address and print it to the string formatted as:
//     <address> <hex bytes> <character bytes>
void readHexLine(uint32_t addr)
{
  byte foo[4];
  char bar[26];
  for(int j = 0; j < 4; j++){ foo[j] = EEPROM.read(addr+j); }
  snprintf(bar, 24, "%04x %02x%02x%02x%02x %c%c%c%c", addr, foo[0],foo[1],foo[2],foo[3], foo[0],foo[1],foo[2],foo[3]);
  bar[24] = 0x00;
  submit(bar);
}

void updateInputBuffer(byte k){
  if(k >= 0x20 && k <= 0x7A){
    //is it printable?
    if(curs <= 20)
    {
      global.command[curs]=k;
      curs++;
    }
    
  }else if(k == 0x08){
    // backspace?
    if(curs > 2){
      curs--;
    }
    global.command[curs] = 0x20; //space is your blank character
  }

  SRXEWriteString(0,120,global.command, FONT_LARGE, 3, 0);
}

// mode_hex_loop(byte)
//  This is the loop function for when the device is in hex-dump mode.
//  TODO:
//    - Enable reading backwards
void mode_hex_loop(byte k){
  if(k){
    readHexLine(global.hexdumpaddr);
    if(global.hexdumpaddr <= (4*1024))
    {
      global.hexdumpaddr += 4;
    }
  }
}

// mode_console_loop(byte)
//  This is the loop function for when the device is in console (default) mode
void mode_console_loop(byte r, byte k){

  if(r){  
    if(r=='\n')
    {
      // HERE IS WHERE WE ACCEPT COMMANDS FROM THE NETWORK
      if(!memcmp(netbuff,"hug",3))
      {
        submit("OMG YOU'VE BEEN HUGGED!");
        tokens=tokens+1;
        EEPROM.put(CONST_MEM_HUGS,tokens);
      }
      else if(!memcmp(netbuff,"hax",3))
      {
        omghax();
        infected=true;
        EEPROM.put(CONST_MEM_HACK,infected);
      }
      else submit(netbuff);
      strncpy(netbuff,"                       ",24);
      ncurs = 0;
    }
    else
    {    
      netbuff[ncurs]=r;
      ncurs = (ncurs+1)%24;
    } 
  }
  
  
  if(k){
    // submit on "return" (key right of 'Sym', box line box)
    if(k==0x0D || k==0x03)
    {
      if(curs>3)
      {
        if(infected==true)
        {
          // while infected, special case to unlock
          if(!memcmp(global.command+2,"unlock",6))
          {
            handleInput(global.command+2);
            resetInputBuffer();
            return;
          }
          
          omghax();
          
          resetInputBuffer();
          return;
        }

        handleInput(global.command+2);
        resetInputBuffer();
      }
    }else{
      //update input buffer
      updateInputBuffer(k);
    }
  }
}

void omghax()
{
  clearScreen();
  submit("YOU HAVE BEEN INFECTED");
  submit("WITH ** WANNAHUG **");
  submit("YOU MUST PAY 10 HUG");
  snprintf(outbuff,24,"You have %lu HUG tokens.",tokens);
  submit(outbuff);
  submit("Type 'unlock' when ready");
}

// mode_rssi_loop()
//  This is a port of earlier RSSI code
void mode_rssi_loop(){

 time_loop = 0;

 int x;
 int y;

 for(int i=11;i<=26;++i)
 {
  rfBegin(i); // change channel
  delay(100);

  y=(i-11)%8*17;
  if(i<19){ x = 0;}
  else { x = 201; }
  
  if(rfAvailable())
  {
    while(0>rfRead()){} // burn through the reads
    rssi[i]+=rssiRaw;
  }
  
  snprintf(outbuff,24,"Ch %d : %d",i,rssi[i]);
  SRXEWriteString(x,y ,outbuff, FONT_LARGE, 3, 0);

 }
 // point out the "back" button
 SRXEWriteString(360,120 ,"back", FONT_SMALL, 3, 0);
}

void loop() {

  // reset the watchdog
  wdt_reset();

  // increment the sleep timer
  time_loop+=1;
  if(time_loop>50000) // maybe a minute or so?
  {
    time_loop=0;
    wdt_disable();
    SRXESleep();
    wdt_enable(WDTO_8S);
    redraw();
    resetInputBuffer();
  }

  
  // if we hear something on the radio, build up the net buffer
  byte r = NULL;
  if (rfAvailable())  
  {
    r = rfRead();
  }

  // otherwise, just take data from the keyboard
  byte k = SRXEGetKey();
  if(k){
    if(k == CONST_KEY_EXIT){
      SRXEPowerDown();
      SRXEPowerUp();
      global.mode = CONST_MODE_CONSOLE;
    }else if(k == CONST_KEY_CLEAR){
      clearScreen();
      k = NULL;   // prevents subprograms from operating on this input
    }
  }
  
  // figure out our mode
  switch(global.mode)
  {
    case CONST_MODE_HEX:
      mode_hex_loop(k);
      break;
    case CONST_MODE_RSSI:
      mode_rssi_loop();
      break;
    case CONST_MODE_CONSOLE:
    default:
      mode_console_loop(r,k);
  }
  //redraw();
}
