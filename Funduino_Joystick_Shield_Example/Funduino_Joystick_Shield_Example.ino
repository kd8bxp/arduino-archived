/* 6th December 2013 - By Kyle Fieldus

This example sketch is designed to show the inputs and outputs of the Funduino Joystick Shield V1.A
The shield this sketch was developed with was provded by ICStation http://www.icstation.com/
*/

int up_button = 2;
int down_button = 4;
int left_button = 5;
int right_button = 3;
int start_button = 6;
int select_button = 7;
int joystick_button = 8;
int joystick_axis_x = A0;
int joystick_axis_y = A1;
int buttons[] = {up_button, down_button, left_button, right_button, start_button, select_button, joystick_button};


void setup() {
  for (int i; i < 7; i++)
  {
   pinMode(buttons[i], INPUT);
   digitalWrite(buttons[i], HIGH);
  }
  Serial.begin(9600);
}

void loop() {
  Serial.print("UP = "),Serial.print(digitalRead(up_button)),Serial.print("\t");
  Serial.print("DOWN = "),Serial.print(digitalRead(down_button)),Serial.print("\t");
  Serial.print("LEFT = "),Serial.print(digitalRead(left_button)),Serial.print("\t");
  Serial.print("RIGHT = "),Serial.print(digitalRead(right_button)),Serial.print("\t");
  Serial.print("START = "),Serial.print(digitalRead(start_button)),Serial.print("\t");
  Serial.print("SELECT = "),Serial.print(digitalRead(select_button)),Serial.print("\t");
  Serial.print("ANALOG = "),Serial.print(digitalRead(joystick_button)),Serial.print("\t");
  Serial.print("X = "),Serial.print(map(analogRead(joystick_axis_x), 0, 1000, -1, 1));Serial.print("\t");
  Serial.print("Y = "),Serial.print(map(analogRead(joystick_axis_y), 0, 1000, -1, 1));Serial.print("\n");  
  Serial.print("X = "),Serial.print(analogRead(joystick_axis_x));Serial.print("\t");
  Serial.print("Y = "),Serial.print(analogRead(joystick_axis_y));Serial.print("\n");  
  delay(100);
  
 }

