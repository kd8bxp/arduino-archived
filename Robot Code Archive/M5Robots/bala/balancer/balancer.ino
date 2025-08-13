///****************************
///@file balancer.ino
/// @brief self balancing robot
///****************************


//The M5stack fire has the MPU6886
#define M5STACK_MPU6886 

#include "src/M5Stack/M5Stack.h"
#include "src/PID/PID_v1.h"
#include "bala.h"
#include "SelectionMode.h"
#include "WiFi.h"
#include "WiFiUdp.h"
#include "datatypes.h"
#include "PIDTuner.h"
#include <string>

const char ssid[] = "NETGEAR32";
const char pass[] = "savetheworld123";

WiFiUDP udp;

Bala bala;
Selector selector;


//kp = 25 has sustained oscillation with a time period of 0.4s
//new kp = 0.45*kp_old
//new ki = timeperiod/1.2
//double kp = 25, ki = 0, kd = 0.0;
PID_DATA outer_pid_data{ 11.25, 0.4 / 1.2, -6.0, 0, 0, 0 };


PID outer_pid(&outer_pid_data.input, &outer_pid_data.output, &outer_pid_data.setpoint, outer_pid_data.kp, 
	outer_pid_data.ki, outer_pid_data.kd, DIRECT);

PIDTuner pidtunner(outer_pid, outer_pid_data);

int16_t wheel_left_speed = 0;
int16_t wheel_right_speed = 0;

unsigned long loop_start = 0;
unsigned long loop_last_reset_time = 0;
unsigned long last_locked_time = 0;
double& speed_demand = outer_pid_data.output;

AHRS attitude {0,0,0};
int lcd_refresh = 0;


void setup() {
    // Initialize the M5Stack object
    M5.begin(true, false, false, false);

    Serial.begin(115200);
    M5.IMU.Init();
	bala.SetSpeed(0, 0);
	bala.SetEncoder(0, 0);
	loop_start = millis();

	outer_pid.SetMode(AUTOMATIC);
	outer_pid.SetOutputLimits(-1022, 1023);
	
	M5.Lcd.setCursor(0, 0);
	
	M5.Lcd.println("PID Output max: " + String(outer_pid.GetOutMax()));
	M5.Lcd.println("PID Output min: " + String(outer_pid.GetOutMin()));
	

	delay(3000);
}

// the loop routine runs over and over again forever
void loop() {
	loop_start = millis();
	//Get Roll data
	M5.IMU.getAhrsData(&attitude.pitch, &attitude.roll, &attitude.yaw);
	//Calculate PID output
	input = -attitude.roll;
	outer_pid.Compute();
	
	//Update the wheel speed
	int16_t output_i16 = static_cast<int>(output);
	wheel_left_speed = output_i16;
	wheel_right_speed = output_i16;
	
	//Only update wheel speed if angle is in range
	if (input > -70 && input < 70) 
	{
		bala.SetSpeed(wheel_left_speed, wheel_right_speed);
	}
	else 
	{
		bala.SetSpeed(0, 0);
	}
	
    // Get motor encoder value
    bala.UpdateEncoder();
    auto encoder = bala.wheel_left_encoder;

	
	
	M5.Lcd.setCursor(0, 0);
	M5.Lcd.println("Roll: " + String(attitude.roll));
	M5.Lcd.println("Pitch: " + String(attitude.pitch));
	M5.Lcd.println("Yaw: " + String(attitude.yaw));
	M5.Lcd.println("Output: " + String(output));
	//Print kp, ki, kd and setpoint in one line
	M5.Lcd.println("Kp: " + String(kp) + " Ki: " + String(ki) + " Kd: " + String(kd) + " Setpoint: " + String(setpoint));
	M5.Lcd.println("Selection Mode: " + selector.getSelectionString());
	
	//Display battery percentage
	M5.Lcd.println("\n Battery: " + String(M5.Power.getBatteryLevel()) + "%");

	
	
	if (loop_start - loop_last_reset_time > 2000)
	{
		loop_last_reset_time = loop_start;
		M5.Lcd.clear();	
	}


	M5.update();
	
}
