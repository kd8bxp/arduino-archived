#ifndef Compass_h
#define Compass_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



#define UseHMC5883L//定义使用HMC5883L指南针
//#define UseHMC6352//如果我们使用的是HMC6530则去掉这个注释把上面的注释掉就行了

#ifdef UseHMC6352
//0x21==0x42>>1, from bildr's code
#define HMC6352SlaveAddress 0x21
#define HMC6352ReadAddress 0x41
#endif

#ifdef UseHMC5883L //在使用HMC5883下使用的一些地址定义
#define HMC5883L_Address 0x1E
#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define DataRegisterBegin 0x03
#define IdentityRegister 0x0A
#define IdentityRegisterValue 0x48

#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03

#define ErrorCode_1 "Entered scale was not valid, valid gauss values are: 0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1"
#define ErrorCode_1_Num 1

#define MagnetcDeclination 4.43 //Shanghai
#define CalThreshold 0

struct MagnetometerScaled
{
	float XAxis;
	float YAxis;
	float ZAxis;
};

struct MagnetometerRaw
{
	int XAxis;
	int YAxis;
	int ZAxis;
};

class Compass
{
	public:
	  Compass();
	  void begin();
	  float getReading();
	  MagnetometerRaw ReadRawAxis();
	  MagnetometerScaled ReadScaledAxis();

	  int SetMeasurementMode(uint8_t mode);
	  int SetScale(float gauss);

	  char* GetErrorText(int errorCode);

	  uint8_t EnsureConnected();
	  uint8_t IsConnected;

	protected:
	  void Write(int address, int byte);
	  uint8_t* Read(int address, int length);
	private:
	  float m_Scale;
	  uint8_t ComCalibraFlag;//电子罗盘 校准标志位
	  //void _beginTransmission();
	  //void _endTransmission();
};

#endif

#ifdef UseHMCUseHMC6352
class Compass{
	public:
		void begin();
		float getReading();
	private:
		void _beginTransmission();
		void _endTransmission();

};

#endif

#endif
