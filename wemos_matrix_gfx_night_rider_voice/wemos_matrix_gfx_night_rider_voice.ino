#include <WEMOS_Matrix_GFX.h>

MLED matrix(5); //set intensity=5

int row = 1;
int col = 1;

void setup() {
  // put your setup code here, to run once:
matrix.clear();
matrix.drawLine(3,1,3,6,1); // draw dot
matrix.drawLine(4,1,4,6,1);
matrix.drawLine(2,2,2,5,1);
matrix.drawLine(5,2,5,5,1);
 matrix.writeDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:

}
