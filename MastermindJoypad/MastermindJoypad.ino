#include <Joypad.h>
#include <SPI.h>
#include <TFT.h>
//#include <Serial.h>
#include "Game.h"

/* TFT pins */
#define LCD_CS   5
//#define SD_CS 4
#define DC   4
#define RESET  8

#define Shock 8

#define screenWidth 128
#define screenHeight 160

float x, y;
boolean button;

struct Color {
  int r, g, b;
};

const Color colors[] = {
  { 191, 0, 0 },
  { 0, 191, 0 },
  { 0, 0, 255 },
  { 127, 191, 0 },
  { 0, 191, 191 },
  { 191, 0, 255 },
};
const Color black = { 0, 0, 0 };
const Color white = { 255, 255, 255 };
const Color gray = { 63, 63, 63 };

TFT screen = TFT(LCD_CS, DC, RESET);

Game game;
  
int pointer;
boolean canGuess;

void setup() {
  screen.begin();
  screen.setRotation(4);
  randomSeed(analogRead(0));
  pinMode(Shock, OUTPUT);
  digitalWrite(Shock, LOW);
  Serial.begin(9600);
}

void loop() {
  startScreen();
  delay(2000);

/*
  drawDemo();
  drawWin();
  delay(5000);

  drawDemo();
  int code[] = { 1, 2, 3, 4 };
  drawLose(code);
  delay(5000);

  return;
*/
  
  clearScreen();
  game.init();
  for (int i = 0; i < game.pegs; i++) {
    Serial.print(game.code[i]);
  }
  Serial.println();
  
  while (!game.won && game.turn < game.turns) {
    int guess[game.pegs];
    for (int i = 0; i < game.pegs; i++) {
      guess[i] = 0;
    }
    pointer = 0;
    canGuess = false;
    
    drawPegs(game.turn, guess);
    drawPeg(game.turn, pointer, guess[pointer], true);
    drawGuessText(canGuess, false);
  
    while (true) {
      readJoypad();
      
      if (button && canGuess) {
        break;
      }
      
      boolean draw = false;
      
      if (x > 280) {
        pointer = max(0, pointer - 1);
        draw = true;
      } else if (x < -280) {
        pointer = min((/*canGuess*/ false ? game.pegs : game.pegs - 1), pointer + 1);
        draw = true;
      }
      
      if (y > 280) {
        guess[pointer] = guess[pointer] - 1;
        if (guess[pointer] < 1)
          guess[pointer] = game.colors;
        draw = true;
      } else if (y < -280) {
        guess[pointer] = (guess[pointer] % game.colors) + 1;
        draw = true;
      }

      if (draw) {
        canGuess = true;
        for (int i = 0; canGuess && i < game.pegs; i++) {
          canGuess = guess[i] != 0;
        }

        drawPegs(game.turn, guess);
        if (pointer < game.pegs)
          drawPeg(game.turn, pointer, guess[pointer], true);

        drawGuessText(canGuess, pointer == game.pegs);
        
        delay(150);
      }
      
      delay(50);
    }
    
    drawPegs(game.turn, guess);
    clearGuessText();
    int turn = game.turn;
    int *keys = game.guess(guess);
    drawKeyPegs(turn, keys);
  }
  
  if (game.won) {
    drawWin();
  } else {
    drawLose(game.code);
  }
  
  delay(1000);
  while (true) {
    readJoypad();
    if (button)
      break;
    delay(50);
  }
}

const int radius = 5;
const int xPadding = 6;
const int yPadding = 4;
const int diameter = radius * 2;

const int r2 = 3;
const int d2 = r2 * 2;

void drawPegs(int turn, int *pegs) {
  for (int i = 0; i < game.pegs; i++) {
    drawPeg(turn, i, pegs[i], false);
  }
}

void drawPeg(int turn, int i, int peg, boolean border) {
  Color c = peg > 0 ? colors[peg - 1] : black;
  if (border)
    screen.stroke(white.r, white.g, white.b);
  else if (peg == 0)
    screen.stroke(gray.r, gray.g, gray.b);
  else
    screen.stroke(c.r, c.g, c.b);
  screen.fill(c.r, c.g, c.b);
  drawPeg(turn, i, 6);
}

