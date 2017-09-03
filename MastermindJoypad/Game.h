class Game {
public:
  static const int pegs = 4;
  static const int turns = 10;
  static const int colors = 6;
  
  static const int black = 1;
  static const int white = 2;
  
  int code[pegs];
  int turn;
  int guesses[turns][pegs];
  int keys[turns][pegs];
  bool won;

  void init() {
    for (int i = 0; i < pegs; i++) {
      code[i] = random(colors) + 1;
    }
    for (int i = 0; i < turns; i++) {
      for (int j = 0; j < pegs; j++) {
        guesses[i][j] = 0;
        keys[i][j] = 0;
      }
    }
    turn = 0;
    won = false;
  }
  
  int *guess(int *guess) {
    int k = 0;
    won = true;
    bool matchedCode[pegs];
    bool matchedGuess[pegs];
    for (int i = 0; i < pegs; i++) {
      matchedCode[i] = false;
      matchedGuess[i] = false;
      guesses[turn][i] = guess[i];
    }
    for (int i = 0; i < pegs; i++) {
      if (guess[i] == code[i]) {
        keys[turn][k++] = black;
        matchedCode[i] = true;
        matchedGuess[i] = true;
      } else {
        won = false;
      }
    }
    for (int i = 0; i < pegs; i++) {
      if (matchedCode[i])
        continue;
      for (int j = 0; j < pegs; j++) {
        if (matchedGuess[j])
          continue;
        if (guess[j] == code[i]) {
          keys[turn][k++] = white;
          matchedCode[i] = true;
          matchedGuess[j] = true;
          break;
        }
      }
    }
    turn++;
    return keys[turn - 1];
  }
};

