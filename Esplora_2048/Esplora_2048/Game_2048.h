extern struct LineOfCells LOC;

extern struct CellFieldMatrix CellFieldMatrix; 

extern unsigned int cells[16];
extern bool availableCells[16];
extern int countAvailableCells;

extern long points;

extern bool gameStarted;
extern bool gameOver;

void moveCellsU();
void moveCellsD();
void moveCellsL();
void moveCellsR();

LineOfCells moveCells( LineOfCells loc );

bool furtherMovesArePossible();

void addRandomTile();

void insertRandomTileToAvailableCell( unsigned int randomTile, unsigned int randomPosition );

void getAvailableCells();

void startGame();
void stopGame();
