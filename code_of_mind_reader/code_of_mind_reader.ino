/*    The Mind Reader - The Magic Cards with Arduino
      Copyright by LAGSILVA
      V2.1 - 17.Apr.2017
*/

#include <LedControl.h>   // Library for LED Array with MAX72XX
#include <MsTimer2.h>     // Library for Timer2

int k, b, chv, cartela, numcol, numlin, res, lin;
int botoes = 0;

byte simPinSW = 8; //Tactile button for the answer YES
byte naoPinSW = 7; //Tactile button for the answer NO
byte botaoSim, botaoNao;

/*
  Pin numbers of Arduino for connection with MAX72XX module
  pin 7 is connected to the DataIn (DIN)
  pin 6 is connected to LOAD (CS)
  pin 5 is connected to the CLK (CLK)
*/

LedControl lc = LedControl(2, 4, 3, 1);

//Array of digits numbers - 0 to 9 - Size 5x3
byte num[10][5] = {
  {7, 5, 5, 5, 7},  //0
  {2, 6, 2, 2, 2},  //1
  {7, 1, 7, 4, 7},  //2
  {7, 1, 7, 1, 7},  //3
  {5, 5, 7, 1, 1},  //4
  {7, 4, 7, 1, 7},  //5
  {7, 4, 7, 5, 7},  //6
  {7, 1, 2, 2, 2},  //7
  {7, 5, 7, 5, 7},  //8
  {7, 5, 7, 1, 7},  //9
};


void setup() {

  pinMode (simPinSW, INPUT_PULLUP);
  pinMode (naoPinSW, INPUT_PULLUP);

  MsTimer2::set(10, lerBotoes); // 10ms period
  MsTimer2::start();

  lc.shutdown(0, false);    // Wakeup call of MAX72XX
  lc.setIntensity(0, 10);    // Set the bright of display (max is 15)
  lc.clearDisplay(0);       // Clear the display

}


void loop() {
  
  randomSeed(analogRead(0));
  chv = random(1, 64);
  res = 0;
  for (cartela = 1; cartela <= 6; cartela++) { // Print the 06 questions
    lc.clearDisplay(0);
    delay(1000);
    for (lin = 0; lin < 5; lin++) { //Print the number of current question
      byte temp = num[cartela][lin];
      temp = temp << 2;
      lc.setRow(0, lin + 1, temp);
    }
    delay(1000);
    lc.clearDisplay(0);
    for (k = 1; k <= 64; k++) {
      b = k ^ chv;
      if (b >> (cartela - 1) & 1) {
        numcol = (k - 1) % 8;
        numlin = (k - 1) / 8;
        lc.setLed(0, numlin, numcol, true);
      }
    }
    while (botaoSim == 1 && botaoNao == 1) {
      delay(100);
      if (botaoSim == 0) {
        bitSet(res, cartela - 1); 
      }
    }
  }
  res = res ^ chv; 
  if (res == 0) {
    res = 64;
  }
  lc.clearDisplay(0);
  delay(1000);
  numcol = (res - 1) % 8;
  numlin = (res - 1) / 8;
  while (botaoSim == 1) { //Press YES to play again
    lc.setLed(0, numlin, numcol, true);
    delay(400); //Blinking LED for the result
    lc.setLed(0, numlin, numcol, false);
    delay(200);
  }
  
}


void lerBotoes() {  //Read YES or NO buttons

  botaoNao = digitalRead(naoPinSW);
  botaoSim = digitalRead(simPinSW);
  
}



