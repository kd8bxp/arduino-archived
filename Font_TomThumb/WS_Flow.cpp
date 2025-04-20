#include "WS_Flow.h"
#include "src/Adafruit_GFX/Fonts/TomThumb.h"

Adafruit_NeoMatrix Matrix = Adafruit_NeoMatrix(5, 8, RGB_Control_PIN, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +                     
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,                
  NEO_GRB            + NEO_KHZ800);                           

int MatrixWidth = 0;
const uint16_t colors[] = {
  Matrix.Color(255, 0, 0), Matrix.Color(0, 255, 0), Matrix.Color(0, 0, 255) };

int getCharWidth(char c) {
  if (c == 'i' || c == 'l' || c == '!' || c == '.') {
    return 3;
  // } else if (c == 'm' || c == 'w') {
  //   return 7;
  } else {
    return 5;
  }
}
int getStringWidth(const char* str) {
  int width = 0;
  int length = strlen(str);
  printf("%d\r\n",length);
  for (int i = 0; i < length; i++) {
    width += getCharWidth(str[i]);
    width += 1;      
  }
  printf("%d\r\n",width);
  return width;
}
void Matrix_Init() {
  Matrix.begin();
  Matrix.setTextWrap(false);  
  
  // English: Please note that the brightness of the lamp bead should not be too high, which can easily cause the temperature of the board to rise rapidly, thus damaging the board !!!
  // Chinese: 请注意，灯珠亮度不要太高，容易导致板子温度急速上升，从而损坏板子!!! 
  Matrix.setBrightness(20);           // set brightness
  Matrix.setTextColor(colors[0]); 
  MatrixWidth   = Matrix.width();
  Matrix.setFont(&TomThumb); 
}
void Text_Flow(char* Text) {
  int textWidth   = getStringWidth(Text);
  Matrix.fillScreen(0);
  Matrix.setCursor(MatrixWidth, 7);
  Matrix.print(F(Text));
  if (--MatrixWidth < -textWidth) {
    MatrixWidth = Matrix.width();
  }
  Matrix.show();
}
