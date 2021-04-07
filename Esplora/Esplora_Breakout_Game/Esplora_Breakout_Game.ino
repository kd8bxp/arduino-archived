/* EsploraTFT Break Out game by Joel Krueger 12-5-2013
 
 Use the Esplora slider to control the paddle
 Press the left switch momentarily to turn sound on/off
 
 Hardware is an Arduino Esplora http://arduino.cc/en/Main/ArduinoBoardEsplora
 with an Adafruit 1.8" TFT breakout board http://www.adafruit.com/products/358
 This code used the stock arduino libraries so the arduino TFT screen should work fine
 */

#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>

int screenX = EsploraTFT.width();
int screenY = EsploraTFT.height();
int ballX = screenX / 2;      //ball start position
int ballY = screenY / 2;
int ballLastX = ballX;        //position for erasing the ball
int ballLastY = ballY;
int ballXDir = 1;             //ball direction across screen
int ballYDir = -2;            //ball direction and speed, up and down screen
int ballRadius = 5;           //ball size
int ballXmax = 8;             //ball max speed left to right
unsigned long time;           //used for refreshing ball on screen
unsigned long waitUntil;
int ballDelay = 30;           //milliseconds to wait to refresh ball position
int paddleX = screenX / 2;    //paddle start position
int paddleY = screenY - 5;
int paddleLastX = ballX;      //gives the last paddle position something
int paddleW = 20;             //width of paddle
const int bricksWide = 14;    //number of bricks across the screen
const int bricksTall = 10;    //number of bricks down the screen
const int brickW = 10;        //width of bricks in pixels
const int brickH = 5;         //highth of brick in pixels
int totalBricks = 0;          //tracks how many bricks are drawn on the screen
int bricksHit = 0;            //track how many bricks have been hit
boolean brick[bricksWide][bricksTall];    //tracks if each individual brick is active
boolean sound = HIGH;         //a flag to turn sound on/off

void setup(){
  // initialize the display
  EsploraTFT.begin();
  // set the background the black
  EsploraTFT.background(0,0,0);
  newScreen();               //routine to redraw a fresh screen
}
void loop(){
  paddle();                  //routine to read the slider and draw the paddle
  checkSound();              //routine to see if the button was pressed for sound on/off
  //this section tracks the time and only runs at intervals set by ballDelay
  time=millis();
  if (time>waitUntil){       //when its time to refresh the ball
    waitUntil=time+ballDelay;//add the delay until the next refresh cycle
    moveBall();              //routine to calculate new position and if the ball runs into anything
  }
}

void paddle(){
  //read the slider, map it to the screen width, then subtract the witdh of the paddle
  //this gives us the position relative the left corner of the paddle
  paddleX=map(Esplora.readSlider(),0,1023,screenX,0)-paddleW/2;
  if (paddleX<1){  //if the paddle tries to go too far left
    paddleX=1;     //position it on the far left
  }
  if (paddleX>screenX-paddleW){  //if the paddle tries to go too far right
    paddleX=screenX-paddleW;     //set the position to the far right - the paddle width
  }
  //this checks to see if the paddle has moved from the last position
  if (paddleX != paddleLastX) {    //if the new position is not equal to the old position
    EsploraTFT.fill(0,0,0);        //erase the old paddle
    EsploraTFT.rect(paddleLastX,paddleY,paddleW,4);
    EsploraTFT.fill(255,255,255);  //draw the new paddle
    EsploraTFT.rect(paddleX,paddleY,paddleW,4);
    paddleLastX=paddleX;           //assign the last posisition ot the new position
  }
}

void checkSound(void){
  if (Esplora.readButton(SWITCH_LEFT) == LOW){    //if this button is pressed
    sound = !sound;                               //toggle the sound flag on/off
    delay(250);                                   //a little delay to release the button
  }
}

