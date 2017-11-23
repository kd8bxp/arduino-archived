#include <WEMOS_Matrix_GFX.h>

MLED matrix(5); //set intensity=5

int row = 1;
int col = 1;

void setup() {
  // put your setup code here, to run once:
matrix.clear();
matrix.drawPixel(row,col,1); // draw dot
 matrix.writeDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:

}
