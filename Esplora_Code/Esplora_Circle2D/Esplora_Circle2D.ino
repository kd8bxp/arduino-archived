//Circle2D found at:
//https://github.com/lucasmasotti/arduino-esplora-games

#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>



#define MAX_MISSILES 15

typedef struct CircleStruct {
  int xPos;
  int yPos;
  int radius;
} 
Circle;

typedef struct MissileStruct {
  int xPos;
  int yPos;
  int mDirection; // 0 - up, 1 - right, 2 - down, 3 -left
} 
Missile;

typedef struct CoordinatesStruct {
  int x;
  int y;
} 
Coordinates;

int screenWidth = EsploraTFT.width()-1;
int screenHeight = EsploraTFT.height()-1;

int border = 1;
int topBorder = 0 + border;
int bottomBorder = screenHeight - border;
int leftBorder = 0 + border;
int rightBorder = screenWidth - border;

int backgroundColor[3] = {
  0, 0, 0};

Missile missilesList[MAX_MISSILES];
int missilesListSize = 0;

Circle circleA {
  EsploraTFT.width()/2, EsploraTFT.height()/2, 3};
Coordinates newCircleCoordinates = {
  0, 0};
Coordinates oldCircleCoordinates = {
  0, 0};

int buttonInterval = 200;
unsigned long switchRightPressedMillis = 0;
unsigned long switchLeftPressedMillis = 0;
unsigned long switchUpPressedMillis = 0;
unsigned long switchDownPressedMillis = 0;

void setup() {
  EsploraTFT.begin();
  Serial.begin(9600);
  setDefaultScenario();
} 

void loop() {
  unsigned long currentMillis = millis();

  int xAxis = Esplora.readJoystickX();
  int yAxis = Esplora.readJoystickY();
  Coordinates axesChanges = filterAxes(xAxis, yAxis);

  newCircleCoordinates = getNewCircleCoordinates(circleA, axesChanges);
  if(!coordinatesAreEqual(newCircleCoordinates, oldCircleCoordinates)) {
    moveCircle(&circleA, newCircleCoordinates);
  }
  oldCircleCoordinates = newCircleCoordinates;

  int switchUpButton = Esplora.readButton(SWITCH_UP);
  if(switchUpButton == LOW && (currentMillis - switchUpPressedMillis > buttonInterval)){
    launchMissile(circleA.xPos, circleA.yPos, 0);
    switchUpPressedMillis = millis();
  }
  int switchRightButton = Esplora.readButton(SWITCH_RIGHT);
  if(switchRightButton == LOW && (currentMillis - switchRightPressedMillis > buttonInterval)){
    launchMissile(circleA.xPos, circleA.yPos, 1);
    switchRightPressedMillis = millis();
  }

  int switchDownButton = Esplora.readButton(SWITCH_DOWN);
  if(switchDownButton == LOW && (currentMillis - switchDownPressedMillis > buttonInterval)){
    launchMissile(circleA.xPos, circleA.yPos, 2);
    switchDownPressedMillis = millis();
  }
  int switchLeftButton = Esplora.readButton(SWITCH_LEFT);
  if(switchLeftButton == LOW && (currentMillis - switchLeftPressedMillis > buttonInterval)){
    launchMissile(circleA.xPos, circleA.yPos, 3);
    switchLeftPressedMillis = millis();
  }

  missilesStep();

  delay(33);
}

void missilesStep() {
  int i;

  for(i = 0; i < missilesListSize; i++) {
    if(canMoveMissile(i)) {
      moveMissile(i);
    } 
    else {
      destroyMissile(i);
    }

  }
}

void destroyMissile(int missileIndex) {
  Missile missile = missilesList[missileIndex];
  eraseMissile(missile);
  removeMissile(missilesList, &missilesListSize, missileIndex);
}

boolean canMoveMissile(int missileIndex) {
  Missile missile = missilesList[missileIndex];

  int newX = missile.xPos;
  int newY = missile.yPos;

  switch(missile.mDirection) {
  case 0:
    newY = newY - 1;
    break;
  case 1:
    newX = newX + 1;
    break;
  case 2:
    newY = newY + 1;
    break;
  case 3:
    newX = newX - 1;
    break; 
  }

  if(newX >= rightBorder || newX <= leftBorder) {
    return 0;
  }
  if(newY >= bottomBorder || newY <= topBorder) {
    return 0;
  }
  return 1;
}

