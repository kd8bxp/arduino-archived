/*
  Esplora TFT Snake

  this is an open source version of the classic snake game.
  Press one of the switches to change the direction of the 
  snake and move the slider to make the game faster or slower.
  The score is in the upper right corner of the screen. 
  You earn 9 point for each beacon the snake eats. Enjoy :)
  
  Created on 16 september 2013
  By Corrado Possieri
  
*/

// first of all we need to include the libraries
#include <Esplora.h>
#include <TFT.h>            // Arduino LCD library
#include <SPI.h>

// then we have to choice the resolution of our game. 
const int scale = 4;
int xScreen = EsploraTFT.width()/scale;
int yScreen = EsploraTFT.height()/scale;

// some global variables
const int snakeInitialLength = 12;
const int snakeMaxLength = 300; // max length of the snake
int snakeLength = snakeInitialLength; // length of the snake
int positions[2][snakeMaxLength]; // positions of the snake pieces
int beacon[2]; // poition of the beacon
int directionSnake = 0; // the direction where the snake moves
unsigned long lastTimeMoved; // last time in wich time snake moved
unsigned long score = (snakeLength - snakeInitialLength) * 9; //score
unsigned long lastScore = score; // score at the previous time
const int scorePosition = 135; // position of the score in the screen

void setup() {
  // we need to initialize the display
  EsploraTFT.begin();
  // and to select the color of the background
  EsploraTFT.background(0,0,0); // black for an higher contrast
  
  // initialize the position of the snake
  const int startX = 8; // position where the snake starts
  const int startY = 5;
  // draw these positions
  for(int ii = 0; ii < snakeLength; ii++) {
    positions[0][ii] = startX + ii;
    positions[1][ii] = startY;
    // draw some boxes to figure where is the snake
    EsploraTFT.fill(255, 255, 255); // drain in white
    EsploraTFT.rect(scale*positions[0][ii], scale*positions[1][ii],
      scale, scale); // the snake dimension are proportional to scale
  }
  delay(2000);
  for(int jj = 0; jj < snakeLength ; jj++) {
   Serial.println(positions[0][jj]); 
   Serial.println(positions[1][jj]); 
   Serial.println();
  }
  
  // to make the position of the beacon random 
  // use random seed with temperature
  randomSeed((long)Esplora.readTemperature(DEGREES_C)*100);
  
  // place the beacon
  placeBeacon();
  Serial.println(beacon[0]);
  Serial.println(beacon[1]);
  
  // write the score  
  EsploraTFT.stroke(255, 255, 255);
  // convert score into String
  String scoreString = String(score);
  // convert string into char array
  char charScore [4];
  scoreString.toCharArray(charScore, 4);
  // print it
  EsploraTFT.text(charScore, scorePosition, 0);
  EsploraTFT.noStroke();
}

void loop() {
  // the snake moves accordingly to the value read from potentiometer
  int slider = Esplora.readSlider();
  // map this value in a value of time
  unsigned int timeDelay = map(slider, 0, 1023, 50, 1000);
  if(millis() - lastTimeMoved > timeDelay) {
    // let the snake move
    moveTheSnake();
    lastTimeMoved = millis();
  }
  
  // read the buttons to change directions
  if (Esplora.readButton(SWITCH_DOWN) == LOW && directionSnake != 3) {
    // if the snake is going up it can't go down 
    // so we use that AND funciotn
    directionSnake = 1;
  }
  if (Esplora.readButton(SWITCH_LEFT) == LOW && directionSnake != 0) {
    // if the snake is going up it can't go down 
    // so we use that AND funciotn
    directionSnake = 2;
  }
  if (Esplora.readButton(SWITCH_UP) == LOW && directionSnake != 1) {
    // if the snake is going up it can't go down 
    // so we use that AND funciotn
    directionSnake = 3;
  }
  if (Esplora.readButton(SWITCH_RIGHT) == LOW && directionSnake != 2) {
    // if the snake is going up it can't go down 
    // so we use that AND funciotn
    directionSnake = 0;
  }
  
  // see it we hurt another part of the snake
  for(int kk = 0; kk < snakeLength - 1 ; kk++) {
    if(positions[0][kk] == positions[0][snakeLength - 1] &&
      positions[1][kk] == positions[1][snakeLength - 1]) {
        // draw a blue blackground
        EsploraTFT.background(0, 100, 150);
        // write in white
        EsploraTFT.stroke(255, 255, 255);
        // select the text size
        EsploraTFT.setTextSize(2);
        // "you Lose"
        EsploraTFT.text("You Lose", 0, 0);
        EsploraTFT.text("Press Reset", 0, 30);
        EsploraTFT.text("To Restart", 0, 60);
        // stop the game
        while(true) {
        }
      }
  }
  
  // update the score
  score = (snakeLength - snakeInitialLength) * 9;
  // if it is different from previous one
  if(score != lastScore) {
    // delete the old one
    EsploraTFT.stroke(0, 0, 0);
    // convert score into String
    String scoreString = String(lastScore);
    // convert string into char array
    char charScore [4];
    scoreString.toCharArray(charScore, 4);
    // print it
    EsploraTFT.text(charScore, scorePosition, 0);
    // print the new one
    EsploraTFT.stroke(255, 255, 255);
    // convert score into String
    scoreString = String(score);
    // convert string into char array
    scoreString.toCharArray(charScore, 4);
    // print it
    EsploraTFT.text(charScore, scorePosition, 0);
    // save the value of last score
    lastScore = score;
    // we don't want stroke no more
    EsploraTFT.noStroke();
  }
    
  
  // a little pause for stability
  delay(10);
}

