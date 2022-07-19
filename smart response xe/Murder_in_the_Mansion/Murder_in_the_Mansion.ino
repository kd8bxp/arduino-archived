/*This is a conversion of the Pocket-Basic game Murder in the Mansion
Conversion by LeRoy F. Miller original game published in "Murder in the Mansion and Other Computer Adventures in Pocket-Basic for the TRS-80" by Jim Cole
The game is pretty simple, you have 4 suspects to choice from, 4 weapons, and 4 rooms.
each is picked at "random" and you guess Weapon, Room (Where), and Who.
if you guess wrong you are given a clue, the clue will let you pick right the next time around. 
So a very simple game to solve. The game is played in the Serial Console, and later will be converted for the Hero Jr project to play it.
https://github.com/kd8bxp/Arduino-Mega-Hero-Jr-Project
and https://github.com/automation-technology-club/Star-Wars-Day-Code
That's about it.
*/
/* Jan 3, 2016 used Serial Read Example by Evil Chemist - 01/04/15) https://codebender.cc/sketch:72379#Serial_Reading.ino */
#include "fonts/BMplain.h" //small/medium font
#include "SmartResponseXE.h"

String suspects[] ={"Butler", "Maid", "Nanny", "Burglar"};
String weapons[] = {"Hairpin", "Gun", "Poker", "Knife"};
String rooms[] = {"Pantry", "Bedroom", "Library", "Hallway"};
String rclues[] = {"Think Food", "Think Pillow", "Think Books", "Think Passage"};
String wclues[] = {"Clue: Coiffure", "Clue: Bullets", "Clue: Logs", "Clue: Roast Beef"};
String sclues[] = {"He Serves", "She Dusts", "She loves Kids", "He was not invited"};
int s; //random suspect
int w; //random weapon
int r; //random room
String incoming;

char serByte;
char serBuffer[128];
char serInString[128];
int serIndex=0;

void setup()
{
  randomSeed(analogRead(1)+analogRead(2)+analogRead(3));
Serial.begin(9600);
SRXEInit();
Serial.print("Your Suspects are: ");
SRXEWriteString(0,0, "Your Suspects are: ", FONT_MEDIUM, BMplain, 3, 0);
//for (int i=0;i<4;i++) {
//  Serial.print(suspects[i]);
//  Serial.print(" ");
  //String temp1;
  //temp1 = suspects[0]; // + suspects[1] + suspects[2] + suspects[3];
  //temp1 = "Butler, Maid, Nanny, Burglar ";
  //char* temp;
  //temp1.toCharArray(temp, temp1.length()+1);
  SRXEWriteString(0,12, "Butler, Maid, Nanny, Burglar", FONT_MEDIUM, BMplain, 3, 0);
//}
Serial.println("");
Serial.print("Your Weapons are: ");
SRXEWriteString(0,24,"Your Weapons are:", FONT_MEDIUM, BMplain, 3, 0);
SRXEWriteString(0,36,"Hairpin, Gun, Poker, Knife", FONT_MEDIUM, BMplain, 3, 0);
//for (int i=0;i<4;i++) {
//  Serial.print(weapons[i]);
//  Serial.print(" ");
//}
Serial.println("");
Serial.print("The rooms are: ");
for (int i=0;i<4;i++) {
  Serial.print(rooms[i]);
  Serial.print(" ");
  }
Serial.println(" ");
Serial.println("Whodunit?");

Serial.println("");
Serial.println("Murder in the Mansion.");
Serial.println("");
Serial.println("A winter eve, A country mansion.....");
Serial.println("The Baron is dead! ");
Serial.println("");
Serial.println("It is your job to find out who did it, where it was done, and with what he was killed with.");

s =random(0,4);
w =random(0,4);
r =random(0,4);
}

void key() {
    if (Serial.available()) {
    memset(serBuffer, 0, sizeof(serBuffer));
    memset(serInString, 0, sizeof(serInString));
    Serial.readBytesUntil('#',serBuffer, 128);
    
  } else key();
}

void guessroom() {
  
key();
  incoming=String(r);
  String check = String(serBuffer);
          if (check == incoming) {Serial.println("Correct!"); return;}
      else {Serial.println(rclues[r]); guessroom();}
}

void guessweapon() {
  key();
  
  incoming=String(w);
  String check = String(serBuffer);
  if (check == incoming) {Serial.println("Correct!"); return;}
  else {Serial.println(wclues[w]); guessweapon();
    }
  }


void guesssuspect() {
  key();
  
  incoming=String(s);
  String check = String(serBuffer);
      if (check == incoming) {Serial.println("Correct!"); return;}
      else {Serial.println(sclues[s]); guesssuspect();}
}

void stop() {
  stop();
  
}

void loop()
{
  Serial.println("");
Serial.println("What room do you suspect? ");
for (int i=0;i<4;i++) {
  Serial.print(i);
  Serial.print(". ");
  Serial.print(rooms[i]);
  Serial.print(" ");
  }
  Serial.println("");
  
  guessroom();
  
  Serial.println("");
  Serial.println("Do you have a weapon in mind? ");
  for (int i=0;i<4;i++) {
    Serial.print(i);
    Serial.print(". ");
    Serial.print(weapons[i]);
    Serial.print(" ");
  }
  Serial.println("");
  
  guessweapon();
  
    Serial.println("");
  Serial.println("Ready to name a suspect? ");
  for (int i=0;i<4;i++) {
    Serial.print(i);
    Serial.print(". ");
    Serial.print(suspects[i]);
    Serial.print(" ");
  }
  Serial.println("");
  
  guesssuspect();
  
  Serial.println("");
  Serial.print("Well Done! It was indeed the ");
  Serial.print(suspects[s]);
  Serial.print(", with the ");
  Serial.print(weapons[w]);
  Serial.print(", in the ");
  Serial.print(rooms[r]);
  Serial.println(".");
  Serial.println("");
  Serial.println("Press RESET to Play again!");
  
  stop();
}
