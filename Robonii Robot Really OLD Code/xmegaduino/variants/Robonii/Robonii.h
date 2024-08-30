#ifndef Robonii_h
#define Robonii_h

//To use in Arduinio. type:  #include <robonii.h>    at the top of your sketch

//A2D - Special Functions
#define A_VBAT A3
#define VBat_Pin	3
#define VBat_Resistor_Scale 0.10569
#define A2D_12Bit_1VRef_bit_Value  0.0009765


//LED's
#define D1_Green	12
#define D1_Red 		13
#define D2_Green	14
#define D2_Red 		15
#define D3_Green	16
#define D3_Red 		17
#define D4_Green	18
#define D4_Red 		19
#define D5_Green	20
#define D5_Red 		21

//Bumpers
#define Bumper_Front	9
#define Bumper_Rear		8

//Obstacle
#define Obstacle_Left	10
#define Obstacle_Right	11



//Sound
#define	Sound_Pin		28
#define	Sound_Shutdown_Pin	27

//RFID
#define RFID_Field_Enable_Pin	29
#define RFID_Input_Pin	


//MOTOR
#define MOTOR1A_Pin				22
#define MOTOR1B_Pin				23
#define MOTOR2A_Pin				24
#define MOTOR2B_Pin				25
#define MOTOR-Power Save_Pin	26

#define MOTOR1AStallCurrent_Pin				4
#define MOTOR1BStallCurrent_Pin				5
#define MOTOR2AStallCurrent_Pin				6
#define MOTOR2BStallCurrent_Pin				7

#define TWI_SDA				31
#define TWI_SCL				32



#endif
