void drive()  //  start driving
{
  
  //set_leds();
  switch(out)
  {
    case 0: cw(150, 0); M5.dis.drawpix(0, 0x00FF00); /*cw(150, 50);*/ delay(10); break; //left - go to right backward //cw(50, 150); delay(350);  break; //left wide backward
    case 1: cw(75,50); M5.dis.drawpix(0, 0x00FF00); /*cw(255, 0);*/ delay(10); break; //left - go a little to right backward//cw(0,255); delay(50);   break;  //left soft
    case 2: ccw(100+cal, 100);   break; //forward
    case 3: cw(0,150); M5.dis.drawpix(0, 0x00FFFF); /*cw(50,150);*/ delay(10); break; //right go to left backward //cw(150,50); delay(350); break; //right wide backward
    case 4: cw(50,75); M5.dis.drawpix(0, 0x00FFFF); /*cw(0, 255);*/ delay(10); break; //right goto a little left backward //cw(255,0); delay(50); break; //right soft
    //case 5: digitalWrite(motA_1, LOW); digitalWrite(motA_2, HIGH);  digitalWrite(motB_1, LOW); digitalWrite(motB_2, HIGH);ledcWrite(Channel_0, 170); ledcWrite(Channel_1, 170);   break;   
  }
  M5.dis.drawpix(0, 0x000000); delay(1);
}

void cw(int lft, int rgt) { 
  //This causes the car to move backward
  
  //ledcWrite(Channel_0, map(lft, 50, 255, 50, 255));
  //ledcWrite(Channel_1, map(rgt, 50, 255, 50, 255));
  Atom.SetMotorSpeed(1,rgt); //100);
  Atom.SetMotorSpeed(2,lft); //100);
 
}

void ccw(int lft, int rgt) {
  //This causes the car to move forward
 
  Atom.SetMotorSpeed(1,rgt); //100);
  Atom.SetMotorSpeed(2,lft); //100);
  M5.dis.drawpix(0, 0xff0000); //green
  
}

void stop() {
  Atom.SetMotorSpeed(1, 0);
  Atom.SetMotorSpeed(2, 0);
  M5.dis.drawpix(0, 0x00FF00); //red
  
}
