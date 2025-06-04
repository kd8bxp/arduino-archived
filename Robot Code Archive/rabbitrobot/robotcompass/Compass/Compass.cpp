#include "Compass.h"
#include <Wire.h>
#include "ArduinoRobot.h"

#ifdef UseHMC5883L

void getRawData(int* x ,int* y,int* z);
void calibrateMag();//校准罗盘
int offsetX,offsetY,offsetZ;

Compass::Compass()
{
  m_Scale = 1;
  ComCalibraFlag=0;//初始化为没有校准的状�?
}

void Compass::begin()
{
	Wire.begin();
}

MagnetometerRaw Compass::ReadRawAxis()
{
  uint8_t* buffer = Read(DataRegisterBegin, 6);
  MagnetometerRaw raw = MagnetometerRaw();
  raw.XAxis = (buffer[0] << 8) | buffer[1];
  raw.ZAxis = (buffer[2] << 8) | buffer[3];
  raw.YAxis = (buffer[4] << 8) | buffer[5];
  return raw;
}

MagnetometerScaled Compass::ReadScaledAxis()
{
  MagnetometerRaw raw = ReadRawAxis();
  MagnetometerScaled scaled = MagnetometerScaled();
  scaled.XAxis = raw.XAxis * m_Scale;
  scaled.ZAxis = raw.ZAxis * m_Scale;
  scaled.YAxis = raw.YAxis * m_Scale;
  return scaled;
}

int Compass::SetScale(float gauss)
{
	uint8_t regValue = 0x00;
	if(gauss == 0.88)
	{
		regValue = 0x00;
		m_Scale = 0.73;
	}
	else if(gauss == 1.3)
	{
		regValue = 0x01;
		m_Scale = 0.92;
	}
	else if(gauss == 1.9)
	{
		regValue = 0x02;
		m_Scale = 1.22;
	}
	else if(gauss == 2.5)
	{
		regValue = 0x03;
		m_Scale = 1.52;
	}
	else if(gauss == 4.0)
	{
		regValue = 0x04;
		m_Scale = 2.27;
	}
	else if(gauss == 4.7)
	{
		regValue = 0x05;
		m_Scale = 2.56;
	}
	else if(gauss == 5.6)
	{
		regValue = 0x06;
		m_Scale = 3.03;
	}
	else if(gauss == 8.1)
	{
		regValue = 0x07;
		m_Scale = 4.35;
	}
	else
		return ErrorCode_1_Num;

	// Setting is in the top 3 bits of the register.
	regValue = regValue << 5;
	Write(ConfigurationRegisterB, regValue);
}

int Compass::SetMeasurementMode(uint8_t mode)
{
	Write(ModeRegister, mode);
}

uint8_t Compass::EnsureConnected()
{
	uint8_t data = Read(IdentityRegister, 1)[0];

	if(data == IdentityRegisterValue)
		IsConnected = 1;
	else
		IsConnected = 0;

	return IsConnected;
}

float Compass::getReading()//得到Heading数据
{

	/*
	SetScale(1.3);//set the scale of the compass
	SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
	// Retrive the raw values from the compass (not scaled).
	MagnetometerRaw raw = ReadRawAxis();
	// Retrived the scaled values from the compass (scaled to the configured scale).
	MagnetometerScaled scaled = ReadScaledAxis();
	// Calculate heading when the magnetometer is level, then correct for signs of axis.
	float heading = atan2(scaled.YAxis, scaled.XAxis);
	 // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
	  // Find yours here: http://www.magnetic-declination.com/
	  // Mine is: 2�?37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
	  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
	  float declinationAngle = 0.0457;
	  heading += declinationAngle;

	  // Correct for when signs are reversed.
	  if(heading < 0)
	    heading += 2*PI;

	  // Check for wrap due to addition of declination.
	  if(heading > 2*PI)
	    heading -= 2*PI;

	  // Convert radians to degrees for readability.
	  float headingDegrees = heading * 180/M_PI;
      */
	  if(ComCalibraFlag==0)//
	  {
		  //Put the HMC5883 IC into the correct operating mode
		  Wire.beginTransmission(HMC5883L_Address); //open communication with HMC5883r
		  Wire.write(0x00); //select configuration register A
		  Wire.write(0x70); //0111 0000b configuration
		  Wire.endTransmission();
		  SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
		// Retrive the raw values from the compass (not scaled).

		  calibrateMag();//进入电子罗盘校准程序
		  ComCalibraFlag=1;
	  }

	  MagnetometerRaw raw = ReadRawAxis();
	  float headingRadians = atan2((double)((raw.YAxis)-offsetY),(double)((raw.XAxis)-offsetX));
		// Correct for when signs are reversed.
	  if(headingRadians < 0)
		  headingRadians += 2*PI;

	  int headingDegrees = headingRadians * 180/M_PI;
		headingDegrees += MagnetcDeclination; //the magnetc-declination angle

		// Check for wrap due to addition of declination.
	   if(headingDegrees > 360)
		  headingDegrees -= 360;

	   return headingDegrees;


}

