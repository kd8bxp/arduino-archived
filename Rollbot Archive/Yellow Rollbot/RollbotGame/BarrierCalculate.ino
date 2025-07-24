void BarrierCalculate()
{
  if (BarrierTime == 12)
  {
    delay(800);
    DirectionFlag = DirectionFlagBackup;
    if (DirectionFlag == 0)
    {
      Motors.Motorstop();
      digitalWrite(LEFT_LED, HIGH); digitalWrite(RIGHT_LED, HIGH);
      Music.MiniBuzzer(tune, duration, Length);
      while (1);
    }
    else
    {
      OLED.SelectFace(3, SmilingFace, LongFace, NormalFace);
      turn_back();
      Barrier_Flag = 1;
      while (DirectionFlag)
      {
        linefollowing();
      }
      for (int i = 50; i > 0; i--)
      {
        linefollowing();
      }
      Barrier_Flag = 0;
      Motors.Motorstop();
      delay(800);
      turn_back();
    }
  }
  switch (BarrierTime)
  {
    case 2:
      {
        delay(800);
        DirectionFlag = 2;
        while (DirectionFlag)
        {
          linefollowing();
        }
        for (int i = 50; i > 0; i--)
        {
          linefollowing();
        }
        Motors.Motorstop();
        delay(800);
      } break;
    case 5:
      {
        delay(800);
        OLED.SelectFace(3, SmilingFace, LongFace, NormalFace);
        turn_back();
        Barrier_Flag = 1;
        DirectionFlag = 4;
        while (DirectionFlag)
        {
          linefollowing();
        }
        for (int i = 50; i > 0; i--)
        {
          linefollowing();
        }
        Barrier_Flag = 0;
        while (!(SignalValue == 6))
        {
          linefollowing();
        }
        BarrierTime = 0;
        Motors.Motorstop();
        delay(800);
        turn_back();
      } break;
    case 9:
      {
        delay(800);
        OLED.SelectFace(3, SmilingFace, LongFace, NormalFace);
        turn_back();
        Barrier_Flag = 1;
        DirectionFlag = 3;
        while (DirectionFlag)
        {
          linefollowing();
        }
        for (int i = 50; i > 0; i--)
        {
          linefollowing();
        }
        Motors.Motorstop();
        Barrier_Flag = 0;
        BarrierTime = 6;
        delay(800);
        turn_back();
      }
    default:
      break;
  }
}

