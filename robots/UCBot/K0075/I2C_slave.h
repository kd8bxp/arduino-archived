#ifndef __I2C_SLAVE_H__
#define __I2C_SLAVE_H__



//stm32 address definetion
#define     STM32_1_SLAVE_ADDR		0x20    //contain the control of motor/color sensor/track module
#define     STM32_2_SLAVE_ADDR		0x21    //contain the control of power/avoid/follow/buzzer/rgb led/color sensor led/ultrasonic
#define 	  ENABLE  				      1
#define 	  DISENABLE  				    1

//stm32 registers
#define MOTOR_REG				    0XE0    //control the motor direction
#define SPEED_REG				    0XE1    //control the motor speed, 0~100
#define COLOR_R_REG			    0XE2    //read the red value of color sensor 
#define COLOR_G_REG			    0XE3    //read the green value of color sensor 
#define COLOR_B_REG			    0XE4    //read the blue value of color sensor 
#define ULTRA_REG				    0XE5    //read the distance of ultrasonic detected
#define POWER_REG				    0XE6    //read the power
#define LED_R_REG				    0XE7    //write the red value of rgb led 
#define LED_G_REG				    0XE8    //write the green value of rgb led
#define LED_B_REG				    0XE9    //write the blue value of rgb led
#define SONG_REG				    0XEA    //play the music onboard 
#define MOTOR_CAL_REG		    0XEB    //set the calibration value of the left and right motor,-20~+20
#define COLOR_CAL_REG		    0XEC    //reserved
#define RUN_MODE_REG		    0XED    //reserved
#define TRACE_REG				    0XEE    //track mode
#define NOTE_REG				    0XEF    //play the notes
#define LED_SET_REG			    0XF0    //open the rgb led
#define AVOID_REG		    		0XF1    //avoid mode
#define IR_REG					    0XF2    //IR remote control
#define ULTRA_DIS_REG		    0XF3    //set the effective distance of ultrasonic
#define FOLLOW_REG			    0XF4    //follow mode
#define IR_STATUS		    		0XF5    //ir decode status
#define ESP32_STATUS        0XF6    //stm32 status, write ture to indicate that esp32 is up and running
#define TCS_LED_SWITCH      0XF7    //color sensor led
#define PIANO_MODE	        0XF8    //piano mode
#define TCS_STATUS					0XF9	  //color sensor status
#define CAMERA_STATUS				0XFA	  //camera init status

//motor status
#define		STOP 				    	1
#define   FORWARD		  		  2
#define		BACK				    	3
#define		LEFT				    	4
#define		RIGHT				    	5
#define		BACK_RIGHT		    6
#define		BACK_LEFT			    7
#define 	RIGHT_FORWARD     8
#define 	LEFT_FORWARD      9

//song
#define BUZZER_OFF	        0
#define HAPPY_BIRTHDAY      1
#define MERRY_CHRISTMAS     2
#define JINGLE_BELLS	      3
#define SONG_OF_JOY		      4

//sccb function
byte wrSensorReg8_8(uint8_t sensor_addr, int regID, int regDat);
byte rdSensorReg8_8(uint8_t sensor_addr, uint8_t regID, uint8_t* regDat);

extern uint8_t avoid_mode;
extern uint8_t follow_mode;

#endif // __I2C_SLAVE_H__
