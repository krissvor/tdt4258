#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"


/* 
  The core clock (which the timer clock is derived from)
  runs at 14 MHz by default. Also remember that the timer counter
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
	 * Sleep while waiting for interrupts
	 */
	*SCR = 6; //0110
	__asm volatile ("wfi");

	return 0;
}