void moveMissile(int missileIndex) {
  Missile *missile = &missilesList[missileIndex];

  eraseMissile(*missile);
  switch(missile->mDirection) {
  case 0:
    missile->yPos = missile->yPos - 1;
    break;
  case 1:
    missile->xPos = missile->xPos + 1;
    break;
  case 2:
    missile->yPos = missile->yPos + 1;
    break;
  case 3:
    missile->xPos = missile->xPos - 1;
    break;
  }
  drawMissile(*missile);
}

void drawMissile(struct MissileStruct missile) {
  EsploraTFT.fill(255,255,255);
  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.point(missile.xPos, missile.yPos);
}

void eraseMissile(struct MissileStruct missile) {
  EsploraTFT.fill(0, 0, 0);
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.point(missile.xPos, missile.yPos);
}


void launchMissile(int initialX, int initialY, int mDirection) {
  Missile newMissile = {
    initialX, initialY, mDirection          };
  insertMissile(missilesList, &missilesListSize, newMissile);
}

boolean coordinatesAreEqual(struct CoordinatesStruct a, struct CoordinatesStruct b) {
  if(a.x == b.x && a.y == b.y) {
    return 1;
  } 
  else {
    return 0;
  }
}

struct CoordinatesStruct filterAxes(int xAxis, int yAxis) {
  int newX = 0;
  int newY = 0;

  if(xAxis > 10 || xAxis < -10) {
    newX = map(xAxis, -512, 512, 4, -4);
  } 

  if(yAxis > 10 || yAxis < -10) {
    newY = map(yAxis, -512, 512, -4, 4); 
  } 

  Coordinates coordinates {
    newX, newY      
  };

  return coordinates;

}

void moveCircle(struct CircleStruct *circle, struct CoordinatesStruct coordinates) { 
  eraseCircle(*circle);

  circle->xPos = coordinates.x;
  circle->yPos = coordinates.y;

  drawCircle(*circle);
}

struct CoordinatesStruct getNewCircleCoordinates(struct CircleStruct circle, struct CoordinatesStruct axesChanges) {
  int newX = circle.xPos + axesChanges.x;
  int newY = circle.yPos + axesChanges.y;

  if((newX + circle.radius) > rightBorder){
    newX = (rightBorder - circle.radius);
  }
  if((newX - circle.radius) < leftBorder){
    newX = (leftBorder + circle.radius);
  }

  if((newY + circle.radius) > bottomBorder){
    newY = (bottomBorder - circle.radius);
  }
  if((newY - circle.radius) < topBorder){
    newY = (topBorder + circle.radius);
  }

  Coordinates newCoordinates {
    newX, newY                        };

  return newCoordinates;
}

void drawCircle(struct CircleStruct circle) {
  EsploraTFT.noStroke();
  EsploraTFT.fill(0, 255, 0);
  EsploraTFT.circle(circle.xPos, circle.yPos, circle.radius);
}

void eraseCircle(struct CircleStruct circle) {
  EsploraTFT.noStroke();
  EsploraTFT.fill(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
  EsploraTFT.circle(circle.xPos, circle.yPos, circle.radius);
}

void setDefaultScenario() {
  clearBackground();
  drawBorders();
}

void clearBackground() {
  EsploraTFT.background(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
}

void drawBorders() {
  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.line(0, 0, 0, screenHeight); //Left border
  EsploraTFT.line(0, 0, screenWidth, 0); //Top border
  EsploraTFT.line(0, screenHeight, screenWidth, screenHeight); //Bottom border
  EsploraTFT.line(screenWidth, 0, screenWidth, screenHeight); //Right Border
}


/* Missile list function */

void insertMissile(MissileStruct missiles[], int *listSize, MissileStruct missile) {
  if(*listSize == MAX_MISSILES) {
    return;
  }
  missiles[*listSize] = missile;
  *listSize = *listSize + 1;
}

void removeMissile(MissileStruct missiles[], int *listSize, int missileIndex) {
  if(missileIndex  < 0 || missileIndex > *listSize) {
    return;
  }

  int i;
  for(i = 0 + missileIndex; i < *listSize; i++) {
    missiles[i] = missiles[i+1];
  }
  *listSize = *listSize - 1;
}
