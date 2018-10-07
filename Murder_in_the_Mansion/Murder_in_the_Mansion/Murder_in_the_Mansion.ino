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

String suspects[] ={"Butler", "Maid", "Nanny", "Burglar"};
String weapons[] = {"Hairpin", "Gun", "Poker", "Knife"};
String rooms[] = {"Pantry", "Bedroom", "Library", "Hallway"};
String rclues[] = {"Think Food", "Think Pillow", "Think Books", "Think Passage"};
String wclues[] = {"Clue: Coiffure", "Clue: Bullets", "Clue: Logs", "Clue: Roast Beef"};
String sclues[] = {"He Serves", "She Dusts", "She loves Kids", "He was not invited"};
int s; //random suspect
int w; //random weapon
int r; //random room
int incoming;

void setup()
{
	randomSeed(analogRead(1)+analogRead(2)+analogRead(3));
Serial.begin(9600);
Serial.print("Your Suspects are: ");
for (int i=0;i<4;i++) {
	Serial.print(suspects[i]);
	Serial.print(" ");
}
Serial.println("");
Serial.print("Your Weapons are: ");
for (int i=0;i<4;i++) {
	Serial.print(weapons[i]);
	Serial.print(" ");
}
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

void guessroom() {
	Serial.flush();

	while (Serial.available()) {
		 incoming = Serial.parseInt();
	}
	//if (incoming < 0 || incoming > 3) {guessroom();}
		 			if (incoming == r) {Serial.println("Correct!"); return;}
			else {Serial.println(rclues[r]); guessroom();}
}

void guessweapon() {
	Serial.flush();
	while (Serial.available()) {
		incoming = Serial.parseInt();
	}
	
	//if (incoming < 0 || incoming > 3) {guessweapon();}
	if (incoming == w) {Serial.println("Correct!"); return;}
	else {Serial.println(wclues[w]); guessweapon();
		}
	}


void guesssuspect() {
	Serial.flush();

	while (Serial.available()) {
		 incoming = Serial.parseInt();
			}
			//if (incoming < 0 || incoming > 3) {guesssuspect();}
			if (incoming == s) {Serial.println("Correct!"); return;}
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
