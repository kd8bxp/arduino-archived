#include<SoftwareSerial.h>
#define RxD 6
#define TxD 7
SoftwareSerial Serial1(RxD,TxD);

void setup()
{
	Serial.begin(9600);
	Serial1.begin(38400);
	delay(1000);
	initialize_bluetooth();
//connecting to the bluetooth device with the address "00:12:11:17:02:10"
	/*Note that this command cannot be issued unless the module is in pairable state,
	in other words, the "AT+INIT" then the "AT+INQ" commands should be first issued in order.
	It worth mentioning that this is done in the initialization function*/
	Serial1.print("AT+LINK=0012,11,170210\r\n"); //This command used only in one of the two devices
	/*Note that the address of the device which we want to connect to can be got using the
	"AT+INQ" command. Then the response string will be on the form: "+INQ:00:12:11:17:02:10,xx,yy".
	The address string can be then extracted from the response string by some simple code
	manipulation and then used in the "AT+LINK" command to establish the connection. For more
	information about string operations in Arduino, please consult the Arduino reference.*/
	delay(1000);
}
void loop()
{
//----------------------------------------------------------
//Simple code for the master sending a message to the slave
//----------------------------------------------------------
//transmitting data to the slave
	Serial1.print("Hello from Emar to RAM!\r\n");
	Serial1.flush(); //this makes the code stops untill all serial data finish transmission
	delay(500);
//waiting for the slave response and print the response on the serial monitor
	while(1)
	{
		if(Serial1.available() > 0)
		{
			print_bt_response();
			break;
		}
	}
	/*
	//--------------------------------------------------------------------------------
	//Simple code for the slave receiving a message from the master and then responding back
	//--------------------------------------------------------------------------------
	//receiving data from the master by the slave
	while(1)
	{
	if(Serial1.available()>0)
	{
	print_bt_response();
	break;
	}
	}
	//responding back to the master
	Serial1.print("Hello from RAM to Emar!\n");
	Serial1.flush(); //this makes the code stops untill all serial data finish transmission
	delay(500);
	*/
}
//**********************************************************************************
//Function that reads BT received data and returns 1 if successfull and 0 otherwise
//**********************************************************************************
int print_bt_response()
{
	int response;
	char out, outprev = '$';
	while(Serial1.available() > 0)
	{
		out = (char)Serial1.read();
		Serial.print(out);
		if ((outprev == 'O') && (out == 'K'))
		{
			Serial.print("\n");
			response = 1;
			return response;
		}
		outprev = out;
	}
	response = 0;
	return response;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//********************************************************************************
//Initializing the bluetooth and put it into the pairable state to be ready for connection
//********************************************************************************
void initialize_bluetooth()
{
	int flag = 2;
	delay(500);
//restore default status
	Serial1.print("AT+ORGL\r\n");
	flag = print_bt_response();
	delay(500);
	if(flag == 1)
	{
		Serial.print("Module default settings restored ... Success(1)\n\n");
	}
	else if(flag == 0)
	{
		Serial.print("Failed(1)\n\n");
	}
	flag = 2;
//set bt name as "BT_Test"
	Serial1.print("AT+NAME=BT_Test\r\n");
	flag = print_bt_response();
	delay(500);
	if(flag == 1)
	{
		Serial.print("Module took new name ... Success(2)\n\n");
	}
	else if(flag == 0)
	{
		Serial.print("Failed(2)\n\n");
	}
	flag = 2;
//set pin code (should be the same for the master and slave for establishing //connection)
	Serial1.print("AT+PIN1234\r\n");
	flag = print_bt_response();
	delay(500);
	if(flag == 1)
	{
		Serial.print("Module took new password ... Success(3)\n\n");
	}
	else if(flag == 0)
	{
		Serial.print("Failed(3)\n\n");
	}
	flag = 2;
	delay(1000);
//initialize spp profile lib (should be issued before issuing the next command)
	Serial1.print("AT+INIT\r\n");
	flag = print_bt_response();
	delay(1000);
	Serial1.print("AT+INIT\r\n");
	flag = print_bt_response();
	delay(1000);
	if(flag == 1)
	{
		Serial.print("Module spp profile lib initialized ... Success(4)\n\n");
	}
	else if(flag == 0)
	{
		Serial.print("Failed(4)\n\n");
	}
	flag = 2;
//set to pairable state (to be seen by other bluetooth devices and to be ready for
//connection)
	Serial1.print("AT+INQ\r\n");
	flag = print_bt_response();
	
	delay(500);
	if(flag == 1)
	{
		Serial.print("Module inquirable (set to pairable state) ... Success(5)\n\n");
	}
	else if(flag == 0)
	{
		Serial.print("Failed(5)\n\n");
	}
	flag = 2;
//if you want to set bt to be master, then "AT+ROLE=1"
//if you want to set bt to be slave, then "AT+ROLE=0"
	Serial1.print("AT+ROLE=1\r\n"); //set as master here
	flag = print_bt_response();
	delay(500);
	if(flag == 1)
	{
		Serial.print("Module role set to master ... Success(6)\n\n");
	}
	else if(flag == 0)
	{
		Serial.print("Failed(6)\n\n");
	}
	flag = 2;
}