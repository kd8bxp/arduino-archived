
#include "RoverC.h"

//Slightly Modified to use Wire1 by LeRoy Miller, KD8BXP June 20, 2022

void RoverC_Init(void)    //sda  0     scl  26
{
    Wire1.begin(0,26,100);
}

void Send_iic(uint8_t Register, uint8_t Speed)
{
  Wire1.beginTransmission(ROVER_ADDRESS);
  Wire1.write(Register);
  Wire1.write(Speed);
  Wire1.endTransmission();
}

void Move_forward(int8_t Speed)
{
  Send_iic(0x00, Speed );
  Send_iic(0x01, Speed );
  Send_iic(0x02, Speed );
  Send_iic(0x03, Speed );
}

void Move_back(int8_t Speed)
{
  Send_iic(0x00, (-1) * Speed );
  Send_iic(0x01, (-1) * Speed );
  Send_iic(0x02, (-1) * Speed );
  Send_iic(0x03, (-1) * Speed );
}

void Move_turnleft(int8_t Speed)
{
  Send_iic(0x00, Speed );
  Send_iic(0x01, (-1) * Speed );
  Send_iic(0x02, Speed );
  Send_iic(0x03, (-1) * Speed );
}

void Move_turnright(int8_t Speed)
{
  Send_iic(0x00, (-1) * Speed );
  Send_iic(0x01, Speed );
  Send_iic(0x02, (-1) * Speed );
  Send_iic(0x03, Speed );
}

void Move_left(int8_t Speed)
{
  Send_iic(0x00, (-1) * Speed );
  Send_iic(0x01, Speed );
  Send_iic(0x02, Speed );
  Send_iic(0x03, (-1) * Speed );
}

void Move_right(int8_t Speed)
{
  Send_iic(0x00, Speed );
  Send_iic(0x01, (-1) * Speed );
  Send_iic(0x02, (-1) * Speed );
  Send_iic(0x03, Speed );
}
void Move_stop(int8_t Speed)
{
  Send_iic(0x00, 0 );
  Send_iic(0x01, 0 );
  Send_iic(0x02, 0 );
  Send_iic(0x03, 0 );
}

void Servo_angle(uint8_t Servo_ch, uint8_t degree)
{
  degree = min(90, int(degree));
  degree = max(0, int(degree));
  Send_iic((Servo_ch - 1) | 0x10 ,int(degree));
}

void Servo_pulse(uint8_t Servo_ch, uint16_t width)
{
  width = min(2500, int(width));
  width = max(500, int(width));
  Send_iic((Servo_ch - 1) | 0x20, width);
}
