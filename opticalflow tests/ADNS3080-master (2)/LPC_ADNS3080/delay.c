#include <NXP/iolpc2103.h>

#define MR0I (1<<0)
#define MR0R (1<<1)
#define MR0S (1<<2)

/** this is pretty half-ass and inaccurate but I'll get to that later */
void delayMicroseconds(unsigned int us)
{
  // Should probably subtract # of usec it takes to run the following
  // 60MHz => 0.01667 usec / instruction

  /* initialize timer */  
  T0PR = 15;              // Prescaler -- 60MHz /4 = 15MHz /15 = 1MHz
  T0TCR = 0x02;           // Reset counter and prescale
  T0CTCR = 0x0;           // Timer0 increments with every rising PCLK
  T0MR0 = us;             // Set cycle time -- 1MHz so 1usec per LSB
  T0MCR = MR0S;           // On match0, stop timer
  T0EMR = 0x00;           // On match do nothing.
  T0TCR = 1;              // Enable timer

  while (T0TC != us);   // stop when T0TC == usec
 }

/** yes I realize this isn't spot-on accurate yet */
void delay(unsigned int ms) 
{
  for (int i=0; i < ms; i++)
    delayMicroseconds(1000);
}
