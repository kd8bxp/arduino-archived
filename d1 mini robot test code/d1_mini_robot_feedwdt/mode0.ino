void mode0() {
  //Mode 0 will be WIFI control.
ESP.wdtFeed();
   switch (cmd) {
    case 0: 
      //Serial.println("STOP");
      stop();
      break;
     case 1:
     //Serial.println("UP"); //forward
     forward(setSpeed);
     break;
     case 2:
          break;
     case 3:
     //Serial.println("LEFT");
     left(setSpeed);
     break;
     case 4:
     //Serial.println("DOWN"); //backward
     backward(setSpeed);
     break;
     case 5: 
     //Serial.println("RIGHT");
     right(setSpeed);
     break;
     default:
     stop();
     break;
  }
 }

/*
 * 0 - STOP
 * 1 - UP
 * 2 - CENTER
 * 3 - LEFT
 * 4 - DOWN
 * 5 - RIGHT
 */