void moveBall(void){
  //check if the ball hits the side walls
  if (ballX<ballXDir*-1 | ballX>screenX-ballXDir-ballRadius){
    ballXDir = -ballXDir;
    if (sound == HIGH){
      Esplora.tone(230,10);
    }
  }
  //check if the ball hits the top of the screen
  if (ballY<ballYDir*-1){
    ballYDir = -ballYDir;
    if (sound == HIGH){
      Esplora.tone(530,10);
    }
  }
  //check if ball hits bottom of bricks
  //we run through the array, if the brick is active, check its position against the balls position
  for (int a=0; a<bricksWide; a++){
    for (int b=0; b<bricksTall; b++){
      if (brick[a][b]==HIGH){
        if (ballX>a*brickW+10-ballRadius & ballX<a*brickW+brickW+10 & ballY>b*brickH+10-ballRadius & ballY<b*brickH+brickH+10){
          //we determined that a brick was hit
          EsploraTFT.fill(0,0,0);        //erase the brick
          EsploraTFT.rect(a*brickW+10,b*brickH+10,brickW,brickH);
          brick[a][b]=LOW;               //set the brick inactive in the array
          ballYDir = -ballYDir;          //change ball direction
          bricksHit=bricksHit+1;         //add to the bricks hit count
          if (sound == HIGH){
            Esplora.tone(330,10);
          }
        }
      }
    }
  }
  //check if the ball hits the paddle
  if (ballX>paddleX-ballXDir-ballRadius & ballX<paddleX+paddleW+ballXDir*-1 & ballY>paddleY-ballYDir-ballRadius & ballY<paddleY){
    ballXDir = ballXDir-(((paddleX+paddleW/2-ballRadius/2)-ballX)*.3);    //change ball angle in relation to hitting the paddle
    if (ballXDir<-ballXmax){      //this wont allow the ball to go too fast left/right
      ballXDir=-ballXmax;
    }
    else if (ballXDir>ballXmax){  //this wont allow the ball to go too fast left/right
      ballXDir=ballXmax;
    }
    ballYDir = -ballYDir;         //change direction up/down
    if (sound == HIGH){
      Esplora.tone(730,10);
    }
  }
  //check if the ball went past the paddle
  if (ballY>paddleY+10){
    if (sound == HIGH){
      Esplora.tone(130,1000);
    }
    for (int i=0; i<200; i++){
      EsploraTFT.stroke(0,255,0);
      EsploraTFT.text("OOPS",30,100);
      EsploraTFT.stroke(255,0,0);
      EsploraTFT.text("OOPS",30,100);
    }
    EsploraTFT.stroke(0,0,0);
    delay(2000);
    newScreen();
    ballY=screenY / 2;
    bricksHit = 0;
  }
  //check if there are any more bricks
  if (bricksHit==totalBricks){
    EsploraTFT.fill(0,0,0);
    EsploraTFT.rect(ballX,ballY,ballRadius,ballRadius);
    if (sound == HIGH){
      Esplora.tone(400,250);
      delay(250);
      Esplora.tone(415,250);
      delay(250);
      Esplora.tone(430,500);
    }
    for (int i=0; i<500; i++){
      EsploraTFT.stroke(0,255,0);
      EsploraTFT.text("YOU WIN",30,100);
      EsploraTFT.stroke(0,0,255);
      EsploraTFT.text("YOU WIN",30,100);
    }
    EsploraTFT.stroke(0,0,0);
    delay(2000);
    newScreen();
    ballY = screenY / 2;
    ballX = screenX / 2;
    bricksHit = 0;
  }
  //calculate the new position for the ball
  ballX=ballX+ballXDir;  //move the ball x
  ballY=ballY+ballYDir;  //move the ball y
  //erase the old ball
  EsploraTFT.fill(0,0,0);
  EsploraTFT.rect(ballLastX,ballLastY,ballRadius,ballRadius);
  // draw the new ball
  EsploraTFT.fill(255,255,255);
  EsploraTFT.rect(ballX, ballY,ballRadius,ballRadius);
  //update the last ball position to the new ball position
  ballLastX = ballX;
  ballLastY = ballY;
}

void newScreen(void){            //this is the setup for clearing the screen for a new game
  EsploraTFT.background(0,0,0);  //set the screen black
  paddleLastX = ballX;           //set the last paddle position to something (makes the game draw a new paddle every time)
  paddle();                      //routine draws the paddle on the screen
  blocks();                      //routine draws the bricks on the screen
}

void blocks(void){
  //assign the individual bricks to active in an array
  totalBricks = 0;
  for (int a=0; a<bricksWide; a++){
    for (int b=0; b<bricksTall; b++){
      brick[a][b]=HIGH;
    }
  }
  //now run trough the array and draw the bricks on the screen
  for (int a=0; a<bricksWide; a++){
    for (int b=0; b<bricksTall; b++){
      int c = map(b,0,bricksWide,0,255);
      if (brick[a][b] == HIGH) {
        totalBricks += 1;
        EsploraTFT.fill(c,255-c/2,255);
        EsploraTFT.rect(a*brickW+10,b*brickH+10,brickW,brickH);
      }
    }
  }
}

