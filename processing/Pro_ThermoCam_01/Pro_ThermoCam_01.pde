/*Pro_ThermoCam_01.pde   Agrucho 04JAN 2017
Presentation of an image (40x30) pixels.
Data send by the Arduino at 115200 bauds.
Arduino Sketch: One_Pixel_ThermoCam_02.ino

*/
import processing.serial.*;
 
Serial myPort; 
 
int x;
int y;
int value;
int reading;
int Rcolor, Gcolor, Bcolor;
int[][] pixel = new int[40][30];   //Two-dimensional Array to receive data.

void setup(){
  println(Serial.list());               // Check COM port number to edit Serial.list[]
  String portName = Serial.list()[4];
  myPort = new Serial(this, "/dev/ttyUSB0", 115200);
  myPort.bufferUntil(10); 
  size(640,480);
  background(0);
  noStroke();
  frameRate(20);     // set draw() screen refresh rate
}

/*------------------------------------------------------------------------------------
 draw()
-------------------------------------------------------------------------------------*/ 
void draw(){
  
  for (int j = 0; j < 30; j++) {
    for (int i = 0; i < 40; i++) {
     // fill(color(pixel[i][j],0,0));
     temp2RGB(pixel[i][j]);
     fill(color(Rcolor,Gcolor,Bcolor));
      rect(i*16,480- (j+1)*16, 16, 16);
    }
  }
  
}

/*------------------------------------------------------------------------------------
 serialEvent ()
-------------------------------------------------------------------------------------*/ 
void serialEvent(Serial myPort){
  String input = myPort.readString(); 
  input = trim(input);
  String[] vars = split(input, ",");
  x = int(vars[0]);
  y = int(vars[1]);
  value= int(vars[2]);
  pixel[x][y]= constrain(value,0,255);
  println("x: " + x + "\ty: " + y + "\tr: " + value);
} 


/*------------------------------------------------------------------------------------
 temp2RGB()
 Conver a number from 0 to 255 to a RGB color to represent temperature.
  BLUE = cold, RED = hot.
-------------------------------------------------------------------------------------*/ 
void temp2RGB(int tmpValue){
 float factor;
 int colorValue;

  colorValue=(int)map(tmpValue,0,255,0,1020);


  if(colorValue <= 510){
    Rcolor = 0;                       // No red in low temperatures
    if(colorValue <= 255){            // 0<=color<=255 
      Gcolor = 0 + colorValue;
      Bcolor = 255;
    }
    else{                             // 255<color<=510
      colorValue = colorValue - 255;
      Bcolor = 255 - colorValue;
      Gcolor = 255;
    }
  }

  else{                               // 510<color<=1020
    colorValue = colorValue - 510;
    Bcolor = 0;                       // No blue in high temperatures
    if(colorValue <= 255){            // 510>color<=765
      Rcolor = 0 + colorValue;
      Gcolor = 255;
    }
    else{      
      colorValue = colorValue - 255;  // 765<color<=1020
      Gcolor = 255 - colorValue;
      Rcolor = 255;
    }
  }
}