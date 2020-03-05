//
//  Kyle Samson
//  C++ Programming II
//  Super Star Trek Main
//
// https://github.com/stellarestuary/SuperStarTrek-CPP

#include "starTrek.hpp"

void setup()
{
  Serial.begin(115200);
    StarTrek* ptrStarTrekGame = new StarTrek();
    ptrStarTrekGame->play();
    delete ptrStarTrekGame;

    //return 0;
}

void loop() {
  
}
