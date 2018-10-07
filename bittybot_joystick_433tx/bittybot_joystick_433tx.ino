#include <VirtualWire.h>

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

byte xval, yval;
signed int x,y;
char hex[9] = {0};


void setup() {
  for (int i; i < 7; i++)
  {
   pinMode(buttons[i], INPUT);
   digitalWrite(buttons[i], HIGH);
  }
  Serial.begin(9600);
  vw_set_tx_pin(10);
  vw_setup(2000);
}

void loop() {
  //Serial.print("UP = "),Serial.print(digitalRead(up_button)),Serial.print("\t");
  //Serial.print("DOWN = "),Serial.print(digitalRead(down_button)),Serial.print("\t");
  //Serial.print("LEFT = "),Serial.print(digitalRead(left_button)),Serial.print("\t");
  //Serial.print("RIGHT = "),Serial.print(digitalRead(right_button)),Serial.print("\t");
  //Serial.print("START = "),Serial.print(digitalRead(start_button)),Serial.print("\t");
  //Serial.print("SELECT = "),Serial.print(digitalRead(select_button)),Serial.print("\t");
  //Serial.print("ANALOG = "),Serial.print(digitalRead(joystick_button)),Serial.print("\t");
  Serial.print("X = "),Serial.print(abs(map(analogRead(joystick_axis_x), 0, 1023, -255, 255)));Serial.print("\t");
  Serial.print("Y = "),Serial.print(abs(map(analogRead(joystick_axis_y), 0, 1023, -255, 255)));Serial.print("\n");  
  //Serial.print("X = "),Serial.print(analogRead(joystick_axis_x));Serial.print("\t");
  //Serial.print("Y = "),Serial.print(analogRead(joystick_axis_y));Serial.print("\n");  
   Serial.print("X = "),Serial.print(map(analogRead(joystick_axis_x), 0, 1000, -1, 1));Serial.print("\t");
  Serial.print("Y = "),Serial.print(map(analogRead(joystick_axis_y), 0, 1000, -1, 1));Serial.print("\n");
  delay(100);
  xval = abs(map(analogRead(joystick_axis_x), 0, 1023, -255, 255));
  yval = abs(map(analogRead(joystick_axis_y), 0, 1023, -255, 255));
 x = map(analogRead(joystick_axis_x), 0, 1000, -1, 1);
 y = map(analogRead(joystick_axis_y), 0, 1000, -1, 1);
 
 sprintf(hex, "%02X%02X %02d%02d", xval, yval, x, y);
 vw_send((uint8_t *)hex, strlen(hex));
 vw_wait_tx();
 delay(100);
 }