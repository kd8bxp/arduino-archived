
#ifndef DATATYPES_
#define DATATYPES_

#include <Arduino.h>


struct PID_DATA
{
	double kp;
	double ki;
	double kd;
	double setpoint;
	double input;
	double output;
};

struct AHRS {
	float pitch;
	float roll;
	float yaw;
};





#endif // !DATATYPES_