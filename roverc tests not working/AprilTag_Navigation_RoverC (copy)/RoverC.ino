// Move TO marker

#include "src/M5StickC/M5StickC.h"
#include "RoverC.h"

/*

  Move_forward(100);
  delay(2000);
  Move_back(100);
  delay(2000);
  Move_turnleft(100);
  delay(2000);
  Move_turnright(100);
  delay(2000);


variables used:
numberTags
startTag
endTag
topTag
bottomTag
rightTag
leftTag

startMoving
setGridTarget
setGridFlag
currentTag
oldTag
roverCFacing; //0 = top, 1 = right, 2 = south, 3= west
gridX
gridY

*/


//read json string from camera, attempt to figure out where rover is and direction it's facing, attempt to Move to Target location (setGridTarget)

void moveToTarget() {
//reads serial string
while (setGridFlag == true) { 

readSerialEvent();
  
  if (stringComplete) {
    Serial.println(inputString);
    decodeJson();
    inputString = "";
    stringComplete = false;
    
    if (startMoving == false) {WhereAmI();} else {toTarget();} //this seems reduntant to me.....
  }
 }
}

void WhereAmI() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0,0);
  
// Not as reliable as I had hopeful for, but did seem to work about 85 to 90% of the time.
if (tagInfo.rotation == 1) {roverCFacing = 0; M5.Lcd.print("Top"); startMoving = true; } //this seems like it should set the setGridFlag not the startMoving flag.
if (tagInfo.rotation == 3) {roverCFacing = 1; M5.Lcd.print("Right"); startMoving = true; }
if (tagInfo.rotation == 4) {roverCFacing = 2; M5.Lcd.print("Bottom"); startMoving = true; }
if (tagInfo.rotation == 0) {roverCFacing = 3; M5.Lcd.print("Left"); startMoving = true; }

if (tagInfo.id != topTag && roverCFacing == 0) {startingTag = tagInfo.id + 7;} else { /* we have to move robot to figure out more */ startMoving = false; }
if (tagInfo.id != rightTag && roverCFacing == 1) {startingTag = tagInfo.id + 1;} else { /* we have to move robot to figure out more */ startMoving = false; }
if (tagInfo.id != bottomTag && roverCFacing == 2) {startingTag = tagInfo.id - 7;} else { /* we have to move robot to figure out more */ startMoving = false; }
if (tagInfo.id != leftTag && roverCFacing == 3) {startingTag = tagInfo.id - 1;} else { /* we have to move robot to figure out more */ startMoving = false; }

}

void resetFlags() {
setGridFlag = false;
startMoving = false;
oldTag = -100;
}

void toTarget() {
//test stuff - reset esp32 for testing reasons.
/* delay(10 * 1000); //10 second delay
 ESP.restart();
 while(1);
 */ 

//maintain center....follow tag
readSerialEvent();
maintainCenter(); 
}


void maintainCenter() {

//Based on code from GEH00073 https://qiita.com/airpocket/items/0cfb9e3e1a202c2f00df Jan 21-22, 2020
//calculate center
dx = int(160-(tagInfo.cx + (tagInfo.cy / 2)));

  if( dx > -threshold && dx < threshold)
            {
                state = straight;  // Go straight
               
            }

            else if(dx <= -threshold)
            {
                state = right;  // Go Right
                
            }

            else if(dx >= threshold)
            {
                state = left; // Go Left
                
            }

            else
            {
                
                state = noTarget;  // Rotate
            }

            if (dx > 0)                 //回転方向を最後にターゲットを見つけた方向に変更する。
            {
                rotate = 1; 
            }
            else
            {
                rotate = -1;
            }
            dx_old = dx;
 
 switch(state)
    {
        case noTarget:
            
            Setspeed(0, 0, 40 * rotate * vpara);   //vpara batch change speed with
            delay(movetime);                       //movetime(msec)回転を継続
            Setspeed(0, 0, 0);
            delay(waitcam);                        //waitcam(msec)カメラの安定待ち時間
        break;

        case left:
            
            Setspeed(40 * vpara, 0, 0);            //vparaで速度一括変更
            delay(20);
            Move_stop(0);
            Setspeed(0, 0, 0);
        break;

        case right:
            
            Setspeed(-40 * vpara, 0, 0);           //vparaで速度一括変更
            delay(20);
            Setspeed(0, 0, 0);
        break;

        case straight:
            Setspeed(0, 40 * vpara, 0);            //vparaで速度一括変更
            delay(20);
            Setspeed(0, 0, 0);
        break;

        case stop:
            Setspeed(0, 0, 0);
        break;

    }           
}
