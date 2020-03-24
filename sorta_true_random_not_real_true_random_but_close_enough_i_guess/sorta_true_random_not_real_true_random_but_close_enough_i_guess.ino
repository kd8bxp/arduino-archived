#include <EEPROM.h>

void reseedRandom( void )
{
  static const uint32_t HappyPrime = 937;
  union
  {
    uint32_t i;
    uint8_t b[4];
  }
  raw;
  int8_t i;
  unsigned int seed;
 
  for ( i=0; i < sizeof(raw.b); ++i )
  {
    raw.b[i] = EEPROM.read( i );
  }

  do
  {
    raw.i += HappyPrime;
    seed = raw.i & 0x7FFFFFFF;
  }
  while ( (seed < 1) || (seed > 2147483646) );

  randomSeed( seed ); 

  for ( i=0; i < sizeof(raw.b); ++i )
  {
    EEPROM.write( i, raw.b[i] );
  }
}

void setup( void )
{
  Serial.begin(115200);
  reseedRandom();
}

void loop() {
  for (int x=0; x<10; x++) {
  Serial.println(random(1,7));
  reseedRandom();
  //delay(1000);
  }
  while(1);
}
