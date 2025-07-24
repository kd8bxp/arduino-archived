void linefollowing()
{
  int num = 0;

  //Detect the relative position of the Rollman-mini and black line in real time
  SignalValue = Sensor.Read_BlackFlag();

  //PD adjustment
  if (SignalValue == 5)
  {
    Motors.Motordrive(Speed_Dir, LEFT_BASE_SPEED, RIGHT_BASE_SPEED);
    digitalWrite(LEFT_LED, HIGH); digitalWrite(RIGHT_LED, HIGH);
    while (SignalValue == 5)
    {
      SignalValue = Sensor.Read_BlackFlag(); num++; delay(1);
    }
    if (num > 20) //*******
    {
      DirectionFlag--;
      if (Barrier_Flag == 1)//
        BarrierTime--;
      else
        BarrierTime++;//
      if (BarrierTime == 12)
      {
        DirectionFlagBackup = DirectionFlag;
        DirectionFlag = 0;//
      }
    }
    Serial.print(BarrierTime);
  }
  
  else
  {
    if ((SignalValue == 0) || (SignalValue == 6))
    {
      Motors.Motordrive(Speed_Dir, LEFT_BASE_SPEED, RIGHT_BASE_SPEED);
      digitalWrite(LEFT_LED, LOW); digitalWrite(RIGHT_LED, LOW);
    }
    else
    {
      if (SignalValue > 0)
      {
        digitalWrite(LEFT_LED, LOW);
        digitalWrite(RIGHT_LED, HIGH);
      }
      else
      {
        digitalWrite(RIGHT_LED, LOW);
        digitalWrite(LEFT_LED, HIGH);
      }
      MotorSpeed = int(P * SignalValue + D * (SignalValue - LastError));
      LastError = 0;
      LastError = SignalValue;
      Speed_L = LEFT_BASE_SPEED + MotorSpeed;
      Speed_R = RIGHT_BASE_SPEED - MotorSpeed;
      if (Speed_R > RIGHT_MAX_SPEED ) Speed_R = RIGHT_MAX_SPEED;
      if (Speed_L > LEFT_MAX_SPEED )  Speed_L = LEFT_MAX_SPEED;
      if (Speed_R < RIGHT_MINE_SPEED) Speed_R = RIGHT_MINE_SPEED;
      if (Speed_L < LEFT_MIN_SPEED)   Speed_L = LEFT_MIN_SPEED;
      Motors.Motordrive(Speed_Dir, Speed_L, Speed_R);
      delay(1);
      Motors.Motordrive(Speed_Dir, LEFT_BASE_SPEED - 15, RIGHT_BASE_SPEED - 15);
    }
    
  }
  
}

