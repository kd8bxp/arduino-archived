#ifndef ROBONII_SOUND_H
#define ROBONII_SOUND_H

/*
This Library is to be used with the Robonii product. 
Schematics can be downloaded from "www.robonii.co.za"
A sample of how to use this library is given at the end of "Driver_RFID.c"
The RFID chip can not be driven from USB power.
Ensure that the RFID antenna is plugged in, else the device might get stuck in an interupt trying to decode random pulses

//Sounds to play : 1-20
*/

#include "..\XTimer\XTimer.h"
#include "..\dma_driver\dma_driver.h"

#define ENABLE_SOUND_PWM TCE0.CTRLB |= TC0_CCCEN_bm
#define ENABLE_SOUND_AMP PORTD.DIRSET = PIN7_bm; PORTD.OUT |= PIN7_bm


#define DISABLE_SOUND_PWM TCE0.CTRLB &= ~TC0_CCCEN_bm
#define DISABLE_SOUND_AMP PORTD.DIRCLR = PIN7_bm



#define SOUND_BUFFER_SIZE 400
#define SOUNDARRAYSIZE		120	





/* === Typedefs ================================================================================ */
/** 
 * Struct to denote a single audio object, containing a tone frequency array as well as a 
 * corresponding sound duration array.
*/
typedef struct 
{													
	unsigned char cToneFrequency[SOUNDARRAYSIZE];
	signed int sToneLength[SOUNDARRAYSIZE];

} Sound_t;

/** Enumeration to indicate the various sound construction functions read from eeprom. */
enum SoundFunctions
{
	RAMPUP 		= 1,
	RAMPDOWN 	= 2,
	WARBLE		= 3,
	TONE		= 4
} ;


enum sound_type_e
{
  SOUND_NONE,
  SOUND_TONE,
  SOUND_WAV
} ;

 enum SoundPriority
{
	SND_NO_PRIORITY = 0,
	
	SND_LOW_L2	= 48,
	SND_LOW_L1	= 49,
	SND_LOW		= 50,
	SND_LOW_H1  = 51,
	SND_LOW_H2	= 52,

	SND_MED_L2	= 98,
	SND_MED_L1	= 99,
	SND_MED 	= 100,
	SND_MED_H1	= 101,
	SND_MED_H2	= 102,

	SND_HIGH_L2 = 148,
	SND_HIGH_L1	= 149,
	SND_HIGH	= 150,
	SND_HIGH_H1 = 151,
	SND_HIGH_H2 = 152
} ;
/*

void InitSound(void);
void SoundGenerator(unsigned char toneArray[], signed int durationArray[], signed char length);

void PlaySound(unsigned char tuneNumber, SoundPriority priority);

void sound_task(void);

unsigned char  BuildCustomSound(void);

static void LoadNextSimpleTone(void);

static void dma_play_sound( void );

*/



#include <avr/io.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>
 void LoadNextSimpleTone();
  void DisableSound();

// ====== Defines =====================================================

//====================================================================


class Robonii_Sound_Driver {
 public:
 
	void InitSound();
	void SoundGenerator(unsigned char toneArray[], signed int durationArray[], signed char length);

	 void PlaySound(unsigned char tuneNumber, unsigned char  priority2);
	void PlayAdvancedSound(uint8_t soundNumber);
	void sound_task();

	unsigned char  BuildCustomSound();

	uint8_t apply_sound_volume();
	unsigned char updated;
	Robonii_Sound_Driver();
	void dma_buffer_switch();
	void StopSound();
	//void LoadNextSimpleTone();

 
	 

	//static void dma_play_sound(  );
	 //void StartSound();
	
	 
	//static void LoadNextSimpleTone();
	
	 void NextTone(unsigned char tone, int duration);
		void RampUp(unsigned char startTone, unsigned char endTone, int duration, unsigned char incrementTone);
	void Warble(unsigned char tone, int intensity, signed char rampDirection, int duration);
	void CentreOffsetSound(uint8_t midTone, uint8_t offset, uint8_t intensity, uint8_t increment, bool rampOut);
	void RampDown(unsigned char startTone, unsigned char endTone, int duration, unsigned char decrementTone);
	static uint16_t load_dma_buffer();
 
	void dma_play_sound(  );
 private:

 };
  
extern Robonii_Sound_Driver Robonii_Sounds;
// Robonii_Sound_Driver::



/*
	static void LoadNextSimpleTone();

	//static void dma_play_sound(  );
	static void StartSound();
	static void DisableSound();
	static void StopSound();
	//static void LoadNextSimpleTone();
	void PlayAdvancedSound(uint8_t soundNumber);
	static void NextTone(unsigned char tone, int duration);
   static void RampUp(unsigned char startTone, unsigned char endTone, int duration, unsigned char incrementTone);
static void Warble(unsigned char tone, int intensity, signed char rampDirection, int duration);
static void CentreOffsetSound(uint8_t midTone, uint8_t offset, uint8_t intensity, uint8_t increment, bool rampOut);
static void RampDown(unsigned char startTone, unsigned char endTone, int duration, unsigned char decrementTone);
static uint16_t load_dma_buffer();
static void dma_buffer_switch();
static void dma_play_sound(  );


*/

#endif
