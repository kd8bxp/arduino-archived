// common anode RGB LED Analog PWM Pin control example
//
// example found on 
// http://www.instructables.com/id/RGB-LED-Tutorial-using-an-Arduino-RGBL/step5/Analog-Arduino-Code/

// connecting the RGB LED
// +5 volt to common anode pin (the longest one)
// use 3 resistors 300+ Ohm to 1 K to connect each of the other pins to ground for testing the led
// I used 1K resistors

// number of pwm pins limits the number of leds controled
// following sample for controlling one RGB LED 
// I used an arduino nano v3
// pin D9, D10, D11

// connecting the RGB LED
//
// connect +5V to common anode pin (the longest lead)
// connect the tree other pins via a 1K resistor to D9, D10 and D11

int pwm_a = 9;
int pwm_b = 10;
int pwm_c = 11;

int i;
int baseval = 0;
void setup()
{
	pinMode(pwm_a, OUTPUT);
	pinMode(pwm_b, OUTPUT);
	pinMode(pwm_c, OUTPUT);

	analogWrite(pwm_a, 255);
	analogWrite(pwm_b, 255);
	analogWrite(pwm_c, 255);

	Serial.begin(9600);
}

void loop()
{
	// uncomment the effect you want 	
	
//	fade_allchannels();

//	fade_channel(pwm_a);
//	fade_channel(pwm_b);
//	fade_channel(pwm_c);

	test_led();
}

void test_led()
{
	analogWrite(pwm_a, 0);
	delay(1000);
	analogWrite(pwm_a, 255);
	
	delay(500);
	
	analogWrite(pwm_b, 0);
	delay(1000);
	analogWrite(pwm_b, 255);
	
	delay(500);
	
	analogWrite(pwm_c, 0);
	delay(1000);
	analogWrite(pwm_c, 255);
	
	delay(500);
}


void fade_allchannels()
{
	//
	//fade all channels up and down.
	//

	Serial.println("Fading all PWM channels up to max.");

	for (i=baseval; i<=255; i++)
	{
		analogWrite(pwm_a, i);
		analogWrite(pwm_b, i);
		analogWrite(pwm_c, i);

		delay(2);
	}

	Serial.println("All Power WM channels at max.");

	// delay(5);
	Serial.println("Fading all channels to 0");

	for (i=255; i>=baseval; i--)
	{
		analogWrite(pwm_a, i);
		analogWrite(pwm_b, i);
		analogWrite(pwm_c, i);
	
		delay(2);
	}

	Serial.println("All pwm channels at zero.");

	// delay(5);
}

void fade_channel(int channel)
{
	Serial.println("Fading pwm channel to max: ");
	Serial.println(channel);

	for (i=baseval; i<=255; i++)
	{
		analogWrite(channel, i);
		delay(5);
	}

	Serial.println("pwm channel at max.");
	
	delay(5);

	Serial.println("fading down.");

	for (i=255; i>=baseval; i--)
	{
		analogWrite(channel, i);
		delay(5);
	}

	Serial.println("pwm channel at 0.");

	delay(5);
}