void Compass::Write(int address, int data)
{
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t* Compass::Read(int address, int length)
{
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.endTransmission();

  Wire.beginTransmission(HMC5883L_Address);
  Wire.requestFrom(HMC5883L_Address, length);

  uint8_t buffer[length];
  if(Wire.available() == length)
  {
	  for(uint8_t i = 0; i < length; i++)
	  {
		  buffer[i] = Wire.read();
	  }
  }
  Wire.endTransmission();

  return buffer;
}

char* Compass::GetErrorText(int errorCode)
{
	if(ErrorCode_1_Num == 1)
		return ErrorCode_1;

	return "Error not defined.";
}

void getRawData(int* x ,int* y,int* z)
{
  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC5883L_Address, 6);
  if(6<=Wire.available()){
    *x = Wire.read()<<8; //X msb
    *x |= Wire.read(); //X lsb
    *z = Wire.read()<<8; //Z msb
    *z |= Wire.read(); //Z lsb
    *y = Wire.read()<<8; //Y msb
    *y |= Wire.read(); //Y lsb
  }
}

void calibrateMag()
{
  int x,y,z; //triple axis data
  int xMax, xMin, yMax, yMin, zMax, zMin;
  //initialize the variables
   getRawData(&x,&y,&z);
  xMax=xMin=x;
  yMax=yMin=y;
  zMax=zMin=z;

  for(int i=0;i<10;i++)
  {
		if(i<5)
		{
			Robot.motorsWrite(-200, 200);  // turn left
		}
		else if(i==5)
		{
		   Robot.motorsWrite(0, 0);
		}
		else
		{
		  Robot.motorsWrite(200, -200);  // turn right
		}
		getRawData(&x,&y,&z);
		//get Max and Min
		// this routine will capture the max and min values of the mag X, Y, and Z data while the
		// compass is being rotated 360 degrees through the level plane and the upright plane.
		// i.e. horizontal and vertical circles.
		// This function should be invoked while making continuous measurements on the magnetometers
		if (x > xMax)
		  xMax = x;
		if (x < xMin )
		  xMin = x;
		if(y > yMax )
		  yMax = y;
		if(y < yMin )
		  yMin = y;
		if(z > zMax )
		  zMax = z;
		if(z < zMin )
		  zMin = z;
		delay(500);
  }
   Robot.motorsWrite(0, 0);
  //compute offsets
  if(abs(xMax - xMin) > CalThreshold )
  {
	  offsetX = (xMax + xMin)/2;
  }
  if(abs(yMax - yMin) > CalThreshold )
  {
	  offsetY = (yMax + yMin)/2;
  }
  if(abs(zMax - zMin) > CalThreshold )
  {
	 offsetZ = (zMax +zMin)/2;
  }
 /*
  Serial.print("offsetX:");
  Serial.print("");
  Serial.print(offsetX);
  Serial.print(" offsetY:");
  Serial.print("");
  Serial.print(offsetY);
  Serial.print(" offsetZ:");
  Serial.print("");
  Serial.println(offsetZ);
  */

  //delay(100);
}



/*
void Compass::_beginTransmission()
{
  ;
}
void Compass::_endTransmission(){
  Wire.endTransmission();
}
*/
#endif

#ifdef UseHMC6352
void Compass::begin(){
	Wire.begin();
}
float Compass::getReading(){
	_beginTransmission();
	_endTransmission();
	
  //time delays required by HMC6352 upon receipt of the command
  //Get Data. Compensate and Calculate New Heading : 6ms
  delay(6);

  Wire.requestFrom(HMC6352SlaveAddress, 2); //get the two data bytes, MSB and LSB

  //"The heading output data will be the value in tenths of degrees
  //from zero to 3599 and provided in binary format over the two bytes."
  byte MSB = Wire.read();
  byte LSB = Wire.read();

  float headingSum = (MSB << 8) + LSB; //(MSB / LSB sum)
  float headingInt = headingSum / 10; 
  
  return headingInt;
}

void Compass::_beginTransmission(){
  Wire.beginTransmission(HMC6352SlaveAddress);
  Wire.write(HMC6352ReadAddress);
}
void Compass::_endTransmission(){
  Wire.endTransmission();
}
#endif