void moveTheSnake() {
  // move the snake according to the direction
  switch(directionSnake) { 
    case 0: { // move right
      if(beaconIsEaten()) { // if we ate the beacon
        snakeLength++; // the snake increases in length
        positions[0][snakeLength - 1] = 
          (positions[0][snakeLength - 2] + 1) % xScreen;  
        // we use the modulus so when the snake goes out from a part
        // it returns in the opposite one
        // the y is the same as before   
        positions[1][snakeLength - 1] = positions[1][snakeLength - 2];   
        // draw the new head of the snake
        drawSnakeHead();
      } else {
        // update positions
        updatePositions();
        // move also the terminal of the snake
        positions[0][snakeLength - 1] =
          (positions[0][snakeLength - 1] + 1) % xScreen; 
        // draw the new head
        drawSnakeHead();
      }
      break;
    }
    
    case 1: { // move down
      if(beaconIsEaten()) { // if we ate the beacon
        snakeLength++; // the snake increases in length
        positions[1][snakeLength - 1] = 
          (positions[1][snakeLength - 2] + 1) % yScreen; 
        positions[0][snakeLength - 1] = positions[0][snakeLength - 2];
        // draw the new head of the snake
        drawSnakeHead();
      } else {
        // update positions
        updatePositions();
        // move also the terminal of the snake
        positions[1][snakeLength - 1] =
          (positions[1][snakeLength - 1] + 1) % yScreen; 
        // draw the new head
        drawSnakeHead();
      }
      break;
    }
    
    case 2: { // move left
      if(beaconIsEaten()) { // if we ate the beacon
        snakeLength++; // the snake increases in length
        positions[0][snakeLength - 1] = 
          (positions[0][snakeLength - 2] - 1);
        // if it goes utside replace on the other part
        if(positions[0][snakeLength - 1] <= 0) {
          positions[0][snakeLength - 1] = xScreen - 1;
        }
        positions[1][snakeLength - 1] = positions[1][snakeLength - 2];
        // draw the new head of the snake
        drawSnakeHead();
      } else {
        // update positions
        updatePositions();
        // move also the terminal of the snake
        positions[0][snakeLength - 1] =
          (positions[0][snakeLength - 1] - 1);
        if(positions[0][snakeLength - 1] <= 0) {
          positions[0][snakeLength - 1] = xScreen - 1;
        }
        // draw the new head
        drawSnakeHead();
      }
      break;
    }
    
    case 3: { // move up
      if(beaconIsEaten()) { // if we ate the beacon
        snakeLength++; // the snake increases in length
        positions[1][snakeLength - 1] = 
          (positions[1][snakeLength - 2] - 1);
        if(positions[1][snakeLength - 1] <= 0) {
          positions[1][snakeLength - 1] = yScreen - 1;
        } 
        positions[0][snakeLength - 1] = positions[0][snakeLength - 2];
        // draw the new head of the snake
        drawSnakeHead();
      } else {
        // update positions
        updatePositions();
        // move also the terminal of the snake
        positions[1][snakeLength - 1] =
          (positions[1][snakeLength - 1] - 1);
        if(positions[1][snakeLength - 1] <= 0) {
          positions[1][snakeLength - 1] = yScreen - 1;
        } 
        // draw the new head
        drawSnakeHead();
      }
      break;
    }
  }  
}

boolean beaconIsEaten() {
  // this function returns true if beacon is eated and 
  // false if it is not
  if(positions[0][snakeLength - 1] == beacon[0] && 
    positions[1][snakeLength - 1] == beacon[1]) {
    Serial.println("Beacon Eated");
    // put the beacon in another place
    placeBeacon();
    return true;
  } else {
    return false;
  }
}

void updatePositions() {
   // debugging
  Serial.println("Snake is Moving");
  for(int jj = 0; jj < snakeLength ; jj++) {
   Serial.println(positions[0][jj]); 
   Serial.println(positions[1][jj]); 
   Serial.println();
  } 
  // this function update the position of the snake except the 
  // head of the snake, which position is user choice dependent
  // make the snake move
  // first we have to delete the tail of the snake
  EsploraTFT.fill(0, 0, 0); // the same color of the background
  EsploraTFT.rect(scale * positions[0][0], scale * positions[1][0],
    scale, scale);
  // next update positions
  for(int ii = 0; ii < snakeLength - 1; ii++) {
    // update the positions
    positions[0][ii] = positions[0][ii + 1];
    positions[1][ii] = positions[1][ii + 1];
  }
}

void drawSnakeHead() {
  // this function draw the new head of the snake
  EsploraTFT.fill(255, 255, 255);
  EsploraTFT.rect(scale*positions[0][snakeLength - 1], 
    scale*positions[1][snakeLength - 1], scale, scale);
}

void placeBeacon() {
  // this function place the beacon where 
  // there is not a snake part
  // repeat while we don't break it
  while (true) {
    // define an int for verification
    int flag = 0;
    // don't put the beacon on the external of the screen
    const int tollerance = 5;
    // locate the beacon in a random place
    beacon[0] = (int) random(tollerance, xScreen - tollerance);
    beacon[1] = (int) random(tollerance, yScreen - tollerance);
    // find if a part of the snake is on the beacon
    for(int jj = 0; jj < snakeLength; jj++) {
      // if they have the same x
      if(beacon[0] == positions[0][jj]) { 
        // cheack the y
        if(beacon[1] == positions[1][jj]) { 
          // increment the flag
          flag++;
        }
      }
    }
    // if the flag is 0 we don't have the beacon over 
    // a part of the snake
    if(flag == 0) {
      // so we can go away
      break;
    }
  }
  
  // draw the beacon
  EsploraTFT.fill(0, 255, 0);
  EsploraTFT.rect(scale * beacon[0], scale * beacon[1], scale, scale);
}