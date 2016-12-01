#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>

#include "Game_2048.h";

#define POSITION_X 20
#define POSITION_Y 13
#define TILE_HEIGHT 27
#define TILE_WIDTH 27

String tileValueString;
char tileValueCharArray[5];

void setup() {
  randomSeed(analogRead(0));

  EsploraTFT.begin();
  
  EsploraTFT.background(255, 255, 255);
  
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.fill(0, 0, 0);
  
  drawPoints();
  drawMatrix();
 
  getAvailableCells();
}

void loop() {
  int xValue = Esplora.readJoystickX();
  int yValue = Esplora.readJoystickY();
  int startButton = Esplora.readButton(SWITCH_RIGHT);

  if ( startButton == LOW ) {
    pressedButtonStart();
  } else if ( yValue < -400 && gameStarted ) {
    joystickU();
  } else if ( yValue > 400 && gameStarted ) {
    joystickD();
  } else if ( xValue > 400 && gameStarted ) {
    joystickL();
  } else if ( xValue < -400 && gameStarted ) {
    joystickR();
  }
  
  delay(100);

  if ( gameStarted ) {  
    drawPoints();
    drawMatrix();
  }

  if ( !gameOver && !furtherMovesArePossible() ) {
    delay(1000);
    stopGame();
    drawPoints();
    drawMatrix();
  }
  
}

void joystickL() {
  delay(10);

  if ( Esplora.readJoystickX() > 300 ) {
    
    erasePoints();
    moveCellsL();
    drawPoints();
    
    while (Esplora.readJoystickX() > 300) {
      delay(50);
    }
    
    delay(50);
  }
}

void joystickR() {
  delay(10);

  if ( Esplora.readJoystickX() < -300 ) {
    
    erasePoints();
    moveCellsR();
    drawPoints();
    
    while (Esplora.readJoystickX() < -300 ) {
      delay(50);
    }
    
    delay(50);
  }
}

void joystickU() {
  delay(10);

  if ( Esplora.readJoystickY() < -300 ) {
    
    erasePoints();
    moveCellsU();
    drawPoints();
    
    while (Esplora.readJoystickY() < -300) {
      delay(50);
    }
    
    delay(50);
  }
}

void joystickD() {
  delay(10);

  if ( Esplora.readJoystickY() > 300 ) {
    
    erasePoints();
    moveCellsD();
    drawPoints();
    
    while (Esplora.readJoystickY() > 300) {
      delay(50);
    }
    
    delay(50);
  }
}

void pressedButtonStart() {
  delay(10);

  if (Esplora.readButton(SWITCH_RIGHT) == LOW) {
    erasePoints();
    startGame();
    
    while (Esplora.readButton(SWITCH_RIGHT) == LOW) {
      delay(10);
    }
  }
}

void drawMatrix() {
  int xOffset;
  int posX;
  int posY;
  for (unsigned int row = 0; row < 4; row++) {
    for (unsigned int col = 0; col < 4; col++) {
      posX = col * (TILE_WIDTH + 2) + POSITION_X;
      posY = row * (TILE_HEIGHT + 2) + POSITION_Y;
      switch ( cells[row *4 + col] ) {
        case 2:
          xOffset = 12;
          EsploraTFT.fill(0xEE, 0xE4, 0xDA);
          break;
        case 4:
          xOffset = 12;
          EsploraTFT.fill(0xED, 0xE0, 0xc8);
          break;
        case 8:
          xOffset = 12;
          EsploraTFT.fill(0xF9, 0xF6, 0xF2);
          break;
        case 16:
          xOffset = 8;
          EsploraTFT.fill(0xF5, 0x95, 0x63);
          break;
        case 32:
          xOffset = 9;
          EsploraTFT.fill(0xF6, 0x7C, 0x5F);
          break;
        case 64:
          xOffset = 9;
          EsploraTFT.fill(0xF6, 0x5E, 0x3B);
          break;
        case 128:
          xOffset = 6;
          EsploraTFT.fill(0xED, 0xCF, 0x72);
          break;
        case 256:
          xOffset = 6;
          EsploraTFT.fill(0xED, 0xCC, 0x61);
          break;
        case 512:
          xOffset = 6;
          EsploraTFT.fill(0xED, 0xC8, 0x50);
          break;
        case 1024:
          xOffset = 3;
          EsploraTFT.fill(0xED, 0xC5, 0x3F);
          break;
        case 2048:
          xOffset = 3;
          EsploraTFT.fill(0xED, 0xC2, 0x2E);
          break;
        default:
          EsploraTFT.fill(0xBB, 0xAD, 0xA0);
      }
      EsploraTFT.rect( posX, posY, TILE_WIDTH, TILE_HEIGHT);
      if ( ( !gameOver ) && ( cells[row * 4 + col] > 0 ) ) {
        tileValueString = String( cells[row * 4 + col] );
        tileValueString.toCharArray(tileValueCharArray, 5);
        EsploraTFT.text( tileValueCharArray , posX + xOffset, posY + 10 ); 
      }
    }
  }
  if ( gameOver ) {
    drawGameOver();
  }
}

void erasePoints() {
  EsploraTFT.stroke(255,255,255);
  
  tileValueString = String( points );
  tileValueString.toCharArray(tileValueCharArray, 7);
  EsploraTFT.text( tileValueCharArray, 52, 2); 
  
  EsploraTFT.stroke(0,0,0);
}

void drawPoints() {
  EsploraTFT.setTextSize(1);
  // write the text to the top left corner of the screen
  EsploraTFT.text("Points: ", 2, 2);
  
  tileValueString = String( points );
  tileValueString.toCharArray(tileValueCharArray, 7);
  EsploraTFT.text( tileValueCharArray, 52, 2); 
}

void drawGameOver() {
  char gameOverStr[10] = "Game Over";
  char gameOverChar[2] = " ";
  
  int posX;
  
  for (unsigned int col = 0; col < 4; col++) {
    posX = col * (TILE_WIDTH + 2) + POSITION_X + 12;
    
    tileValueString = String( gameOverStr[col] );
    tileValueString.toCharArray(tileValueCharArray, 5);
      
    EsploraTFT.stroke( 0, 0, 0 );
    EsploraTFT.text( tileValueCharArray, posX, (TILE_WIDTH + 2) + POSITION_Y + 10 ); 
  }
    
  for (unsigned int col = 0; col < 4; col++) {
    posX = col * (TILE_HEIGHT + 2) + POSITION_X + 12;
    
    tileValueString = String( gameOverStr[col + 5 ] );
    tileValueString.toCharArray(tileValueCharArray, 5);
      
    EsploraTFT.stroke( 0, 0, 0 );
    EsploraTFT.text( tileValueCharArray, posX, 2 * (TILE_WIDTH + 2) + POSITION_Y + 10 ); 
  }
}
