void drive()  //  start driving
{
  int cal = 5;
  set_leds();
  switch(out)
  {
    case 0: turnLeft(255, 0); /*cw(150, 50);*/ delay(350); break; //left - go to right backward //cw(50, 150); delay(350);  break; //left wide backward
    case 1: turnLeft(150,50); /*cw(255, 0);*/ delay(50); break; //left - go a little to right backward//cw(0,255); delay(50);   break;  //left soft
    case 2: forward(150);   break; //forward
    case 3: turnRight(0,255); /*cw(50,150);*/ delay(350); break; //right got to left backward //cw(150,50); delay(350); break; //right wide backward
    case 4: turnRight(50,150); /*cw(0, 255);*/ delay(50); break; //right goto a little left backward //cw(255,0); delay(50); break; //right soft
    //case 5: digitalWrite(motA_1, LOW); digitalWrite(motA_2, HIGH);  digitalWrite(motB_1, LOW); digitalWrite(motB_2, HIGH);ledcWrite(Channel_0, 170); ledcWrite(Channel_1, 170);   break;   
  }
}

void forward(int moveSpeed)
{
  MotorL.run(-moveSpeed);
  MotorR.run(moveSpeed);
}

void backward(int moveSpeed)
{
  MotorL.run(moveSpeed); 
  MotorR.run(-moveSpeed);
}

void turnLeft(int moveSpeedL, int moveSpeedR)
{
  MotorL.run(-moveSpeedL); //(-moveSpeed/10);
  MotorR.run(moveSpeedR);
}

void turnRight(int moveSpeedL, int moveSpeedR)
{
  MotorL.run(-moveSpeedL);
  MotorR.run(moveSpeedR); //(moveSpeed/10);
}

void stop()
{
//  rgb.clear();
  MotorL.run(0);
  MotorR.run(0);
}
