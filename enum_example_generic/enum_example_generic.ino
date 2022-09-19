
enum directions {unknown, top, right, bottom, left, up, down};
//enum roverCFacing {unknown, top, right, bottom, left};
//enum moveDirection {unknown1, up, down, left1, right1};

void setup() {
  Serial.begin(115200);
  delay(1000);
  directions roverCFacing = unknown;

  if (roverCFacing == unknown) {Serial.println("I don't have a clue");}
directions moveDirection = up;

if (roverCFacing == unknown && moveDirection == up) {Serial.println("Don't think I can do that");}

moveDirection = unknown;

if (roverCFacing == unknown && moveDirection == unknown) {Serial.println("I lost"); }


}

void loop() {
  // put your main code here, to run repeatedly:

}
