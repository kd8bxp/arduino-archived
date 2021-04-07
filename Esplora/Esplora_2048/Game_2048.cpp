#include "Game_2048.h"

#include <Arduino.h>

struct LineOfCells {
  bool cellMoved;
  unsigned int cellInLine[4];
};

unsigned int cells[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
bool availableCells[16];
int countAvailableCells;

long points = 0;

bool gameStarted = false;
bool gameOver = false;

void moveCellsL() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ 4 * row + 0 ];
    loc.cellInLine[1] = cells[ 4 * row + 1 ];
    loc.cellInLine[2] = cells[ 4 * row + 2 ];
    loc.cellInLine[3] = cells[ 4 * row + 3 ];
    loc = moveCells( loc );
    cells[ 4 * row + 0 ] = loc.cellInLine[0];
    cells[ 4 * row + 1 ] = loc.cellInLine[1];
    cells[ 4 * row + 2 ] = loc.cellInLine[2];
    cells[ 4 * row + 3 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved ) {
    addRandomTile();
  }
}

void moveCellsR() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ 4 * row + 3 ];
    loc.cellInLine[1] = cells[ 4 * row + 2 ];
    loc.cellInLine[2] = cells[ 4 * row + 1 ];
    loc.cellInLine[3] = cells[ 4 * row + 0 ];
    loc = moveCells( loc );
    cells[ 4 * row + 3 ] = loc.cellInLine[0];
    cells[ 4 * row + 2 ] = loc.cellInLine[1];
    cells[ 4 * row + 1 ] = loc.cellInLine[2];
    cells[ 4 * row + 0 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved ) {
    addRandomTile();
  }
}

void moveCellsU() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ row + 0 ];
    loc.cellInLine[1] = cells[ row + 4 ];
    loc.cellInLine[2] = cells[ row + 8 ];
    loc.cellInLine[3] = cells[ row + 12 ];
    loc = moveCells( loc );
    cells[ row + 0 ] = loc.cellInLine[0];
    cells[ row + 4 ] = loc.cellInLine[1];
    cells[ row + 8 ] = loc.cellInLine[2];
    cells[ row + 12 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved ) {
    addRandomTile();
  }
}

void moveCellsD() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ row + 12 ];
    loc.cellInLine[1] = cells[ row + 8 ];
    loc.cellInLine[2] = cells[ row + 4 ];
    loc.cellInLine[3] = cells[ row + 0 ];
    loc = moveCells( loc );
    cells[ row + 12 ] = loc.cellInLine[0];
    cells[ row + 8 ] = loc.cellInLine[1];
    cells[ row + 4 ] = loc.cellInLine[2];
    cells[ row + 0 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved ) {
    addRandomTile();
  }
}

LineOfCells moveCells( LineOfCells loc ) {
  for (unsigned int arrayPos = 0; arrayPos < 4; arrayPos++) {
    if ( loc.cellInLine[arrayPos] == 0 ) {
      for (unsigned int arrayPosNext = arrayPos; arrayPosNext < 4; arrayPosNext++) {
        if ( loc.cellInLine[arrayPosNext] != 0 ) {
          loc.cellMoved = true;
          loc.cellInLine[arrayPos] = loc.cellInLine[arrayPosNext];
          loc.cellInLine[arrayPosNext] = 0;
          break;
        }
      }
    }
    if ( loc.cellInLine[arrayPos] > 0 ) {
      for (unsigned int arrayPosNext = arrayPos+1; arrayPosNext < 4; arrayPosNext++) {
        if ( loc.cellInLine[arrayPos] == loc.cellInLine[arrayPosNext] ) {
          
          points += loc.cellInLine[arrayPos] + loc.cellInLine[arrayPosNext];
          
          loc.cellMoved = true;
          loc.cellInLine[arrayPos] = loc.cellInLine[arrayPos] + loc.cellInLine[arrayPosNext];
          loc.cellInLine[arrayPosNext] = 0;
          
          break;
        } else if ( loc.cellInLine[arrayPosNext] > 0 ) {
          break;
        }
      }
    }
  }
  
  return(loc);
}

bool furtherMovesArePossible() {
  int arrayPos;
  
  for (unsigned int row = 0; row < 4; row++) {
    for (unsigned int col = 0; col < 4; col++) {
      arrayPos = row * 4 + col;
      
      // return true if any cell is available to set a new tile
      if ( cells[arrayPos] == 0) {
        return true;
      }
      
      // return true if any right neighbor cell has the same value
      if ( ( col < 3 ) && ( cells[arrayPos] == cells[arrayPos + 1] ) ) {
        return true;
      }
      
      // return true if any lower neighbor cell has the same value
      if ( ( row < 3 ) && ( cells[arrayPos] == cells[arrayPos + 4] ) ) {
        return true;
      }
    }
  }
  return false;
}

// Adds a tile in a random position
void addRandomTile() {
  getAvailableCells();
  
  if ( countAvailableCells > 0 ) {
    unsigned int randomTile = random(10) < 9 ? 2 : 4;
    unsigned int randomPosition = random(countAvailableCells);
    
    insertRandomTileToAvailableCell( randomTile, randomPosition );
  
    getAvailableCells();
  } else {
    stopGame();
  }
}

void insertRandomTileToAvailableCell( unsigned int randomTile, unsigned int randomPosition ) {
  countAvailableCells = 0;
  
  for (unsigned int arrayPos = 0; arrayPos < 16; arrayPos++) {
    if ( availableCells[arrayPos] == true ) {
      if ( randomPosition == countAvailableCells) {
        cells[arrayPos] = randomTile;
        break;
      }
      
      countAvailableCells += 1;
    }
  }
}

void getAvailableCells() {
  int arrayPos;
  
  countAvailableCells = 0;
  
  for (unsigned int row = 0; row < 4; row++) {
    for (unsigned int col = 0; col < 4; col++) {
      arrayPos = row * 4 + col;
      if ( cells[arrayPos] == 0 ) {
        availableCells[arrayPos] = true;
        countAvailableCells += 1;
      } else {
        availableCells[arrayPos] = false;
      }
    }
  }
};

void startGame() {
  for (unsigned int arrayPos = 0; arrayPos < 16; arrayPos++) {
    cells[arrayPos] = 0;
    availableCells[arrayPos] = true;
  }
  
  addRandomTile();
  addRandomTile();
  
  gameStarted = true;
  gameOver = false;
  
  points = 0;
}

void stopGame() {
  gameStarted = false;
  gameOver = true;
}
