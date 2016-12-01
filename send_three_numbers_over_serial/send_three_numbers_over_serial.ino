
byte R,G;
signed int B,XX;


char hex[9] = {0};

int first;
int second;
signed int third;
signed int fourth;

void setup()
{
	R = 255;
G = 125;
B = 1;
XX = -1;

	Serial.begin(9600);

sprintf(hex,"%02X%02X %02d%02d",R,G,B,XX);


	Serial.println(hex);

sscanf (hex, "%02X%02X %02d%02d", &first, &second, &third,&fourth);

//first = (int)hex/100;
//second = (int)hex%100/10;
//third = (int)hex%10;

//first = hex/1000;
//second = hex%1000/100;
//third = hex%100;

Serial.println(first);
//Serial.print(" ");
//Serial.println(first, HEX);
Serial.println(second);
Serial.println(third);
Serial.println(fourth);



}

void loop()
{

}
