//compiles using board core 1.0.9

#include <M5EPD.h>

M5EPD_Canvas canvas(&M5.EPD);

int maxPicSize = 3; //Number of images in the SD card
int pic = 0;        //Counter of images
bool draw = false;

void setup()
{
    M5.begin();
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);

    canvas.createCanvas(540, 960);
    canvas.setTextSize(3);
    canvas.drawPngFile(SD, "/pictures/p0.png");  //Initial display image
    canvas.pushCanvas(0,0,UPDATE_MODE_GC16);
}

void loop()
{
  if(M5.BtnL.wasPressed()){
    pic = pic + 1;
    draw = true;
  }
  if(M5.BtnR.wasPressed()){
    pic = pic - 1;
    if(pic < 0){
      pic = maxPicSize - 1; 
    }
    draw = true;
  }
  if(M5.BtnP.wasPressed()) {
    M5.update();
    canvas.drawPngFile(SD, "/pictures/p1.png");  //InSpecial image
    canvas.pushCanvas(0,0,UPDATE_MODE_GC16);
  }
  M5.update();
  if(draw){
    int num = pic % maxPicSize;
    String s = "/pictures/p"+String(num)+".png";
    canvas.drawPngFile(SD, s.c_str());
    canvas.pushCanvas(0,0,UPDATE_MODE_GC16);
  }
  draw = false;

}
