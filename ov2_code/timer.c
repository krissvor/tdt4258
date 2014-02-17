#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

#define CLOCK_PERIOD_44KHZ 317

/* function to setup the timer */
void setupTimer(uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = CLOCK_PERIOD_44KHZ;
  *TIMER1_IEN = 1;
  *ISER0 |= (1 << 12);
  *TIMER1_CMD = 1;
}


