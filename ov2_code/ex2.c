#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"


/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   317


/* Your code will start executing here */
int main(void) 
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);


	/*
	 * TODO for higher energy efficiency, sleep while waiting for interrupts
	 * instead of infinite loop for busy-waiting
	 */
	while(1);

	return 0;
}