void drawKeyPegs(int turn, int *pegs) {
  int k = 0;
  for (int i = 0; i < game.pegs; i++) {
    if (pegs[i] == 0)
      continue;
    Color c = pegs[i] == 1 ? black : white;
    screen.stroke(255, 255, 255);
    screen.fill(c.r, c.g, c.b);
    drawKeyPeg(turn, k++);
  }
}

void drawGuessText(boolean enabled, boolean selected) {
  static Color c = { 0, 255, 0 };
  int y = getPegY(game.turn) - radius;
  if (selected) {
    screen.stroke(c.r, c.g, c.b);
    screen.fill(c.r, c.g, c.b);
    screen.rect(78, y, 33, diameter + 1);
    screen.stroke(black.r, black.g, black.b);
  } else if (enabled) {
    screen.stroke(c.r, c.g, c.b);
  } else {
    screen.stroke(gray.r, gray.g, gray.b);
  }
  screen.setTextSize(1);
  screen.text("GUESS", 80, y + 2);
}

void drawCenteredBox(int width, int height) {
  screen.stroke(white.r, white.g, white.b);
  screen.fill(black.r, black.g, black.b);
  int x = ((screenWidth + width) / 2) - width;
  int y = ((screenHeight + height) / 2) - height;
  screen.rect(x, y, width, height);
}

void drawCenteredText(char *text, int y) {
  int len = strlen(text);
  int width = len * 12;
  int x = ((screenWidth + width) / 2) - width;
  screen.text(text, x + 1, y);
}

void drawWin() {
  drawCenteredBox(60, 58);
  screen.setTextSize(2);
  drawCenteredText("YOU", 60);
  drawCenteredText("WIN!", 86);
}

void drawLose(int *code) {
  drawCenteredBox(86, 70);
  screen.setTextSize(2);
  drawCenteredText("YOU", 54);
  drawCenteredText("LOSE:(", 80);
  
  for (int i = 0; i < game.pegs; i++) {
    Color c = colors[code[i] - 1];
    screen.stroke(c.r, c.g, c.b);
    screen.fill(c.r, c.g, c.b);
    drawPeg(6, i, 34);
  }
}

void clearGuessText() {
  static Color c = black;
  int y = getPegY(game.turn) - radius;
  screen.stroke(c.r, c.g, c.b);
  screen.fill(c.r, c.g, c.b);
  screen.rect(78, y, 33, diameter + 1);
}

inline void clearScreen() {
  screen.background(0, 0, 0);
}

void startScreen() {
  clearScreen();
  
  screen.setTextSize(2);
  screen.stroke(0, 255, 0);
  screen.text("Mastermind", 6, 10);
  
  screen.setTextSize(1);
  screen.text("by Dave Dunkin", 6, 28);
}

void drawDemo() {
  clearScreen();
  
  int k = 0;
  for (int t = 0; t < game.turns; t++) {
    for (int i = 0; i < game.pegs; i++) {
      Color c = colors[(k++ % 6)];
      screen.stroke(c.r, c.g, c.b);
      screen.fill(c.r, c.g, c.b);
      drawPeg(t, i, 6);
    }
    
    for (int i = 0; i < game.pegs; i++) {
      Color c = i % 2 == 0 ? black : white;
      screen.stroke(255, 255, 255);
      screen.fill(c.r, c.g, c.b);
      drawKeyPeg(t, i);
    }
  }
}

inline int getPegY(int turn) {
  return turn * (diameter + yPadding + 2) + radius + 2;
}

inline void drawPeg(int turn, int i, int xOffset) {
  screen.circle(i * (diameter + xPadding) + radius + xOffset, getPegY(turn), radius);
}

inline void drawKeyPeg(int turn, int i) {
  screen.circle(i * (d2 + xPadding) + r2 + 76, getPegY(turn), r2);
}

void readJoypad() {
  x = Joypad.readJoystickX();
  y = Joypad.readJoystickY();
  button = Joypad.readJoystickSwitch() == 0;

  /*
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(button);
  */
}

