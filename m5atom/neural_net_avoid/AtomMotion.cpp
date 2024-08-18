#include "AtomMotion.h"

//attempt to modified to work with the m5stack ultrasonic sensors on Wire

void AtomMotion::Init()		
{
	Wire1.begin(25, 21);
}

void AtomMotion::Write1Byte(uint8_t address,uint8_t Register_address,uint8_t data)
{
  Wire1.beginTransmission(address);
  Wire1.write(Register_address);
  Wire1.write(data);
  Wire1.endTransmission();
}

void AtomMotion::Write2Byte(uint8_t address,uint8_t Register_address,uint16_t data)
{
  Wire1.beginTransmission(address);
  Wire1.write(Register_address);
  Wire1.write(data >> 8); //MSB
  Wire1.write(data & 0xFF); //LSB
  
  Wire1.endTransmission();
}


uint8_t AtomMotion::ReadBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest) {

  Wire1.beginTransmission(address);   // Initialize the Tx buffer
  Wire1.write(subAddress);            // Put slave register address in Tx buffer
  uint8_t i = 0;
  if (Wire1.endTransmission(false) == 0 && Wire1.requestFrom(address, (uint8_t)count)) {
    while (Wire1.available()) {
		dest[i++]=Wire1.read();
    }
    return true;
  }
  return false;
}



/*******************************************************************************/


uint8_t AtomMotion::SetServoAngle(uint8_t Servo_CH,uint8_t angle)
{
	uint8_t Register_address=Servo_CH-1;
	if(Register_address>3)
		return 0;
	Write1Byte(SERVO_ADDRESS,Register_address,angle);
	return 0;
}

uint8_t AtomMotion::SetServoPulse(uint8_t Servo_CH,uint16_t width)    //0x10        ->16
{
	uint8_t servo_ch =	Servo_CH-1;
	uint8_t Register_address=2*servo_ch+16;
	if(Register_address%2==1 || Register_address>32)
		return 1;
	Write2Byte(SERVO_ADDRESS,Register_address,width);
	return 0;
}

uint8_t AtomMotion::SetMotorSpeed(uint8_t Motor_CH, int8_t speed)    //0x10        ->16
{
	uint8_t servo_ch =	Motor_CH-1;
	if(servo_ch>1)
		return 1;
	uint8_t Register_address=servo_ch+32;
	Write1Byte(SERVO_ADDRESS, Register_address, (uint8_t)speed);
	return 0;
}


uint8_t AtomMotion::ReadServoAngle(uint8_t Servo_CH)
{
	uint8_t data=0;
	uint8_t Register_address=Servo_CH-1;
	ReadBytes(SERVO_ADDRESS,Register_address,1,&data);
	return data;
}

uint16_t AtomMotion::ReadServoPulse(uint8_t Servo_CH)
{
	uint8_t data[2];
	uint8_t servo_ch =	Servo_CH-1;
	uint8_t Register_address=2*servo_ch | 0x10;
	ReadBytes(SERVO_ADDRESS,Register_address,2,data);
  return  (data[0]<<8)+data[1];
}

int8_t AtomMotion::ReadMotorSpeed(uint8_t Motor_CH)
{
	int8_t data=0;
	uint8_t servo_ch =	Motor_CH-1;
	if(servo_ch>1)
		return 1;
	uint8_t Register_address=servo_ch | 0x20;
	ReadBytes(SERVO_ADDRESS,Register_address,1,(uint8_t *)&data);
	return data;
}
